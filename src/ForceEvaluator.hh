#ifndef FORCEEVALUATOR_HH
#define FORCEEVALUATOR_HH

#include "Grid.hh"

#include <memory>

class Interaction
{
public:

    Interaction( unsigned _i, unsigned _j, Imath::V2f& _sep, float _r2 )
     : i( _i ), j( _j ), sep( _sep ), r2( _r2 ) {}

public:

    unsigned i, j;
    Imath::V2f sep;
    float r2;
};

typedef std::vector< Interaction > InteractionArray;


class ForceEvaluator
{
public:

    struct EvaluationData
    {
        EvaluationData(
                VectorArray& _p,
                VectorArray& _v,
                FloatArray& _m,
                FloatArray& _d,
                FloatArray& _pr
                )
         : position( _p ), velocity( _v ), mass( _m ), density( _d ), pressure( _pr ) {}

        VectorArray& position;
        VectorArray& velocity;
        FloatArray& mass;
        FloatArray& density;
        FloatArray& pressure;
    };

    ForceEvaluator(
            const float h,
            const float viscosity,
            const float gravity,
            const float pressureConstant,
            const float averageDensity,
            const GridFactory& gridFactory
            )
     : m_h( h ),
       m_viscosity( viscosity ),
       m_gravity( gravity ), 
       m_pressureConstant( pressureConstant ),
       m_averageDensity( averageDensity ),
       m_gridFactory( gridFactory ) {}

    void evaluate( EvaluationData& data, VectorArray& force )
    {
        Bounds bounds;

        unsigned int particleCount = data.position.size();

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            bounds.extendBy( data.position[i] );
        }

        evaluate( data, bounds, force );
    }

    void evaluate( EvaluationData& data, const Bounds& bounds, VectorArray& force )
    {
        InteractionArray interactions;

        initialPass( data, bounds, interactions, force );

        pressurePass( data, interactions, force );

        viscosityPass( data, interactions, force );
    }

    void initialPass( EvaluationData& data, const Bounds& bounds, InteractionArray& interactions, VectorArray& force )
    {
        VectorArray& position = data.position;
        FloatArray& density = data.density;
        FloatArray& mass = data.mass;
        FloatArray& pressure = data.pressure;

        const float h = m_h;
        const float h2 = h * h;
        const float h3 = h2 * h;
        const float h5 = h2 * h2 * h;
        const float h6 = h5 * h;
        const float h9 = h6 * h2 * h;

        std::auto_ptr< const Grid > grid( m_gridFactory.create( data.position, bounds ) );

        // Double counting everything!!!!!!!!!!!!!!!!!!
        //
        // Still double counting within a cell!!!

        // Iterator over the whole grid, minus the border
        for ( unsigned int y=1; y < grid->yRes() - 1; ++y )
        {
            for ( unsigned int x=1; x < grid->xRes() - 1; ++x )
            {
                const Cell& cell = grid->cell( x, y );

                // Iterate over the 9 cells around the current one
                for ( int i=-1; i < 2; ++i )
                {
                    for ( int j=0; j < 2; ++j )
                    {
                        // Don't double compare grid cells
                        if ( i == -1 && j == 0 )
                            continue;

                        const Cell& otherCell = grid->cell( x + i, y + j );

                        // Iterate over all the particles in the cell
                        for ( unsigned int p=0; p<cell.size(); ++p )
                        {
                            int pp = cell[ p ];

                            // Iterate over the particles in the other cell
                            for ( unsigned int q=0; q<otherCell.size(); ++q )
                            {
                                int qp = otherCell[ q ];

                                // Don't double compare particles in the same cell
                                if ( i == 0 && j == 0 && p >= q )
                                    continue;

                                Imath::V2f diff = position[ pp ] - position[ qp ];
                                const float lengthSquared = diff.x * diff.x + diff.y * diff.y;

                                if ( lengthSquared > h2 )
                                    continue;

                                //  Calculate Kernel contribution for p1 from p2
                                //
                                //  Use poly6 kernel as noted in the siggraph course notes
                                //
                                const float r2 = lengthSquared;
                                const float a = h2 - r2;
                                const float a3 = a * a * a;
                                const float W = ( 315.0f * a3 ) / ( 64.0f * M_PI * h9 );

                                interactions.push_back( Interaction( pp, qp, diff, r2 ) );

                                //  Calculate density for each particle
                                //
                                density[ pp ] += W * mass[ qp ];
                                density[ qp ] += W * mass[ pp ];
                            }
                        }
                    }
                }

                // Iterate over all the particles in the cell
                for ( unsigned int p=0; p<cell.size(); ++p )
                {
                    int pp = cell[ p ];

                    // Account for the mass of the particle itself
                    const float W = 315.0f / ( 64.0f * M_PI * h3 );
                    density[ pp ] += W * mass [ pp ];

                    // Calculate pressure based on density
                    pressure[ pp ] = m_pressureConstant * ( density[ pp ] - m_averageDensity );

                    // Apply external forces
                    const Imath::V2f down( 0.0, -1.0 );
                    force[ pp ] += down * mass[ pp ] * m_gravity;
                }
            }
        }
    }


    void pressurePass( EvaluationData& data, InteractionArray& interactions, VectorArray& force )
    {
        unsigned int numInteractions = interactions.size();

        FloatArray& density = data.density;
        FloatArray& mass = data.mass;
        FloatArray& pressure = data.pressure;
        
        const float h = m_h;
        const float h2 = h * h;
        const float h5 = h2 * h2 * h;
        const float h6 = h5 * h;

        for ( unsigned int i=0; i<numInteractions; ++i )
        {
            Interaction& interaction = interactions[i];
            
            unsigned int i = interaction.i;
            unsigned int j = interaction.j;

            const float r2 = interaction.r2;
            const float r = sqrt( r2 );

            // Don't calculate for zero separation as the force would be zero but
            // due to the 1/r part it is nan
            // TODO: Figure out why this happens so much
            if ( r2 != 0 )
            {
                //  Pressure calculations
                //
                //  Using Spiky kernel as noted in literature
                //
                const float c = - ( 45.0f / ( M_PI * h6 ) );
                const float a = ( ( h2 + r2 ) / r ) - 2 * h;

                const Imath::V2f dWdr = c * a * interaction.sep;

                const Imath::V2f f1_pressure = ( mass[j] * ( pressure[i] + pressure[j] ) * dWdr ) / ( 2 * density[j] );
                const Imath::V2f f2_pressure = ( mass[i] * ( pressure[i] + pressure[j] ) * dWdr ) / ( 2 * density[i] );

                force[ i ] += - f1_pressure;
                force[ j ] += f2_pressure;
            }
        }
    }


    void viscosityPass( EvaluationData& data, InteractionArray& interactions, VectorArray& force )
    {
        unsigned int numInteractions = interactions.size();

        FloatArray& density = data.density;
        FloatArray& mass = data.mass;
        VectorArray& vel = data.velocity;
        
        const float h = m_h;
        const float h2 = h * h;
        const float h5 = h2 * h2 * h;

        for ( unsigned int i=0; i<numInteractions; ++i )
        {
            Interaction& interaction = interactions[i];
            
            unsigned int i = interaction.i;
            unsigned int j = interaction.j;

            const float r2 = interaction.r2;
            const float r = sqrt( r2 );

            //  Viscosity Calculations
            //
            //  Using viscosity kernel as recommended in literature
            //
            const float c = 90.0f / ( 2 * M_PI * h5 );
            const float d2Wdr2 = c * ( 1 - r / h );

            // viscosity of water

            const Imath::V2f f1_viscosity = m_viscosity * mass[j] * ( vel[j] - vel[i] ) * d2Wdr2 / ( density[j] );
            const Imath::V2f f2_viscosity = m_viscosity * mass[i] * ( vel[i] - vel[j] ) * d2Wdr2 / ( density[i] );

            force[i] += f1_viscosity;
            force[j] += f2_viscosity;
        }
    }

private:

    const float m_h;
    const float m_viscosity;
    const float m_gravity;
    const float m_pressureConstant;
    const float m_averageDensity;

    const GridFactory& m_gridFactory;

};

#endif // FORCEEVALUATOR_HH

