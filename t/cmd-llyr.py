#!/usr/bin/python

from llyr import Region, Point, FrameRange, DataFactory, run, view
from optparse import OptionParser

import ctypes
import sys

def basic_block():

    factory = DataFactory()

    print "Basic Block"
    data = create_sim_data(
            frameRange=FrameRange( start=1, end=200 ),
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[
                Region( min=Point( -0.5, -2.4 ), max=Point( 0.5, 0.5 ) ),
                ],
            box_boundaries=[],
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def left_block():

    factory = DataFactory()

    print "Left Block"
    data = create_sim_data(
            frameRange=FrameRange( start=1, end=200 ),
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[ Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ) ],
            box_boundaries=[],
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def high_grav_block():

    factory = DataFactory()

    print "High Grav"
    data = factory.create_sim_data(
            frameRange=FrameRange( start=1, end=200 ),
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[ Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ) ],
            box_boundaries=[],
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=1000.81
            )

    run( data )


def two_blocks():

    factory = DataFactory()

    print "Two Blocks"
    data = factory.create_sim_data(
            frameRange=FrameRange( start=1, end=200 ),
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[
                Region( min=Point( -2.4, -1.4 ), max=Point( -1.0, 2.4 ) ),
                Region( min=Point( 1.0, 1.5 ), max=Point( 1.5, 2.0 ) ),
                ],
            box_boundaries=[],
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def simple_collision():

    factory = DataFactory()

    print "Simple Collision"
    data = factory.create_sim_data(
            frameRange=FrameRange( start=1, end=200 ),
            container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
            particle_regions=[
                Region( min=Point( 0.2, 1.0 ), max=Point( 0.4, 1.3 ) ),
                ],
            box_boundaries=[
                Region( min=Point( -1.0, -1.5 ), max=Point( 0.5, -0.5 ) ),
                ],
            h=0.1,
            viscosity=12.94e-2,
            logfile="log.log",
            gravity=200.81
            )

    run( data )


def more_particles_collision( view_only ):

    factory = DataFactory()

    frame_range = FrameRange( start=1, end=200, substeps=100 )

    filename = "output/tester"

    print "Collision with More Particles"
    if not view_only:

        data = factory.create_sim_data(
                frameRange=frame_range,
                filename=filename,
                container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
                particle_regions=[
                    Region( min=Point( -1.0, 0.0 ), max=Point( 0.5, 2.0 ) ),
                    ],
                box_boundaries=[
                    Region( min=Point( -1.0, -1.5 ), max=Point( 0.5, -0.5 ) ),
                    ],
                h=0.1,
                viscosity=12.94e-4,
                gravity=9.81,
                logfile="log.log",
                )

        run( data )

    view_data = factory.create_view_data(
            frameRange=frame_range,
            filename=filename,
            z_depth=-9.0,
            )

    view( view_data )


def larger( view_only ):

    factory = DataFactory()

    frame_range = FrameRange( start=1, end=200, substeps=100 )

    filename = "output/larger"

    print "Larger Simulation"
    if not view_only:

        data = factory.create_sim_data(
                frameRange=frame_range,
                filename=filename,
                container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
                particle_regions=[
                    Region( min=Point( -1.0, 0.0 ), max=Point( 0.5, 2.0 ) ),
                    ],
                box_boundaries=[
                    Region( min=Point( -1.0, -1.5 ), max=Point( 0.5, -0.5 ) ),
                    ],
                h=0.05,
                viscosity=12.94e-4,
                gravity=9.81,
                logfile="log.log",
                )

        run( data )

    view_data = factory.create_view_data(
            frameRange=frame_range,
            filename=filename,
            z_depth=-9.0,
            )

    view( view_data )


def main( argv ):

    parser = OptionParser()

    # We currently parse but ignore -d
    parser.add_option( "-d", "--debug", action="store_true", default=False, help="Run in debug mode" )
    parser.add_option( "-i", "--view", dest="view", action="store_true", default=False, help="Only view the result" )

    opts, args = parser.parse_args( argv )

    runs = [
            basic_block,
            high_grav_block,
            left_block,
            two_blocks,
            simple_collision,
            more_particles_collision,
            larger,
            ]

    try:
        index = int( args[ 1 ] )
    except IndexError:
        index = len( runs ) - 1 

    dispatcher = runs[ index ]

    dispatcher( opts.view )


if __name__ == "__main__":
    main( sys.argv )

