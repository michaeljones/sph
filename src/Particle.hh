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
        FloatArray& _mass,
        const Bounds& _bounds
        )
     : bounds( _bounds ),
       position( _position ),
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
        // Calculate the bounds
        //
        Bounds bounds;
        unsigned int particleCount = pos.size();

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            bounds.extendBy( pos[i] );
        }

        return new ParticleData( pos, velocity, mass, bounds );
    }
};

#endif // PARTICLE_HH

