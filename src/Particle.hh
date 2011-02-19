#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <ImathVec.h>

#include <vector>

class Particle
{
public:

    Particle( const Imath::V2f& pos )
     : mass( 1.0f ), 
       density( 0.0f ),
       pressure( 0.0f ),
       pos( pos ),
       vel( 0.0f ),
       force( 0.0f ) {}

public:

    float mass;
    float density;
    float pressure;

    Imath::V2f pos;
    Imath::V2f vel;
    Imath::V2f force;
};


typedef std::vector< Particle* > ParticlePtrArray;

#endif // PARTICLE_HH

