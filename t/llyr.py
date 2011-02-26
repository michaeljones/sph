
from ctypes import cdll, c_int, c_float, c_char_p, pointer, Structure

class InputData( Structure ):

    _fields_ = [
            ( "particleCount", c_int ),
            ( "width", c_float ),
            ( "height", c_float ),
            ( "zDepth", c_float ),
            ( "h", c_float ),
            ( "viscosity", c_float ),
            ]

def run( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.run( data )


