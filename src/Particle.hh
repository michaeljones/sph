#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "Array.h"

class ParticleData
{
public:

    ParticleData( 
        VectorArray& _position,
        VectorArray& _velocity,
        FloatArray& _mass
        )
     : position( _position ),
       velocity( _velocity ),
       mass( _mass ) {}

public:
    VectorArray& position;
    VectorArray& velocity;
    FloatArray& mass;
};

#endif // PARTICLE_HH

