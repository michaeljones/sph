#!/usr/bin/python

from llyr import InputData, run

data = InputData(
        particleCount=200,
        width=5,
        height=5,
        zDepth=-9.0,
        h=0.1,
        viscosity=12.94e-2,
        logfile="log.log",
        gravity=200.81
        )

run( data )

