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

    struct Settings
    {
        Settings(
                float _h,
                float _viscosity,
                float _gravity
                )
         : h( _h ),
           viscosity( _viscosity ),
           gravity( _gravity )
           {}

        float h;
        float viscosity;
        float gravity;
    };

    Simulator(
            Stepper& stepper,
            ForceEvaluator& forceEvaluator,
            ParticleData& particles,
            BoundaryPtrArray& boundaries,
            EmitterPtrArray& emitters,
            Settings& settings,
            std::ostream& logStream
            )
     : m_stepper( stepper ),
       m_forceEvaluator( forceEvaluator ),
       m_particles( particles ),
       m_boundaries( boundaries ),
       m_emitters( emitters ),
       m_settings( settings ),
       m_logStream( logStream ) {}

    virtual ~Simulator() {}

    void step( unsigned int frame, float timeStep );

private:

    Stepper& m_stepper;

    ForceEvaluator& m_forceEvaluator;

    ParticleData& m_particles;
    BoundaryPtrArray& m_boundaries;
    EmitterPtrArray& m_emitters;

    Settings& m_settings;

    std::ostream& m_logStream;
};



#endif // SIMULATOR_HH


