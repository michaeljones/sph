#ifndef FORCEEVALUATOR_HH
#define FORCEEVALUATOR_HH

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
            float h,
            float viscosity,
            float gravity
            )
     : m_h( h ),
       m_viscosity( viscosity ),
       m_gravity( gravity ) {}

    void evaluate( EvaluationData& data, VectorArray& force )
    {
        InteractionArray interactions;

        initialPass( data, interactions, force );

        pressurePass( data, interactions, force );

        viscosityPass( data, interactions, force );
    }

    void initialPass( EvaluationData& data, InteractionArray& interactions, VectorArray& force )
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

        unsigned int particleCount = position.size();

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            for ( unsigned int j=i+1; j<particleCount; ++j )
            {
                Imath::V2f diff = position[ i ] - position[ j ];
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

                interactions.push_back( Interaction( i, j, diff, r2 ) );

                //  Calculate density for each particle
                //
                density[ i ] += W * mass[ j ];
                density[ j ] += W * mass[ i ];
            }

            // Account for the mass of the particle itself
            const float W = 315.0f / ( 64.0f * M_PI * h3 );
            density[ i ] += W * mass [ i ];

            // Calculate pressure based on density
            const float k = 0.5f;
            const float averageDensity = 1.0f;
            pressure[ i ] = k * ( density[ i ] - averageDensity );

            // Apply external forces
            const Imath::V2f down( 0.0, -1.0 );
            force[ i ] += down * mass[ i ] * m_gravity;
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

};

#endif // FORCEEVALUATOR_HH

