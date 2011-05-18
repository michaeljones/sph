
// Llyr Library includes
#include <Particle.hh>
#include <Simulator.hh>
#include <Boundary.hh>
#include <Emitter.hh>
#include <Stepper.hh>
#include <ForceEvaluator.hh>
#include <Output.hh>
#include <Grid.hh>

// Helper includes
#include "Validator.hh"

#include <stdlib.h>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>


template < typename T >
class deleter
{
public:
    deleter() {}

    void operator()( T* ptr )
    {
        delete ptr;
    }
};

struct Point
{
    float x;
    float y;
};

struct Region
{
    Point min;
    Point max;
};

struct RegionGroup
{
    int numRegions;
    Region* regions;
};

struct FrameRange
{
    int start;
    int end;
    int substeps;
};

struct SimData
{
    FrameRange frameRange;
    char* filename;
    Region container;
    RegionGroup particleRegions;
    RegionGroup boxBoundaries;
    float h;
    float viscosity;
    float gravity;
    const char* logfile;
};


extern "C" {


bool run( SimData inputData )
{    
    std::cout << "Initialising Llyr" << std::endl;

    std::cout << "Frame Range:        " << inputData.frameRange.start 
              << " - " << inputData.frameRange.end 
              << " (" << inputData.frameRange.substeps 
              << " substeps)" << std::endl;
    std::cout << "Filename:           " << inputData.filename << std::endl;
    std::cout << "Container:          " 
        << inputData.container.min.x << " " 
        << inputData.container.min.y << " " 
        << inputData.container.max.x << " " 
        << inputData.container.max.y << " " << std::endl;

    for ( int i=0; i < inputData.particleRegions.numRegions; ++i )
    {
        std::cout << "Particle Region " << i << ":  " 
            << inputData.particleRegions.regions[i].min.x << " " 
            << inputData.particleRegions.regions[i].min.y << " " 
            << inputData.particleRegions.regions[i].max.x << " " 
            << inputData.particleRegions.regions[i].max.y << " " << std::endl;
    }

    for ( int i=0; i < inputData.boxBoundaries.numRegions; ++i )
    {
        std::cout << "Box Boundary " << i << ":     " 
            << inputData.boxBoundaries.regions[i].min.x << " " 
            << inputData.boxBoundaries.regions[i].min.y << " " 
            << inputData.boxBoundaries.regions[i].max.x << " " 
            << inputData.boxBoundaries.regions[i].max.y << " " << std::endl;
    }

    std::cout << "Smoothing distance: " << inputData.h << std::endl;
    std::cout << "Viscosity:          " << inputData.viscosity << std::endl;
    std::cout << "Gravity:            " << inputData.gravity << std::endl;
    std::cout << "Logfile:            " << inputData.logfile << std::endl;

    srand48( 1 );

    //  Particles
    //
    std::auto_ptr< VectorArray > position( new VectorArray );
    std::auto_ptr< VectorArray > velocity( new VectorArray );
    std::auto_ptr< FloatArray > mass( new FloatArray );

    // Fill particle array
    for ( int i=0; i < inputData.particleRegions.numRegions; ++i )
    {
        float x = inputData.particleRegions.regions[i].min.x, y = inputData.particleRegions.regions[i].min.y;
        float h = inputData.h;
        unsigned int row = 0;
        float jitter = 0.1;
        float scale = 1.0f + 2 * jitter;

        while ( y < inputData.particleRegions.regions[i].max.y )
        {
            x = row % 2
                ? inputData.particleRegions.regions[i].min.x
                : inputData.particleRegions.regions[i].min.x + ( h * 0.5 * scale );

            while ( x < inputData.particleRegions.regions[i].max.x )
            {
                float xsign = drand48() > 0.5 ? 0.5f : -0.5f;
                float ysign = drand48() > 0.5 ? 0.5f : -0.5f;
                float px = x + ( jitter * ( xsign * h ) );
                float py = y + ( jitter * ( ysign * h ) );
                position->push_back( Imath::V2f( px, py ) );
                velocity->push_back( Imath::V2f( 0.0f, 0.0f ) );
                mass->push_back( 1.0f );
                x += h * scale;
            }

            y += h * sin( M_PI / 3.0 ) * scale;
            row += 1;
        }
    }

    //  Create particle data object
    //
    ParticleDataFactory particleDataFactory;
    std::auto_ptr< ParticleData > particles( particleDataFactory.create( *position, *velocity, *mass ) );

    //  Boundaries
    //
    BoundaryPtrArray boundaries;
    Imath::V2f min( inputData.container.min.x, inputData.container.min.y );
    Imath::V2f max( inputData.container.max.x, inputData.container.max.y );
    boundaries.push_back( new ContainerBoundary( max, min, *particles ) );

    for ( int i=0; i < inputData.boxBoundaries.numRegions; ++i )
    {
        Imath::V2f minb(
                inputData.boxBoundaries.regions[i].min.x,
                inputData.boxBoundaries.regions[i].min.y
                );
        Imath::V2f maxb(
                inputData.boxBoundaries.regions[i].max.x,
                inputData.boxBoundaries.regions[i].max.y
                );

        boundaries.push_back( new BoxBoundary( maxb, minb, *particles ) );
    }

    //  Emitters
    //  
    EmitterPtrArray emitters;

    // emitters.push_back( new PointEmitter( Imath::V2f( 0.0f, 0.0f ), *particles ) );

    //  Log file
    //
    std::ostream* logStream = NULL;
    std::ofstream fileStream( inputData.logfile );
    logStream = &fileStream;

    if ( ! fileStream.is_open() )
    {
        logStream = &std::cout;
        *logStream << "Failed to open file: " << inputData.logfile << std::endl;
    }

    NanValidator validator( *particles );
    LyOutput output( *particles, inputData.filename );

    Stepper stepper;
    GridFactory gridFactory( inputData.h );
    ForceEvaluator forceEvaluator( 
            inputData.h,
            inputData.viscosity,
            inputData.gravity,
            gridFactory
            );
    Simulator sim( stepper, forceEvaluator, *particles, boundaries, emitters, *logStream );

    const float timeStep = 1.0f / ( 24.0f * inputData.frameRange.substeps );

    for ( int frame=inputData.frameRange.start; frame<=inputData.frameRange.end; ++frame )
    {
        std::cout << "Frame " << frame << std::endl;
        for ( int substep=0; substep<inputData.frameRange.substeps; ++substep )
        {
            sim.step( frame, timeStep );
        }
        output.write( frame );
    }

    std::for_each( boundaries.begin(), boundaries.end(), deleter<Boundary>() );

    std::cout << "Finished." << std::endl;

    return true;
}

}

