
#include "Particle.hh"
#include "Simulator.hh"
#include "Display.hh"

#include <stdlib.h>
#include <algorithm>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>

#include <iostream>

#include <cmath>

#define ESCAPE 27
#define KEY_Q 938

class deleter
{
public:
    deleter() {}

    void operator()( Particle* particle )
    {
        delete particle;
    }
};


/* The number of our GLUT window */
int window; 

Simulator* sim = NULL;
Display* display = NULL;

void InitGL(int Width, int Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
	
	glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
	if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View

    // Step the simulation forward
    sim->step();

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

int main(int argc, char **argv) 
{	
	/* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
		 X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */	
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

	/* Go fullscreen.	This is as soon as possible. */
	glutFullScreen();

	/* Even if there are no events, redraw our gl scene. */
	glutIdleFunc(&DrawGLScene);

	/* Register the function called when our window is resized. */
	glutReshapeFunc(&ReSizeGLScene);

	/* Register the function called when the keyboard is pressed. */
	glutKeyboardFunc(&keyPressed);

	/* Initialize our window. */
	InitGL(640, 480);


    std::cout << "Initialising Llyr" << std::endl;

    srand48( 0 );

    const unsigned particleCount = 1000;
    ParticlePtrArray particles( particleCount );

    // Fill particle array
    for ( unsigned int i=0; i<particleCount; ++i )
    {
        float x = ( drand48() - 0.5 ) * 10.0f;
        float y = ( drand48() - 0.5 ) * 10.0f;
        particles[i] = new Particle( Imath::V2f( x, y ) );
    }

    sim = new Simulator( particles );
    display = new Display( particles );
	
	/* Start Event Processing Engine */	
	glutMainLoop();	

    delete sim;
    delete display;

    std::for_each( particles.begin(), particles.end(), deleter() );

    std::cout << "Finished." << std::endl;

	return 0;
}


