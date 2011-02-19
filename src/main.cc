
#include "Particle.hh"
#include "Simulator.hh"
#include "Display.hh"

#include <stdlib.h>
#include <algorithm>

class deleter
{
public:
    deleter() {}

    void operator()( Particle* particle )
    {
        delete particle;
    }
};

int main( int argc, char *argv[] )
{
    std::cout << "Initialising Llyr" << std::endl;

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

    Simulator sim( particles );
    Display display( particles );

    for ( unsigned int i=0; i<10; ++i )
    {
        std::cout << "Step " << i << std::endl;
        sim.step();
        display.draw();
    }

    std::for_each( particles.begin(), particles.end(), deleter() );

    std::cout << "Finished." << std::endl;
}

