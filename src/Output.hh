#ifndef OUTPUT_HH
#define OUTPUT_HH

#include <fstream>
#include <sstream>
#include <iomanip>

class Output
{
public:

    Output() {}
    virtual ~Output() {}

    virtual void write( int frame ) const = 0;

};

class LyOutput : public Output
{
public:

    LyOutput( ParticleData& particles, const std::string& filename )
     : m_filename( filename ),
       m_particles( particles ) {}

    void write( int frame ) const 
    {
        std::ostringstream filename;
        filename << m_filename << "." << std::setfill( '0' ) << std::setw( 4 ) << frame << ".ly";

        std::ofstream stream( filename.str().c_str(), std::ios_base::out | std::ios_base::binary );

        unsigned int version = 1;

        if ( stream.is_open() )
        {
            // Version 1 of the file format
            stream.write( reinterpret_cast< const char* >( &version ), sizeof(unsigned int) );

            size_t particleCount = m_particles.position.size();
            stream.write( reinterpret_cast< const char* >( &particleCount ), sizeof( size_t ) );

            for ( size_t i=0; i<particleCount; ++i )
            {
                stream.write( reinterpret_cast< const char* >( &m_particles.position[i].x ), sizeof( float ) );
                stream.write( reinterpret_cast< const char* >( &m_particles.position[i].y ), sizeof( float ) );
            }
        }
    }

private:

    const std::string m_filename;
    ParticleData& m_particles;
    
};

class NullOutput : public Output
{
    NullOutput() {}

    void write( int frame ) const {}

};


#endif // OUTPUT_HH

