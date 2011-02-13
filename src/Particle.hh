#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <ImathVec.h>

#include <vector>

class Particle
{
public:

    Particle() {}

public:

    Imath::V2f pos;
    Imath::V2f vel;
    float mass;
};


typedef std::vector< Particle* > ParticlePtrArray;

#endif // PARTICLE_HH

