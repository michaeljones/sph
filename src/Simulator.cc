
#include "Simulator.hh"

#include <math.h>

void Simulator::step()
{
    const float smoothingDistance = 0.5;
    const float smoothingDistanceSquared = smoothingDistance * smoothingDistance;

    unsigned int numParticles = m_particles.size();

    for ( unsigned int i=0; i<numParticles; ++i )
    {
        for ( unsigned int j=0; j<numParticles; ++j )
        {
            if ( i == j )
                continue;

            // TODO: Don't process each pair twice
            Particle* p1 = m_particles[i];
            Particle* p2 = m_particles[j];
            
            Imath::V2f diff = p1->pos - p2->pos;

            float lengthSquared = diff.x * diff.x + diff.y * diff.y;

            if ( lengthSquared > smoothingDistanceSquared )
                continue;

            //  Calculate Kernel contribution for p1 from p2
            //
            //  Use poly6 kernel as noted in the siggraph course notes
            //
            const float d = smoothingDistance;
            const float r2 = lengthSquared;
            const float d2 = d * d;
            const float d9 = d2 * d2 * d2 * d2 * d;
            const float a = d2 - r2;
            const float a3 = a * a * a;
            const float W = ( 315.0f * a3 ) / ( 64.0f * M_PI * d9 );

            // Calculate density for each particle
            //
            p1->density += W * p2->mass;
        }
    }

    for ( unsigned int i=0; i<numParticles; ++i )
    {
    }
}


