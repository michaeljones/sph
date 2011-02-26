#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Display
{
public:

    Display( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    void draw()
    {
        unsigned int numParticles = m_particles.size();
    
        glBegin( GL_POINTS );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            glVertex3f( m_particles[i]->pos.x, m_particles[i]->pos.y, -0.5f );
        }
        glEnd();

        /*
        glBegin( GL_LINES );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];
            glVertex3f( p->pos.x, p->pos.y, -0.5f );
            glVertex3f( p->pos.x + p->force.x/100.0f, p->pos.y + p->force.y/100.0f, -0.5f );
        }
        glEnd();
        */
    }

private:

    ParticlePtrArray& m_particles;
};

#endif // DISPLAY_HH

