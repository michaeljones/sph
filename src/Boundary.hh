#ifndef BOUNDARY_HH
#define BOUNDARY_HH

#include "Particle.hh"

#include <ImathVec.h>

class Boundary
{
public:

    Boundary( ParticleData& particles )
     : m_particles( particles ) {}

    virtual void resolve() = 0;

protected:

    ParticleData& m_particles;
};


class PlaneBoundary : public Boundary
{
public:

    PlaneBoundary( Imath::V2f& pos, Imath::V2f& normal, ParticleData& particles )
     : Boundary( particles ),
       m_pos( pos ),
       m_normal( normal) {}

    void resolve()
    {
        unsigned int numParticles = m_particles.position.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            VectorArray& pos = m_particles.position;
            VectorArray& vel = m_particles.velocity;
            const Imath::V2f diff = pos[i] - m_pos;
            const float dotProduct = m_normal.dot( diff );

            if ( dotProduct > 0.0f )
            {
                continue;
            }

            // Correct position
            pos[i] -= m_normal * dotProduct;

            const float velDot = m_normal.dot( vel[i] );
            vel[i] -= 2.0f * velDot * m_normal;
        }
    }

private:

    const Imath::V2f m_pos;
    const Imath::V2f m_normal;
};


class ContainerBoundary : public Boundary
{
public:

    ContainerBoundary( Imath::V2f& max, Imath::V2f& min, ParticleData& particles )
     : Boundary( particles ),
       m_max( max ),
       m_min( min) {}
       

    void resolve()
    {
        unsigned int numParticles = m_particles.position.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            VectorArray& pos = m_particles.position;
            VectorArray& vel = m_particles.velocity;

            // Bottom
            resolveSide( pos[i], vel[i], m_min, Imath::V2f( 0.0f, 1.0f ) );
            
            // Left
            resolveSide( pos[i], vel[i], m_min, Imath::V2f( 1.0f, 0.0f ) );

            // Top
            resolveSide( pos[i], vel[i], m_max, Imath::V2f( 0.0f, -1.0f ) );

            // Right
            resolveSide( pos[i], vel[i], m_max, Imath::V2f( -1.0f, 0.0f ) );
        }
    }

private:

    void resolveSide( Imath::V2f& pos, Imath::V2f& vel, Imath::V2f boundaryPos, Imath::V2f normal )
    {
        const Imath::V2f diff = pos - boundaryPos;
        const float dotProduct = normal.dot( diff );

        if ( dotProduct > 0.0f )
        {
            return;
        }

        // Correct position
        pos -= normal * dotProduct;

        // Reflect velocity
        const float velDot = normal.dot( vel );
        vel -= 2.0f * velDot * normal;
    }

    const Imath::V2f m_max;
    const Imath::V2f m_min;
};

typedef std::vector< Boundary* > BoundaryPtrArray;

#endif // BOUNDARY_HH


