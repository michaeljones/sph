
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
    const unsigned int numBoundaries = m_boundaries.size();
    for ( unsigned int i=0; i<numBoundaries; ++i )
    {
        m_boundaries[i]->resolve();
    }

    const float smoothingDistance = 0.055;
    const float smoothingDistanceSquared = smoothingDistance * smoothingDistance;

    const unsigned int numParticles = m_particles.size();

    std::vector< Interaction > interactions;

    const float d = smoothingDistance;
    const float d2 = d * d;
    const float d5 = d2 * d2 * d;
    const float d6 = d5 * d;
    const float d9 = d6 * d2 * d;

    const Imath::V2f zero( 0.0f );

    for ( unsigned int i=0; i<numParticles; ++i )
    {
        Particle* p1 = m_particles[i];

        // Reset force on the particle
        p1->force = zero;

        // std::cerr << "mpj-debug: p " << p1->pos << std::endl;
        // std::cerr << "mpj-debug: v " << p1->vel << std::endl;

        for ( unsigned int j=i+1; j<numParticles; ++j )
        {
            //  We don't skip the i==j case as the poly6 is not singular
            //
            Particle* p2 = m_particles[j];
            
            Imath::V2f diff = p1->pos - p2->pos;

            const float lengthSquared = diff.x * diff.x + diff.y * diff.y;

            /*
            if ( i == 0 )
                m_logStream << i << " " << j << " " << lengthSquared << " p " << p1->pos << " p2 " << p2->pos << std::endl;
            */

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

            //  Calculate density for each particle
            //
            p1->density += W * p2->mass;
            p2->density += W * p1->mass;
        }

        // Calculate pressure based on density
        const float k = 0.5f;
        const float averageDensity = 1.0f;
        p1->pressure = k * ( p1->density - averageDensity );

        // Apply external forces
        const Imath::V2f down( 0.0, -1.0 );
        p1->force += down * p1->mass * 9.81f;

        /*
        if ( i == 0 )
            m_logStream << i << " grav " << p1->force << std::endl;
        */
    }

    unsigned int numInteractions = interactions.size();
    
    for ( unsigned int i=0; i<numInteractions; ++i )
    {
        Interaction& interaction = interactions[i];
        
        Particle* p1 = m_particles[ interaction.i ];
        Particle* p2 = m_particles[ interaction.j ];

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
            const float c = - ( 45.0f / ( M_PI * d6 ) );
            const float a = ( ( d2 + r2 ) / r ) - 2 * d;

            const Imath::V2f dWdr = c * a * interaction.sep;

            const Imath::V2f f1_pressure = ( p2->mass * ( p1->pressure + p2->pressure ) * dWdr ) / ( 2 * p2->density );
            const Imath::V2f f2_pressure = ( p1->mass * ( p1->pressure + p2->pressure ) * dWdr ) / ( 2 * p1->density );

            if ( interaction.i == 0 )
                m_logStream << interaction.i << " mass " << p2->mass << " pressure1 " << p1->pressure << " pressure2 " << p2->pressure << " dWdr " << dWdr << " p2density " << p2->density << std::endl;

            p1->force += - f1_pressure;
            p2->force += f2_pressure;

            if ( interaction.i == 0 )
                m_logStream << interaction.i << " press " << f1_pressure << " r " << r << " i.s " << interaction.sep << " j " << interaction.j << std::endl;

        }

        //  Viscosity Calculations
        //
        //  Using viscosity kernel as recommended in literature
        //
        const float c = 90.0f / ( 2 * M_PI * d5 );
        const float d2Wdr2 = c * ( 1 - r / d );

        // viscosity of water
        const float mu = 8.94e-4f;

        const Imath::V2f f1_viscosity = mu * p2->mass * ( p2->vel - p1->vel ) * d2Wdr2 / ( p2->density );
        const Imath::V2f f2_viscosity = mu * p1->mass * ( p1->vel - p2->vel ) * d2Wdr2 / ( p2->density );

        p1->force += f1_viscosity;
        p2->force += f2_viscosity;

        if ( interaction.i == 0 )
            m_logStream << interaction.i << " vis " << f1_viscosity << std::endl;
    }

    for ( unsigned int i=0; i<numParticles; ++i )
    {
        const float dt = 1.0f / 24000.0f;

        Particle* p = m_particles[ i ];

        p->vel += ( p->force / p->mass ) * dt;
        p->pos += p->vel * dt;

        m_logStream << i << " p " << p->pos << " v " << p->vel << " f " << p->force << " d " << p->density << " pr " << p->pressure << std::endl;
    }
}

