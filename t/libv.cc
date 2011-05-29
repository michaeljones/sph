
// Llyr Library includes
#include <Particle.hh>
#include <Simulator.hh>
#include <Boundary.hh>
#include <Emitter.hh>
#include <Stepper.hh>
#include <ForceEvaluator.hh>
#include <Output.hh>
#include <Reader.hh>

// Helper includes
#include "Display.hh"
#include "Validator.hh"

#include <stdlib.h>
#include <algorithm>

#include <GL/freeglut_ext.h>
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

Display* display = NULL;
Reader* reader = NULL;
ParticleData* particles = NULL;

int frame = 0;
int endFrame = 0;

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

    // Wait for one frame: 1.0 / 24.0 * 1e6
    usleep(41666);

    frame++;

    if ( frame > endFrame )
    {
        glutLeaveMainLoop();
        return;
    }

    reader->read( frame, particles->position );

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


struct FrameRange
{
    int start;
    int end;
    int substeps;
};

struct ViewData
{
    FrameRange frameRange;
    char* filename;
    float zDepth;
};

extern "C" {

bool view( ViewData viewData )
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

    std::cout << "Initialising Viewer" << std::endl;

    srand48( 1 );

    //  Particles
    //
    std::auto_ptr< VectorArray > position( new VectorArray );
    std::auto_ptr< VectorArray > velocity( new VectorArray );
    std::auto_ptr< FloatArray > mass( new FloatArray );

    reader = new LyReader( viewData.filename );

    endFrame = viewData.frameRange.end;

    ParticleDataFactory particleDataFactory;
    particles = particleDataFactory.create( *position, *velocity, *mass );

    //  Displays
    //
    std::vector< Display* > displays;
    displays.push_back( new ParticleDisplay( *particles, viewData.zDepth ) );

    display = new MultiDisplay( displays );

    /* Start Event Processing Engine */    
    glutMainLoop();    

    /* shut down our window */
    glutDestroyWindow(window); 

    std::cout << "Finished." << std::endl;

    return true;
}

}


