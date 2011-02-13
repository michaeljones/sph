#ifndef SIMULATOR_HH
#define SIMULATOR_HH

#include <ImathVec.h>

class Simulator
{
public:

    Simulator( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    void step() {}

private:

    ParticlePtrArray& m_particles;
};



#endif // SIMULATOR_HH


