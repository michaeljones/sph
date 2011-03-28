#!/usr/bin/python

from llyr import Region, Point, run, create
from optparse import OptionParser

import ctypes
import sys

def basic_block():

    print "Basic Block"
    data = create(
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[
                Region( min=Point( -0.5, -2.4 ), max=Point( 0.5, 0.5 ) ),
                ],
            z_depth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def left_block():

    print "Left Block"
    data = create(
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[ Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ) ],
            z_depth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def high_grav_block():

    print "High Grav"
    data = create(
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[ Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ) ],
            z_depth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=1000.81
            )

    run( data )


def two_blocks():

    print "Two Blocks"
    data = create(
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[
                Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ),
                Region( min=Point( 1.0, 1.5 ), max=Point( 1.5, 2.0 ) ),
                ],
            z_depth=-9.0,
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def main( argv ):

    parser = OptionParser()
    parser.add_option( "-d", "--debug", action="store_true", default=False, help="Run in debug mode" )

    opts, args = parser.parse_args( argv )

    runs = [
            basic_block,
            high_grav_block,
            left_block,
            two_blocks,
            ]
    
    try:
        index = int( args[ 1 ] )
    except IndexError:
        index = len( runs ) - 1 

    dispatcher = runs[ index ]

    dispatcher()

if __name__ == "__main__":
    main( sys.argv )

