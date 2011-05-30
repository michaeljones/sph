#!/usr/bin/python

from llyr import Region, Point, FrameRange, DataFactory, run, view
from optparse import OptionParser

import ctypes
import yaml
import sys

class ConfigFactory(object):

    def read(self, config_file):

        file_ = open( config_file )
        config = yaml.load( file_ )

        sim_config = config["simulation"]

        sim_data = {}

        # Populate simulation data
        #
        sim_data["frame_range"] = FrameRange(
                start=sim_config["frame_range"]["start"],
                end=sim_config["frame_range"]["end"],
                substeps=sim_config["frame_range"]["substeps"],
                )

        sim_data["container"] = Region(
                min=Point( sim_config["container"]["min"][0], sim_config["container"]["min"][1] ),
                max=Point( sim_config["container"]["max"][0], sim_config["container"]["max"][1] )
                )

        sim_data["particle_regions"] = []
        for entry in sim_config["particles"]:
            (key,) = entry.keys()
            if key == "region":
                sim_data["particle_regions"].append(
                        Region(
                            min=Point( entry["region"]["min"][0], entry["region"]["min"][1] ),
                            max=Point( entry["region"]["max"][0], entry["region"]["max"][1] )
                            )
                        )

        sim_data["box_boundaries"] = []
        for entry in sim_config["boundaries"]:
            (key,) = entry.keys()
            if key == "region":
                sim_data["box_boundaries"].append(
                        Region(
                            min=Point( entry["region"]["min"][0], entry["region"]["min"][1] ),
                            max=Point( entry["region"]["max"][0], entry["region"]["max"][1] )
                            )
                        )

        sim_data["h"] = sim_config["h"]
        sim_data["viscosity"] = sim_config["viscosity"]
        sim_data["pressure_constant"] = sim_config["pressure_constant"]
        sim_data["average_density"] = sim_config["average_density"]
        sim_data["gravity"] = sim_config["gravity"]

        return sim_data, None


def main( argv ):

    parser = OptionParser()

    # We currently parse but ignore -d
    parser.add_option( "-d", "--debug", action="store_true", default=False, help="Run in debug mode" )
    parser.add_option( "-i", "--view", dest="view", action="store_true", default=False, help="Only view the result" )

    opts, args = parser.parse_args( argv )

    config_file = args[1]

    config_factory = ConfigFactory()
    sim_config, view_config = config_factory.read( config_file )

    sim_config["filename"] = "output/larger2"
    sim_config["logfile"] = "log.log"

    factory = DataFactory()
    data = factory.create_sim_data(
        **sim_config
        )

    run( data )


if __name__ == "__main__":
    main( sys.argv )

