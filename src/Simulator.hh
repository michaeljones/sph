#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include "Particle.hh"
#include "Boundary.hh"
#include "Emitter.hh"

#include <ImathVec.h>

class Stepper;
class ForceEvaluator;

class Simulator
{
public:

    /** Constructor
        \param stepper
        \param forceEvaluator
        \param particles
        \param boundaries
        \param emitters
        \param logStream
        */
    Simulator(
            Stepper& stepper,
            ForceEvaluator& forceEvaluator,
            ParticleData& particles,
            BoundaryPtrArray& boundaries,
            EmitterPtrArray& emitters,
            std::ostream& logStream
            )
     : m_stepper( stepper ),
       m_forceEvaluator( forceEvaluator ),
       m_particles( particles ),
       m_boundaries( boundaries ),
       m_emitters( emitters ),
       m_logStream( logStream ) {}

    virtual ~Simulator() {}

    /** Moves the simulation one step forward with the time step provided
        \param frame Frame number for this step of the simulation
        \param timeStep Length of the simulation step
        \todo Time units?
        */
    void step( unsigned int frame, float timeStep );

private:

    Stepper& m_stepper;

    ForceEvaluator& m_forceEvaluator;

    ParticleData& m_particles;
    BoundaryPtrArray& m_boundaries;
    EmitterPtrArray& m_emitters;

    std::ostream& m_logStream;
};



#endif // SIMULATOR_HH


