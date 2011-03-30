#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Display
{
public:

    Display() {}
    virtual ~Display() {}

    virtual void draw() const = 0;

};

class ParticleDisplay : public Display
{
public:

    ParticleDisplay( ParticleData& particles, float zDepth, float h )
     : m_zDepth( zDepth ),
       m_h( h ),
       m_particles( particles ) {}

    void draw() const
    {
        unsigned int numParticles = m_particles.position.size();
    
        glBegin( GL_POINTS );
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            glVertex3f( m_particles.position[i].x, m_particles.position[i].y, m_zDepth );
        }
        glEnd();

        /*
        const float fs = 0.5;

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
        */
    }

private:

    const float m_zDepth;
    const float m_h;

    ParticleData& m_particles;
};

class BoxDisplay : public Display
{
public:

    BoxDisplay( Imath::V2f& max, Imath::V2f& min, float zDepth )
     : m_zDepth( zDepth ), m_max( max ), m_min( min ) {}

    void draw() const
    {
        glBegin( GL_LINES );
            glVertex3f( m_min.x, m_min.y, m_zDepth );
            glVertex3f( m_min.x, m_max.y, m_zDepth );

            glVertex3f( m_min.x, m_max.y, m_zDepth );
            glVertex3f( m_max.x, m_max.y, m_zDepth );

            glVertex3f( m_max.x, m_max.y, m_zDepth );
            glVertex3f( m_max.x, m_min.y, m_zDepth );

            glVertex3f( m_max.x, m_min.y, m_zDepth );
            glVertex3f( m_min.x, m_min.y, m_zDepth );
        glEnd();
    }

private:

    const float m_zDepth;

    const Imath::V2f m_max;
    const Imath::V2f m_min;

};


class MultiDisplay : public Display
{
public:

    MultiDisplay( std::vector< Display* >& displays )
     : m_displays( displays ) {}

    void draw() const 
    {
        std::vector< Display* >::const_iterator it = m_displays.begin();
        std::vector< Display* >::const_iterator end = m_displays.end();

        for ( ; it != end; ++it )
        {
            (*it)->draw();
        }
    }

private:

    const std::vector< Display* >& m_displays;

};

#endif // DISPLAY_HH

