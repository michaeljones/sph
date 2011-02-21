#ifndef BOUNDARY_HH
#define BOUNDARY_HH

#include "Particle.hh"

#include <ImathVec.h>

class Boundary
{
public:

    Boundary( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    virtual void resolve() = 0;

protected:

    ParticlePtrArray& m_particles;
};


class PlaneBoundary : public Boundary
{
public:

    PlaneBoundary( Imath::V2f& pos, Imath::V2f& normal, ParticlePtrArray& particles )
     : Boundary( particles ),
       m_pos( pos ),
       m_normal( normal) {}

    void resolve()
    {
        unsigned int numParticles = m_particles.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];
            const Imath::V2f diff = p->pos - m_pos;
            const float dotProduct = m_normal.dot( diff );

            if ( dotProduct > 0.0f )
            {
                continue;
            }

            const float velDot = m_normal.dot( p->vel );
            p->vel -= 2.0f * velDot * m_normal;
        }
    }

private:

    const Imath::V2f m_pos;
    const Imath::V2f m_normal;
};


class ContainerBoundary : public Boundary
{
public:

    ContainerBoundary( Imath::V2f& max, Imath::V2f& min, ParticlePtrArray& particles )
     : Boundary( particles ),
       m_max( max ),
       m_min( min) {}
       

    void resolve()
    {
        unsigned int numParticles = m_particles.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];

            // Bottom
            resolveSide( p, m_min, Imath::V2f( 0.0f, 1.0f ) );
            
            // Left
            resolveSide( p, m_min, Imath::V2f( 1.0f, 0.0f ) );

            // Top
            resolveSide( p, m_max, Imath::V2f( 0.0f, -1.0f ) );

            // Right
            resolveSide( p, m_max, Imath::V2f( -1.0f, 0.0f ) );
        }
    }

private:

    void resolveSide( Particle* p, Imath::V2f pos, Imath::V2f normal )
    {
        const Imath::V2f diff = p->pos - pos;
        const float dotProduct = normal.dot( diff );

        if ( dotProduct > 0.0f )
        {
            return;
        }

        const float velDot = normal.dot( p->vel );
        p->vel -= 2.0f * velDot * normal;
    }

    const Imath::V2f m_max;
    const Imath::V2f m_min;
};

typedef std::vector< Boundary* > BoundaryPtrArray;

#endif // BOUNDARY_HH


