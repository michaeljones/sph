#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <ImathVec.h>

#include <vector>

class Particle
{
public:

    Particle( const Imath::V2f& pos )
     : m_pos( pos ), m_vel( 0.0f ), m_mass( 1.0f )  {}

    const Imath::V2f& pos() const { return m_pos; }
    const Imath::V2f& vel() const { return m_vel; }
    float mass() const { return m_mass; }

private:

    Imath::V2f m_pos;
    Imath::V2f m_vel;
    float m_mass;
};


typedef std::vector< Particle* > ParticlePtrArray;

#endif // PARTICLE_HH

