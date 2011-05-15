#ifndef READER_HH
#define READER_HH

#include <fstream>
#include <sstream>
#include <iomanip>

class Reader
{
public:

    Reader() {}
    virtual ~Reader() {}

    virtual void read( int frame, VectorArray& position ) const = 0;

};

class LyReader : public Reader
{
public:

    LyReader( const std::string& filename )
     : m_filename( filename ) {}

    void read( int frame, VectorArray& position ) const 
    {
        std::ostringstream filename;
        filename << m_filename << "." << std::setfill( '0' ) << std::setw( 4 ) << frame << ".ly";

        std::ifstream stream( filename.str().c_str(), std::ios_base::in | std::ios_base::binary );

        if ( stream.is_open() )
        {
            unsigned int version;
            stream.read( reinterpret_cast< char* >( &version ), sizeof(unsigned int) );

            size_t particleCount;
            stream.read( reinterpret_cast< char* >( &particleCount ), sizeof( size_t ) );

            position.resize( particleCount );

            for ( size_t i=0; i<particleCount; ++i )
            {
                stream.read( reinterpret_cast< char* >( &position[i].x ), sizeof( float ) );
                stream.read( reinterpret_cast< char* >( &position[i].y ), sizeof( float ) );
            }
        }
    }

private:

    const std::string m_filename;
};

#endif // READER_HH

