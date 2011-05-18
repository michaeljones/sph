#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "Array.hh"
#include <ImathBox.h>

typedef Imath::Box< Imath::V2f > Bounds;

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

    /** Emptys and recalculates the bounds based on the current
        particle positions
        */
    void recalculateBounds()
    {
        bounds.makeEmpty();

        unsigned int particleCount = position.size();

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            bounds.extendBy( position[i] );
        }
    }

public:

    Bounds bounds;

    VectorArray& position;
    VectorArray& velocity;
    FloatArray& mass;

};

class ParticleDataFactory
{
public:

    ParticleDataFactory() {}

    ParticleData* create( VectorArray& pos, VectorArray& velocity, FloatArray& mass ) const
    {
        ParticleData* data = new ParticleData( pos, velocity, mass );

        data->recalculateBounds();

        return data;
    }
};

#endif // PARTICLE_HH

