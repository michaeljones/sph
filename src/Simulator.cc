
#include "Simulator.hh"

void Simulator::step()
{
    const float smoothingDistance = 0.5;
    const float smoothingDistanceSquared = smoothingDistance * smoothingDistance;

    unsigned int numParticles = m_particles.size();

    for ( unsigned int i=0; i<numParticles; ++i )
    {
        for ( unsigned int j=0; j<numParticles; ++j )
        {
            // TODO: Don't process each pair twice
            Particle* p1 = m_particles[i];
            Particle* p2 = m_particles[j];
            
            Imath::V2f diff = p1->pos() - p2->pos();

            float lengthSquared = diff.x * diff.x + diff.y * diff.y;

            if ( lengthSquared > smoothingDistanceSquared )
                continue;

            std::cerr << "mpj-debug: Pair " << i << " " << j << std::endl;
            
        }
    }
}


