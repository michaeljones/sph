#include "Simulator.hh"

#include "ForceEvaluator.hh"
#include "Stepper.hh"

#include <math.h>
#include <memory>

void Simulator::step( unsigned int frame, float timeStep )
{
    m_logStream << "Frame " << frame << " --------------------------------------" << std::endl;

    const unsigned int numEmitters = m_emitters.size();
    for ( unsigned int i=0; i<numEmitters; ++i )
    {
        m_emitters[i]->emit();
    }

    const unsigned int numBoundaries = m_boundaries.size();
    for ( unsigned int i=0; i<numBoundaries; ++i )
    {
        m_boundaries[i]->resolve();
    }

    unsigned int particleCount = m_particles.position.size();

    //
    //  Evaluate forces at start of time step
    //
    std::auto_ptr< FloatArray > initialDensity( new FloatArray( particleCount ) );
    std::auto_ptr< FloatArray > initialPressure( new FloatArray( particleCount )  );

    ForceEvaluator::EvaluationData initialEvalData(
            m_particles.position,
            m_particles.velocity,
            m_particles.mass,
            *initialDensity,
            *initialPressure
        );

    std::auto_ptr< VectorArray > initialForces( new VectorArray( particleCount ) );
    m_forceEvaluator.evaluate( initialEvalData, *initialForces );

    //
    //  Step from start to midpoint
    //
    Stepper::FromData fromData(
            m_particles.position,
            m_particles.velocity,
            m_particles.mass,
            *initialForces
            );

    std::auto_ptr< VectorArray > midPointPosition( new VectorArray( particleCount ) );
    std::auto_ptr< VectorArray > midPointVelocity( new VectorArray( particleCount ) );
    Stepper::ToData toData( *midPointPosition, *midPointVelocity );

    m_stepper.step( fromData, toData, timeStep * 0.5 );

    //
    //  Evaluate forces at the midpoint
    //
    std::auto_ptr< FloatArray > midPointDensity( new FloatArray( particleCount ) );
    std::auto_ptr< FloatArray > midPointPressure( new FloatArray( particleCount)  );

    ForceEvaluator::EvaluationData midPointEvalData(
            *midPointPosition,
            *midPointVelocity,
            m_particles.mass,
            *midPointDensity,
            *midPointPressure
        );

    std::auto_ptr< VectorArray > midPointForces( new VectorArray( particleCount ) );
    m_forceEvaluator.evaluate( midPointEvalData, *midPointForces );

    //
    //  Step from start to end
    //
    Stepper::FromData midPointFromData(
            m_particles.position,
            m_particles.velocity,
            m_particles.mass,
            *midPointForces
            );

    Stepper::ToData midPointToData(
            m_particles.position,
            m_particles.velocity
            );

    m_stepper.step( midPointFromData, midPointToData, timeStep );
}


