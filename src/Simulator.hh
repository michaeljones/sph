#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include "Particle.hh"
#include "Boundary.hh"

#include <ImathVec.h>

class Simulator
{
public:

    struct Settings
    {
        Settings(
                float _h,
                float _viscosity
                )
         : h( _h ),
           viscosity( _viscosity )
           {}

        float h;
        float viscosity;
    };

    Simulator(
            ParticlePtrArray& particles,
            BoundaryPtrArray& boundaries,
            Settings& settings,
            std::ostream& logStream
            )
     : m_particles( particles ),
       m_boundaries( boundaries ),
       m_settings( settings ),
       m_logStream( logStream ) {}

    void step();

private:

    ParticlePtrArray& m_particles;
    BoundaryPtrArray& m_boundaries;

    Settings& m_settings;

    std::ostream& m_logStream;
};



#endif // SIMULATOR_HH


