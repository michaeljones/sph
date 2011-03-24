
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

class InputData( Structure ):

    _fields_ = [
            ( "container", Region ),
            ( "numParticleRegions", c_int ),
            ( "particleRegions", POINTER( Region ) ),
            ( "zDepth", c_float ),
            ( "h", c_float ),
            ( "viscosity", c_float ),
            ( "gravity", c_float ),
            ( "logfile", c_char_p ),
            ]

def create( 
        container,
        particle_regions,
        z_depth,
        h,
        viscosity,
        logfile,
        gravity,
        ):

    num_particle_regions = len( particle_regions )

    particle_region_data = ( Region * num_particle_regions )(
            *particle_regions
            )

    return InputData(
            container=container,
            numParticleRegions=num_particle_regions,
            particleRegions=particle_region_data,
            zDepth=z_depth,
            h=h,
            viscosity=viscosity,
            logfile=logfile,
            gravity=gravity,
            )

def run( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.run( data )


