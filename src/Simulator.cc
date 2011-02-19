
#include "Simulator.hh"

#include <math.h>

class Interaction
{
public:

    Interaction( unsigned _i, unsigned _j, Imath::V2f& _sep, float _r2, float _W )
     : i( _i ), j( _j ), sep( _sep ), r2( _r2 ), W( _W ) {}

public:

    unsigned i, j;
    Imath::V2f sep;
    float r2;
    float W;
};

void Simulator::step()
{
    const float smoothingDistance = 0.5;
    const float smoothingDistanceSquared = smoothingDistance * smoothingDistance;

    unsigned int numParticles = m_particles.size();

    std::vector< Interaction > interactions;

    const float d = smoothingDistance;
    const float d2 = d * d;
    const float d9 = d2 * d2 * d2 * d2 * d;

    for ( unsigned int i=0; i<numParticles; ++i )
    {
        Particle* p1 = m_particles[i];

        for ( unsigned int j=i+1; j<numParticles; ++j )
        {
            // We don't skip the i==j case as the poly6 is not singular
            //

            Particle* p2 = m_particles[j];
            
            Imath::V2f diff = p1->pos - p2->pos;

            float lengthSquared = diff.x * diff.x + diff.y * diff.y;

            if ( lengthSquared > smoothingDistanceSquared )
                continue;

            //  Calculate Kernel contribution for p1 from p2
            //
            //  Use poly6 kernel as noted in the siggraph course notes
            //
            const float r2 = lengthSquared;
            const float a = d2 - r2;
            const float a3 = a * a * a;
            const float W = ( 315.0f * a3 ) / ( 64.0f * M_PI * d9 );

            interactions.push_back( Interaction( i, j, diff, r2, W ) );

            // Calculate density for each particle
            //
            p1->density += W * p2->mass;
            p2->density += W * p1->mass;
        }

        const float k = 1.0f;
        const float averageDensity = 1.0f;
        p1->pressure = k * ( p1->density - averageDensity );
    }

    unsigned int numInteractions = interactions.size();
    
    for ( unsigned int i=0; i<numInteractions; ++i )
    {
        Interaction& interaction = interactions[i];
        
        Particle* p1 = m_particles[ interaction.i ];
        Particle* p2 = m_particles[ interaction.j ];


        const float r2 = interaction.r2;
        const float r = sqrt( r2 );

        // Pressure calculations
        //
        const float c = - ( 315.0f * 3.0f ) / ( 32.0f * M_PI * d9 );
        const float a = d2 - r2;
        const float a2 = a * a;

        const float dWdr = c * a2 * r;

        const float f1_pressure = ( p2->mass * ( p1->pressure + p2->pressure ) * dWdr ) / ( 2 * p2->density );
        const float f2_pressure = ( p1->mass * ( p1->pressure + p2->pressure ) * dWdr ) / ( 2 * p1->density );

        Imath::V2f n = interaction.sep / r;

        p1->force += f1_pressure * n;
        p2->force += f2_pressure * -n;

    }
}


