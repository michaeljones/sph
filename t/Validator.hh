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

    NanValidator( ParticleData& particles )
     : m_particles( particles ) {}

    bool valid()
    {
        unsigned int numParticles = m_particles.position.size();
        for ( unsigned int i=0; i<numParticles; ++i )
        {
            Imath::V2f& pos = m_particles.position[ i ];

            // Check for nans with self equalilty fail
            if ( pos != pos )
                return false;
        }

        return true;
    }

private:

    ParticleData& m_particles;
};

#endif // VALIDATOR_HH

