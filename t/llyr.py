
from ctypes import cdll, c_int, c_float, c_char_p, pointer, Structure

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
            ( "particleCount", c_int ),
            ( "container", Region ),
            ( "particles", Region ),
            ( "zDepth", c_float ),
            ( "h", c_float ),
            ( "viscosity", c_float ),
            ( "gravity", c_float ),
            ( "logfile", c_char_p ),
            ]

def run( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.run( data )


