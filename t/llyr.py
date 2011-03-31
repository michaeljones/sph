
from ctypes import cdll, c_int, c_float, c_char_p, pointer, Structure, POINTER

class Point( Structure ):

    _fields_ = [
            ( "x", c_float ),
            ( "y", c_float ),
            ]

class Region( Structure ):

    _fields_ = [
            ( "min", Point ),
            ( "max", Point ),
            ]

class RegionGroup( Structure ):

    _fields_ = [
            ( "numRegions", c_int ),
            ( "regions", POINTER( Region ) ),
            ]

class InputData( Structure ):

    _fields_ = [
            ( "container", Region ),
            ( "particleRegions", RegionGroup ),
            ( "boxBoundaries", RegionGroup ),
            ( "zDepth", c_float ),
            ( "h", c_float ),
            ( "viscosity", c_float ),
            ( "gravity", c_float ),
            ( "logfile", c_char_p ),
            ]

def create_region_group( regions ):

    num_regions = len( regions )

    region_data = ( Region * num_regions )(
            *regions
            )

    return RegionGroup(
            numRegions=num_regions,
            regions=region_data
            )


def create( 
        container,
        particle_regions,
        box_boundaries,
        z_depth,
        h,
        viscosity,
        logfile,
        gravity,
        ):

    particle_region_group = create_region_group( particle_regions )
    box_region_group = create_region_group( box_boundaries )

    return InputData(
            container=container,
            particleRegions=particle_region_group,
            boxBoundaries=box_region_group,
            zDepth=z_depth,
            h=h,
            viscosity=viscosity,
            logfile=logfile,
            gravity=gravity,
            )

def run( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.run( data )


