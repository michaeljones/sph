#!/usr/bin/python

from llyr import InputData, Region, Point, run

import sys

def basic_block():

    data = InputData(
            particleCount=200,
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particles=Region( min=Point( -0.5, -2.4 ), max=Point( 0.5, 0.5 ) ),
            zDepth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def left_block():

    data = InputData(
            particleCount=200,
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particles=Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ),
            zDepth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def main( argv ):

    runs = [
            basic_block,
            left_block
            ]
    
    try:
        index = int( argv[ 1 ] )
    except IndexError:
        index = len( runs ) - 1 

    dispatcher = runs[ index ]

    dispatcher()

if __name__ == "__main__":
    main( sys.argv )

