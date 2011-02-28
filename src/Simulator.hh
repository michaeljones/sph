#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include "Particle.hh"
#include "Boundary.hh"
#include "Emitter.hh"

#include <ImathVec.h>

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
            ParticlePtrArray& particles,
            BoundaryPtrArray& boundaries,
            EmitterPtrArray& emitters,
            Settings& settings,
            std::ostream& logStream
            )
     : m_particles( particles ),
       m_boundaries( boundaries ),
       m_emitters( emitters ),
       m_settings( settings ),
       m_logStream( logStream ) {}

    void step();

private:

    ParticlePtrArray& m_particles;
    BoundaryPtrArray& m_boundaries;
    EmitterPtrArray& m_emitters;

    Settings& m_settings;

    std::ostream& m_logStream;
};



#endif // SIMULATOR_HH


