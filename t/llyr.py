
from ctypes import cdll, c_int, c_char_p, pointer, Structure

class InputData( Structure ):

    _fields_ = [ ( "particleCount", c_int ), ]

def run( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.run( data )


