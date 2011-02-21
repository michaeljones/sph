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
            BoundaryPtrArray& boundaries
            )
     : m_particles( particles ),
       m_boundaries( boundaries ) {}

    void step();

private:

    ParticlePtrArray& m_particles;
    BoundaryPtrArray& m_boundaries;
};



#endif // SIMULATOR_HH


