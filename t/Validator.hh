#ifndef VALIDATOR_HH
#define VALIDATOR_HH

#include <Particle.hh>

class Validator
{
public:

    Validator() {}

    virtual bool valid() = 0;
};

class NanValidator : public Validator
{
public:

    NanValidator( ParticlePtrArray& particles )
     : m_particles( particles ) {}

    bool valid()
    {
        unsigned int numParticles = m_particles.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Particle* p = m_particles[i];

            if ( p->pos != p->pos )
                return false;
        }

        return true;
    }

private:

    ParticlePtrArray& m_particles;
};

#endif // VALIDATOR_HH

