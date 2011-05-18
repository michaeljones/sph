#ifndef GRID_HH
#define GRID_HH

typedef std::vector< int > Cell;

struct Resolution
{
    const unsigned int x;
    const unsigned int y;
};

class Grid
{
public:

    Grid( const std::vector< Cell >* cells, const Resolution& resolution )
     : m_resolution( resolution ),
       m_cells( cells ) {}

    ~Grid()
    {
        delete m_cells;
    }

    unsigned int xRes() const
    {
        return m_resolution.x;
    }
    
    unsigned int yRes() const
    {
        return m_resolution.y;
    }

    const Cell& cell( unsigned int x, unsigned int y ) const
    {
        return (*m_cells)[ x + y * m_resolution.x ];
    }

    unsigned int id( unsigned int x, unsigned int y ) const
    {
        return  x + y * m_resolution.x;
    }

private:

    const Resolution m_resolution;
    const std::vector< Cell >* m_cells;
};

class GridFactory
{
public:

    GridFactory( float h )
     : m_h( h ) {}

    const Grid* create( const VectorArray& positions, const Bounds& bounds ) const
    {
        Bounds expanded( bounds );
        expanded.extendBy( bounds.max + Imath::V2f( 1.01 *  m_h, 1.01 * m_h ) );
        expanded.extendBy( bounds.min + Imath::V2f( 1.01 *-m_h, 1.01 *-m_h ) );

        // Ceiling?
        int xRes = ceil( ( expanded.max.x - expanded.min.x ) / m_h );
        int yRes = ceil( ( expanded.max.y - expanded.min.y ) / m_h );

        const Resolution res = { xRes, yRes };

        std::vector< Cell >* cells = new std::vector< std::vector< int > >( res.x * res.y );

        unsigned int count = positions.size();

        for ( unsigned int i=0; i<count; ++i )
        {
            Imath::V2f local = positions[i] - expanded.min;

            int xCell = int( local.x / m_h );
            int yCell = int( local.y / m_h );

            (*cells)[ xCell + yCell * res.x ].push_back( i );
        }

        /*
        std::cerr << "post build" << std::endl;

        for ( int y=0; y < yRes; ++y )
        {
            for ( int x=0; x < xRes; ++x )
            {
                std::cerr << (*cells)[ x + y * xRes].size() << " ";
            }
            std::cerr << std::endl;
        }
        */

        return new Grid( cells, res );
    }

private:

    float m_h;

};

#endif // GRID_HH


