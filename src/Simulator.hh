#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include "Particle.hh"
#include "Boundary.hh"

#include <ImathVec.h>

class Simulator
{
public:

    Simulator(
            ParticlePtrArray& particles,
            BoundaryPtrArray& boundaries,
            std::ostream& logStream
            )
     : m_particles( particles ),
       m_boundaries( boundaries ),
       m_logStream( logStream ) {}

    void step();

private:

    ParticlePtrArray& m_particles;
    BoundaryPtrArray& m_boundaries;

    std::ostream& m_logStream;
};



#endif // SIMULATOR_HH


