#!/usr/bin/python

from llyr import InputData, run

data = InputData(
        particleCount=20,
        width=5.4,
        height=5.4,
        zDepth=-9.0,
        h=0.1,
        viscosity=4.94e-2,
        )

run( data )

