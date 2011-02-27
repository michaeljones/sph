#!/usr/bin/python

from llyr import InputData, run

data = InputData(
        particleCount=200,
        width=5.4,
        height=5.4,
        zDepth=-9.0,
        h=0.1,
        viscosity=8.94e-2,
        logfile="log.log",
        )

run( data )

