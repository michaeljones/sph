
#include "Particle.hh"
#include "Simulator.hh"

int main( int argc, char *argv[] )
{
    std::cerr << "Initialising Llyr" << std::endl;

    ParticlePtrArray particles;

    // Fill particle array
    for ( unsigned int i=0; i<1000; ++i )
    {

        
    }

    Simulator sim( particles );
    sim.step();
}

