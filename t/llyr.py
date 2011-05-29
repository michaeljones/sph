
from ctypes import cdll, c_int, c_float, c_char_p, pointer, Structure, POINTER

class Point( Structure ):

    _fields_ = [
            ( "x", c_float ),
            ( "y", c_float ),
            ]

class Region( Structure ):

    _fields_ = [
            ( "min", Point ),
            ( "max", Point ),
            ]

class RegionGroup( Structure ):

    _fields_ = [
            ( "numRegions", c_int ),
            ( "regions", POINTER( Region ) ),
            ]

class FrameRange( Structure ):

    _fields_ = [
            ( "start", c_int ),
            ( "end", c_int ),
            ( "substeps", c_int ),
            ]

class SimData( Structure ):

    _fields_ = [
            ( "frameRange", FrameRange ),
            ( "filename", c_char_p ),
            ( "container", Region ),
            ( "particleRegions", RegionGroup ),
            ( "boxBoundaries", RegionGroup ),
            ( "h", c_float ),
            ( "viscosity", c_float ),
            ( "gravity", c_float ),
            ( "logfile", c_char_p ),
            ]

class ViewData( Structure ):

    _fields_ = [
            ( "frameRange", FrameRange ),
            ( "filename", c_char_p ),
            ( "zDepth", c_float ),
            ]

def create_region_group( regions ):

    num_regions = len( regions )

    region_data = ( Region * num_regions )(
            *regions
            )

    return RegionGroup(
            numRegions=num_regions,
            regions=region_data
            )


class DataFactory(object):

    def create_sim_data(
            self,
            frame_range,
            filename,
            container,
            particle_regions,
            box_boundaries,
            h,
            viscosity,
            logfile,
            gravity,
            ):

        particle_region_group = create_region_group( particle_regions )
        box_region_group = create_region_group( box_boundaries )

        return SimData(
                frameRange=frame_range,
                filename=filename,
                container=container,
                particleRegions=particle_region_group,
                boxBoundaries=box_region_group,
                h=h,
                viscosity=viscosity,
                logfile=logfile,
                gravity=gravity,
                )


    def create_view_data(
            self,
            frame_range,
            filename,
            z_depth,
            ):

        return ViewData(
                frameRange=frame_range,
                filename=filename,
                zDepth=z_depth,
                )

def run( data ):

    libc = cdll.LoadLibrary( "libc.so" )
    libc.run( data )

def view( data ):

    libv = cdll.LoadLibrary( "libv.so" )
    libv.view( data )

