#ifndef EMITTER_HH
#define EMITTER_HH

#include <ImathVec.h>

#include <vector>
#include <stdlib.h>

class Emitter
{
public:

    Emitter( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    virtual void emit() const = 0;

protected:

    ParticlePtrArray& m_particles;

};

class PointEmitter : public Emitter
{
public:

    PointEmitter( Imath::V2f pos, ParticlePtrArray& particles )
     : Emitter( particles ),
       m_counter( 0 ),
       m_pos( pos ) {}

    void emit() const
    {
        if ( ! ( m_counter % 500 ) )
        {
            float x = 0;
            float y = 2.4;

            Particle* p = new Particle( Imath::V2f( m_pos.x + x, m_pos.y + y ) );
            p->vel = Imath::V2f( 2 * ( drand48() - 0.5 ), 2 * ( drand48() - 0.5 ) );
            m_particles.push_back( p );
        }
        ++m_counter;
    }

private:

    mutable unsigned m_counter;

    Imath::V2f m_pos;
};

typedef std::vector< Emitter* > EmitterPtrArray;

#endif // PARTICLE_HH

