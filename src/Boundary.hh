#ifndef BOUNDARY_HH
#define BOUNDARY_HH

#include "Particle.hh"

#include <ImathVec.h>

class Boundary
{
public:

    Boundary( ParticleData& particles )
     : m_particles( particles ) {}

    virtual void resolve( float timeStep ) = 0;

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

    void resolve( float timeStep )
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
       

    void resolve( float timeStep )
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


class BoxBoundary : public Boundary
{
public:

    BoxBoundary( Imath::V2f& max, Imath::V2f& min, ParticleData& particles )
     : Boundary( particles ),
       m_max( max ),
       m_min( min ) {}
       

    void resolve( float timeStep )
    {
        unsigned int numParticles = m_particles.position.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Imath::V2f& pos = m_particles.position[i];
            Imath::V2f& vel = m_particles.velocity[i];

            if ( pos.x < m_min.x || pos.y < m_min.y || pos.x > m_max.x || pos.y > m_max.y )
            {
                continue;
            }

            Imath::V2f a( m_min );
            Imath::V2f b( m_min.x, m_max.y );
            Imath::V2f c( m_max );
            Imath::V2f d( m_max.x, m_min.y );

            Imath::V2f abNormal( -1.0f,  0.0 );
            Imath::V2f bcNormal(  0.0f,  1.0 );
            Imath::V2f adNormal(  0.0f, -1.0 );
            Imath::V2f dcNormal(  1.0f,  0.0 );

            compare( pos, vel, a, b, abNormal, timeStep, "a" )
            || compare( pos, vel, b, c, bcNormal, timeStep, "b" )
            || compare( pos, vel, a, d, adNormal, timeStep, "c" )
            || compare( pos, vel, d, c, dcNormal, timeStep, "d" );
        }
    }

private:

    bool compare(
            Imath::V2f& pos,
            Imath::V2f& vel,
            Imath::V2f min,
            Imath::V2f max,
            Imath::V2f normal,
            float timeStep,
            const char* name
            )
    {
        const Imath::V2f diff = pos - min;
        const float dotProduct = normal.dot( diff );

        if ( dotProduct > 0.0f )
        {
            return false;
        }

        Imath::V2f tVel = vel * timeStep;
        Imath::V2f ppos = pos - tVel;

        const Imath::V2f prevDiff = ( ppos - min );
        const float prevDotProduct = normal.dot( prevDiff );

        if ( prevDotProduct < 0.0f )
        {
            return false;
        }

        const float c = ( tVel.y * ( min.x - pos.x ) + tVel.x * ( pos.y - min.y ) )
            / ( tVel.x * ( max.y - min.y ) - tVel.y * ( max.x - min.x ) ); 

        if ( c > 1.0f || c < 0.0f )
        {
            return false;
        }

        // Correct position
        pos -= normal * dotProduct;

        // Reflect velocity
        const float velDot = normal.dot( vel );
        vel -= 2.0f * velDot * normal;

        return true;
    }

    const Imath::V2f m_max;
    const Imath::V2f m_min;
};

typedef std::vector< Boundary* > BoundaryPtrArray;

#endif // BOUNDARY_HH


