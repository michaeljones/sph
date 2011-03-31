
// Llyr Library includes
#include <Particle.hh>
#include <Simulator.hh>
#include <Boundary.hh>
#include <Emitter.hh>
#include <Stepper.hh>
#include <ForceEvaluator.hh>

// Helper includes
#include "Display.hh"
#include "Validator.hh"

#include <stdlib.h>
#include <algorithm>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>

#define ESCAPE 27
#define KEY_Q 938

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


/* The number of our GLUT window */
int window; 

Simulator* sim = NULL;
Display* display = NULL;
Validator* validator = NULL;

const float timeStep = 1.0f / 4800.0f;
unsigned int frame = 0;

void InitGL(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        // This Will Clear The Background Color To Black
    glClearDepth(1.0);                // Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
    glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                // Reset The Projection Matrix
    
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);    // Calculate The Aspect Ratio Of The Window
    
    glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
    if (Height==0)                // Prevent A Divide By Zero If The Window Is Too Small
        Height=1;

    glViewport(0, 0, Width, Height);        // Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear The Screen And The Depth Buffer
    glLoadIdentity();                // Reset The View

    // Step the simulation forward
    sim->step( frame++, timeStep );

    if ( ! validator->valid() )
    {
        /* shut down our window */
        glutDestroyWindow(window); 

        /* exit the program...normal termination. */
        exit(0);
    }

    // Draw the result
    display->draw();

    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this procedure */
    usleep(10);
    
    /* If escape is pressed, kill everything. */
    if ( key == ESCAPE || key == KEY_Q ) 
    { 
        /* shut down our window */
        glutDestroyWindow(window);
        
        /* exit the program...normal termination. */
        exit(0);                                     
    }
}

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

struct InputData
{
    Region container;
    RegionGroup particleRegions;
    RegionGroup boxBoundaries;
    float zDepth;
    float h;
    float viscosity;
    float gravity;
    const char* logfile;
};


extern "C" {


bool run( InputData inputData )
{    
    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
         X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */    
    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);    

    /* Select type of Display mode:     
         Double buffer 
         RGBA color
         Alpha components supported 
         Depth buffer */    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);    

    /* get a 640 x 480 window */
    glutInitWindowSize(640, 480);    

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);    

    /* Open a window */    
    window = glutCreateWindow("Llyr SPH");    

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);    

    /* Go fullscreen.    This is as soon as possible. */
    glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);

    /* Initialize our window. */
    InitGL(640, 480);

    glPointSize( 2.0f );

    std::cout << "Initialising Llyr" << std::endl;

    srand48( 1 );

    //  Particles
    //
    std::auto_ptr< VectorArray > position( new VectorArray );
    std::auto_ptr< VectorArray > velocity( new VectorArray );
    std::auto_ptr< FloatArray > mass( new FloatArray );
    ParticleData particles( *position, *velocity, *mass );

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
                particles.position.push_back( Imath::V2f( px, py ) );
                particles.velocity.push_back( Imath::V2f( 0.0f, 0.0f ) );
                particles.mass.push_back( 1.0f );
                x += h * scale;
            }

            y += h * sin( M_PI / 3.0 ) * scale;
            row += 1;
        }
    }

    //  Displays
    //
    std::vector< Display* > displays;

    //  Boundaries
    //
    BoundaryPtrArray boundaries;
    Imath::V2f min( inputData.container.min.x, inputData.container.min.y );
    Imath::V2f max( inputData.container.max.x, inputData.container.max.y );
    boundaries.push_back( new ContainerBoundary( max, min, particles ) );
    displays.push_back( new BoxDisplay( max, min, inputData.zDepth ) );

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

        boundaries.push_back( new BoxBoundary( maxb, minb, particles ) );
        displays.push_back( new BoxDisplay( maxb, minb, inputData.zDepth ) );
    }

    //  Emitters
    //  
    EmitterPtrArray emitters;

    // emitters.push_back( new PointEmitter( Imath::V2f( 0.0f, 0.0f ), particles ) );

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

    validator = new NanValidator( particles );
    displays.push_back( new ParticleDisplay( particles, inputData.zDepth, inputData.h ) );

    display = new MultiDisplay( displays );

    Stepper stepper;
    ForceEvaluator forceEvaluator( 
            inputData.h,
            inputData.viscosity,
            inputData.gravity
            );
    sim = new Simulator( stepper, forceEvaluator, particles, boundaries, emitters, *logStream );

    /* Start Event Processing Engine */    
    glutMainLoop();    

    delete validator;
    delete sim;
    delete display;

    std::for_each( boundaries.begin(), boundaries.end(), deleter<Boundary>() );

    std::cout << "Finished." << std::endl;

    return true;
}

}


