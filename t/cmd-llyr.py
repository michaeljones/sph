#!/usr/bin/python

from llyr import Region, Point, FrameRange, DataFactory, run, view
from optparse import OptionParser

import ctypes
import yaml
import glob
import sys
import os

class ConfigFactory(object):

    def create(self, sim_config):

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

        sim_data["filename"] = sim_config["filename"]
        sim_data["logfile"] = sim_config["logfile"]

        view_data = {}
        view_data["frame_range"] = FrameRange(
                start=sim_config["frame_range"]["start"],
                end=sim_config["frame_range"]["end"],
                substeps=sim_config["frame_range"]["substeps"],
                )

        view_data["filename"] = sim_config["filename"]

        return sim_data, view_data

def dispatch_run( root, config ):

    runs = glob.glob(os.path.join(root, "run*"))
    run_num = len(runs) + 1

    run_name = "run%03d" % run_num
    directory = os.path.join( root, run_name )

    try:
        os.makedirs( directory )
    except OSError:
        print "Directory already exists: %s" % directory

    cached_config_path = os.path.join( directory, "config.lc" )
    print "Caching config: %s" % cached_config_path

    config["filename"] = os.path.join( directory, "particles" )
    config["logfile"] = os.path.join( directory, "log.log" )

    config_factory = ConfigFactory()
    sim_config, view_config = config_factory.create( config )

    cached_config = open( cached_config_path, "w" )
    yaml.dump( config, cached_config )
    cached_config.close()

    factory = DataFactory()
    data = factory.create_sim_data(
        **sim_config
        )

    print
    run( data )

def float_range(start, stop, step):
    while start < stop:
        yield start
        start += step

class Dispatcher(object):

    def __init__(self):

        pass

    def simulate(self, opts, args):

        try:
            config_file = args[2]
        except IndexError:
            sys.stderr.write( "usage: llyr sim <config file>\n" )
            return 1

        config_stream = open( config_file )
        config = yaml.load( config_stream )
        config_stream.close()

        if opts.wedge:

            wedges = glob.glob("output/wedge*")
            wedge_num = len(wedges) + 1

            wedge_name = "wedge%03d" % wedge_num

            parameter, start, end, step = opts.wedge.split(":")
            start, end, step = float(start), float(end), float(step)

            print "Wedging '%s' with [%s, %s, %s]" % ( parameter, start, end, step )

            for value in float_range(start, end, step):

                sim_config = config["simulation"]

                sim_config[parameter] = value

                print "Run with '%s' set to %s" % ( parameter, value )

                dispatch_run(
                        os.path.join( "output", wedge_name ), 
                        sim_config
                        )
                print
                print


        else:

            sim_config = config["simulation"]

            dispatch_run(
                    "output", 
                    sim_config
                    )

    def view(self, opts, args):

        try:
            directory = args[2]
        except IndexError:
            sys.stderr.write( "usage: llyr view <run directory>\n" )
            return 1

        # Read config file
        config_file = os.path.join( directory, "config.lc" )
        config_stream = open( config_file )
        config = yaml.load( config_stream )
        config_stream.close()

        config_factory = ConfigFactory()
        sim_config, view_config = config_factory.create( config )

        factory = DataFactory()
        view_data = factory.create_view_data(
                 frame_range=view_config["frame_range"],
                 filename=view_config["filename"],
                 z_depth=-9.0,
                 )

        view( view_data )




def main( argv ):

    parser = OptionParser()

    # We currently parse but ignore -d
    parser.add_option( "-d", "--debug", action="store_true", default=False, help="Run in debug mode" )
    # parser.add_option( "-i", "--view", dest="view", action="store_true", default=False, help="Only view the result" )
    parser.add_option( "-w", "--wedge", dest="wedge", default=None, help="Set of a series of simulations over a particular parameter" )

    opts, args = parser.parse_args( argv )

    dispatcher = Dispatcher()

    if args[1] == "sim":

        return dispatcher.simulate(opts, args)

    elif args[1] == "view":

        return dispatcher.view(opts, args)

    return 1

if __name__ == "__main__":
    sys.exit( main( sys.argv ) )

