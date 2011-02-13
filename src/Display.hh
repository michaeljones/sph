#ifndef DISPLAY_HH
#define DISPLAY_HH

class Display
{
public:

    Display( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    void draw() {}

private:

    ParticlePtrArray& m_particles;
};

#endif // DISPLAY_HH

