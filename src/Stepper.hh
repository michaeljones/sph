#ifndef STEPPER_HH
#define STEPPER_HH

#include <ImathVec.h>

#include <vector>

class Stepper 
{
public:

    struct FromData {
        FromData(
                VectorArray& _p, 
                VectorArray& _v, 
                FloatArray& _m, 
                VectorArray& _f
                )
         : position( _p ), velocity( _v ), mass( _m ), force( _f ) {}

        VectorArray& position;
        VectorArray& velocity;
        FloatArray& mass;
        VectorArray& force;
    };

    struct ToData {
        ToData( VectorArray& _p, VectorArray& _v )
         : position( _p ), velocity( _v ) {}

        VectorArray& position;
        VectorArray& velocity;
    };

    Stepper() {}

    void step( FromData& fromData, ToData& toData, float timeStep )
    {
        unsigned int particleCount = fromData.velocity.size();

        VectorArray& toVelocity = toData.velocity;
        VectorArray& fromVelocity = fromData.velocity;
        VectorArray& force = fromData.force;
        FloatArray& mass = fromData.mass;

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            toVelocity[ i ] = fromVelocity[ i ] + timeStep * force[ i ] / mass[ i ];
        }

        VectorArray& toPosition = toData.position;
        VectorArray& fromPosition = fromData.position;

        for ( unsigned int i=0; i<particleCount; ++i )
        {
            toPosition[ i ] = fromPosition[ i ] + timeStep * toVelocity[ i ];
        }
    }
};


#endif // STEPPER_HH

