#!/usr/bin/python

from llyr import InputData, Region, Point, run

data = InputData(
        particleCount=200,
        container=Region( min=Point( -2.5, -2.5 ), max=Point( 2.5, 2.5 ) ),
        particles=Region( min=Point( -0.5, -0.5 ), max=Point( 0.5, 0.5 ) ),
        zDepth=-9.0,
        h=0.1,
        viscosity=12.94e-2,
        logfile="log.log",
        gravity=200.81
        )

run( data )

