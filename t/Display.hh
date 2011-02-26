#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Display
{
public:

    Display( ParticlePtrArray& particles, float zDepth, float h )
     : m_zDepth( zDepth ),
       m_h( h ),
       m_particles( particles ) {}

    void draw()
    {
        unsigned int numParticles = m_particles.size();
    
        glBegin( GL_POINTS );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            glVertex3f( m_particles[i]->pos.x, m_particles[i]->pos.y, m_zDepth );
        }
        glEnd();

        const float fs = 0.1;

        glBegin( GL_LINES );
        glColor3f( 1.0f, 0.0f, 0.0f );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];
            glVertex3f( p->pos.x, p->pos.y, m_zDepth );
            glVertex3f( p->pos.x + p->f_pressure.x * fs, p->pos.y + p->f_pressure.y * fs, m_zDepth );
        }
        glEnd();

        glBegin( GL_LINES );
        glColor3f( 0.0f, 1.0f, 0.0f );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];
            glVertex3f( p->pos.x, p->pos.y, m_zDepth );
            glVertex3f( p->pos.x + p->f_viscosity.x * fs, p->pos.y + p->f_viscosity.y * fs, m_zDepth );
        }
        glEnd();

        glBegin( GL_LINES );
        glColor3f( 0.0f, 0.0f, 1.0f );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];
            glVertex3f( p->pos.x, p->pos.y, m_zDepth );
            glVertex3f( p->pos.x + p->f_external.x * fs, p->pos.y + p->f_external.y * fs, m_zDepth );
        }
        glEnd();
    }

private:

    const float m_zDepth;
    const float m_h;

    ParticlePtrArray& m_particles;
};

#endif // DISPLAY_HH

