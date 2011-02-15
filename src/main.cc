
#include "Particle.hh"
#include "Simulator.hh"
#include "Display.hh"

#include <stdlib.h>

int main( int argc, char *argv[] )
{
    std::cerr << "Initialising Llyr" << std::endl;

    srand48( 0 );

    const unsigned particleCount = 1000;
    ParticlePtrArray particles( particleCount );

    // Fill particle array
    for ( unsigned int i=0; i<particleCount; ++i )
    {
        float x = drand48() * 10.0f;
        float y = drand48() * 10.0f;
        particles[i] = new Particle( Imath::V2f( x, y ) );
    }

    std::cerr << "mpj-debug: " << particles.size() << std::endl;

    Simulator sim( particles );
    sim.step();

    Display display( particles );
    display.draw();
}

