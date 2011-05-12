#ifndef ARRAY_HH
#define ARRAY_HH

#include <ImathVec.h>
#include <vector>

typedef std::vector< Imath::V2f > VectorArray;
typedef std::vector< float > FloatArray;

void min_max_average( const std::string name, VectorArray& data )
{
    Imath::V2f min(data[0]);
    Imath::V2f max(data[0]);
    Imath::V2f avg(data[0]);

    for ( unsigned int i=1; i<data.size(); ++i )
    {
        avg += data[i];

        if ( data[i].x < min.x ) min.x = data[i].x;
        if ( data[i].y < min.y ) min.y = data[i].y;

        if ( data[i].x > max.x ) max.x = data[i].x;
        if ( data[i].y > max.y ) max.y = data[i].y;

    }

    avg /= data.size();

    std::cout << name << " min: " << min << std::endl;
    std::cout << name << " max: " << max << std::endl;
    std::cout << name << " avg: " << avg << std::endl;

}


#endif // ARRAY_HH


