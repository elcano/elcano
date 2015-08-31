# Provides functions for processing a training set of images separated into
# orange cones and non-orange-cone backgrounds, and generating a table of the
# conditional probability of an orange cone given a color.  Also provides a
# class to hold said table and perform lookups of the probability given a
# color.

# Allow division to return a float even when arguments are integers.
from __future__ import division

import math
import Image

# A background color to be excluded from images.  Default for the orange cone
# images is white.  If a transparent background is used, then is not needed.
SKIP_COLOR = (255, 255, 255)

# There are too few images to have a valid probability per each r, g, b value
# for r, g, b, in 0..255.  So bin the r, g, b, values.  This is the default
# bin width.
BINSIZE = 4

# Constants for keys in the command-line arguments.
DIR = "DIR"
CONEIMGS = "CONEIMGS"
BGIMGS = "BGIMGS"
CONEHIST = "CONEHIST"
BGHIST = "BGHIST"
CONETBL = "CONETBL"
CONEOBJ = "CONEOBJ"
BIN = "BIN"
SKIP = "SKIP"

# This class holds orange cone probability info and provides accessors.
# Typically an OrangeConeProbability object will be created by reading in the
# probability table from a pickle object or from a csv file.  Given an
# OrangeConeProbability object, say, ocp, and a color tuple rgb = (r, g, b),
# to get the probability that this belongs to an orange cone, do:
#
# prob = ocp(rgb)
#
# If you also have a modified prior probability for orange cone pixels (i.e.
# the fraction of image pixels that you expect to contain orange cone), then
# that can be included in the call:
#
# prob = ocp(rgb, alt_prior)

class OrangeConeProbability:

    # @table - dict of color versus probability P(orange cone | color), for
    # orange cone colors only.  All other colors are assumed to have probability
    # zero.
    # @prior - P(orange cone) constant default value, derived from training
    # images.  This can (should) be adjusted according to the probability of
    # orange cone pixels in the current camera view, based on location and
    # orientations of the robot, by supplying an alt_prior when reading out the
    # conditional probability.
    def __init__(self, prior, table):
        self.prior = prior
        self.table = table

    # Given an tuple (r, g, b), return the conditional probability of an orange
    # cone given that color.  An alternative prior probability of orange cone
    # can be supplied.  This should be set to the fraction of the image that
    # the orange cone is expected to occupy.
    def __call__(self, color, alt_prior=None):
        prob = self.table.get(color, 0)
        if prob != 0 and alt_prior != None:
            prob = prob * alt_prior / self.prior
        return prob
        
    # Given an OrangeConeProbability object, write out a csv file representing
    # the object.  The only purpose of this is to have something readable in
    # other languages.  For saving and restoring in Python, it's better to
    # serialize out the object.
    def write_ocp_csv(self, ocp_file):
        import csv
        with open(ocp_file, "w") as ocp_handle:
            writer = csv.writer(ocp_handle)
            writer.writerow([-1,-1,-1,self.prior])
            for rgb, prob in self.table.iteritems():
                r,g,b = rgb
                writer.writerow([r,g,b,prob])

    # Serialize out an OrangeConeProbability object.
    def write_ocp(self, ocp_file):
        import cPickle as pickle
        with open(ocp_file, "wb") as ocp_handle:
            pickle.dump(self, ocp_handle, pickle.HIGHEST_PROTOCOL)

# -----------------------------------------------------------------------------

# Helper for process_images that updates the histogram hash table.
def count_colors(img, hist, listtotal, none_value, skip_color, binsize):
    img = img.convert("RGBA")
    imgpixels = img.load()
    w,h = img.size
    total = listtotal[0]
    for x in range(0,w):
        for y in range(0,h):
            rgba = imgpixels[x,y]
            # If the pixel is 1) not transparent, and 2) is not the color
            # to skip (if that's defined), then process it.
            if rgba[-1] != 0 and (rgba[0:3] != skip_color):
                # Bin the orange values -- we don't need a full 256 gradations
                # and we don't have enough orange data to support that fine of
                # granularity.
                if binsize == 1:
                    rgb = rgba[0:3]
                else:
                    r,g,b,_a = rgba
                    # math.floor returns a float
                    r = int(math.floor(r/binsize) * binsize)
                    g = int(math.floor(g/binsize) * binsize)
                    b = int(math.floor(b/binsize) * binsize)
                    rgb = (r,g,b)
                count = hist.get(rgb, none_value)
                if count >= 0:
                    hist[rgb] = count + 1
                    total = total + 1
    listtotal[0] = total

# Read in an image file containing color samples, and generate a histogram
# of colors other than a specified background color (default is white) or
# transparent.  This variant makes an empty dict to store the counts in,
# indexed by triples of (r,g,b) as integers in the range 0 - 255.  This is
# for the orange cones, which are expected to be sparse.  The background
# counts are only needed where there are orange counts, so we'll write out
# the hash keys to use for the background histogram.
#
# This is run in two stages, as we first need to know which (very limited)
# set of rgb values there are in the orange cones.  Once we know which rgb
# values we need to consider, we can then process the backgrounds, and ignore
# anything that is not an orange cone color.  See __main__ for the complete
# workflow.
def process_images(fgfile, bgfile, imgpaths, skip_color=SKIP_COLOR, binsize=BINSIZE):
    import csv
    import os
    import os.path

    hist = dict()
    listtotal = [0]

    # Assume we're processing the foreground colors (orange cones).
    csvfile = fgfile
    none_value = 0

    if bgfile != "":
        # Here, we have already processed the foreground colors.
        # Use this as a marker for non-existent keys.
        none_value = -1
        # Backgrounds may legitimately contain the color used as a background
        # for the cones.  @ToDo, place cones on a transparent background and
        # then this can be removed.
        skip_color = False
        # Read those back into the dict but clear out the counts.
        with open(fgfile, "r") as fghandle:
            reader = csv.reader(fghandle)
            # Skip the first row with the total.
            row = reader.next()
            # reader.next()
            for row in reader:
                r,g,b,count = [int(x) for x in row]
                # We don't need the count -- just store a zero for each key.
                hist[(r,g,b)] = 0
            
        # We're going to write the background colors.
        csvfile = bgfile

    # Each item may be a single file or a directory.
    for imgpath in imgpaths:
        if os.path.isdir(imgpath):
            imgfiles = [os.path.join(imgpath, img) for img in os.listdir(imgpath)]
        else:
            imgfiles = [imgpath]
        for imgfile in imgfiles:
            with open(imgfile, "rb") as imghandle:
                img = Image.open(imghandle)
                # Wrap total in a list so it can be modified.
                count_colors(img, hist, listtotal, none_value, skip_color, binsize)

    # Here, we have a dict with a (sparse) image histogram.
    # Write it out as a csv file.  Also get the total count, though it's
    # problematic where to put it.  For now, return it and let the caller
    # print it.
    with open(csvfile, "w") as csvhandle:
        writer = csv.writer(csvhandle)
        # Write out a row to hold the total count of pixels.  We need this
        # for computing the probability of each pixel values, but want to
        # keep the raw counts as we may be adding more images.
        writer.writerow([-1,-1,-1,listtotal[0]])
        for rgb, count in hist.iteritems():
            r,g,b = rgb
            writer.writerow([r,g,b,count])
            #total += count

    return listtotal[0]

# -----------------------------------------------------------------------------

# Read in one color histogram and return the total count and dict of
# counts per color.  The histogram file is expected to have the total
# pixels in the first row -- rgb values are ignored for this row.
# Subsequent rows have r, g, b, count.
def read_histogram(hist_file):
    import csv
    with open(hist_file, "r") as hist_handle:
        hist_reader = csv.reader(hist_handle)
        _r,_g,_b,total = hist_reader.next()
        total = int(total)
        hist = dict()
        for row in hist_reader:
            r,g,b,count = row
            hist[(int(r),int(g),int(b))] = int(count)
        return (total, hist)

# Read in orange and background histograms, and construct
# P(orange cone | color) = P(color | orange cone) P(orange cone) / P(color)
# The orange histogram holds the raw counts for P(color | orange cone).
# The background histogram likewise holds the raw counts for
# P(color | background).
# Return an OrangeConeProbability object.
def ocp_from_histograms(orange_file, background_file):
    or_total, or_hist = read_histogram(orange_file)
    bg_total, bg_hist = read_histogram(background_file)
    total = or_total + bg_total
    # Prior P(orange cone) for the training images.  Save this in case we
    # adapt it per location and heading later.
    p_or = or_total / total
    # Combine the two histograms.
    p_or_per_color = dict()
    for rgb, or_count in or_hist.iteritems():
        bg_count = bg_hist.get(rgb, 0)
        # P(color) is the count for this rgb value / total pixels.
        # But don't divide yet, because...
        # P(color | orange cone) is the orange count / total orange pixels.
        # P(orange cone | color)
        #   = P(color | orange cone) P(orange cone) / P(color)
        #   = (orange count for this color / total orange pixels)
        #     * (total orange pixels / total pixels)
        #     / (count for this color) / total pixels)
        #   = orange count for this color / count for this color
        #   = orange count for this color /
        #     (orange count for this color + bg count for this color)
        # Another way to say this is, we counted orange pixels in the
        # parts of the images identified as orange cones, and orange pixels
        # in the remainder of the images, so the probability that some
        # specific orange color belongs to the cones is the number of
        # pixels of that color in the cones divided by the number of pixels
        # of that color anywhere.
        color_count = or_count + bg_count
        p_or_per_color[rgb] = or_count / color_count
    return OrangeConeProbability(p_or, p_or_per_color)

# Read in a saved OrangeConeProbability csv file.  Return an
# OrangeConeProbability object.
def read_ocp_csv(ocp_file):
    import csv
    with open(ocp_file, "r") as ocp_handle:
        reader = csv.reader(ocp_handle)
        _r,_g,_b,prior = reader.next()
        table = dict()
        for row in reader:
            r,g,b,prob = row
            table[(int(r),int(g),int(b))] = float(prob)
    return OrangeConeProbability(prior, table)

# Read in a serialized OrangeConeProbability object.
def read_ocp(ocp_file):
    import cPickle as pickle
    with open(ocp_file, "rb") as ocp_handle:
        ocp = pickle.load(ocp_handle)
    return ocp

# -----------------------------------------------------------------------------    

# Run all three parts:
# Histogram the cones.
# Histogram the backgrounds.
# Compute the conditional probability of orange cone given color.
#
# Example command line, for work in a directory called "vision_work", cone
# images in subdirectory "cones" and background images in "backgrounds":
#
# python orange_cone_probabilities.py \
# --directory "Vision" \
# --orange_cone_images "orange_cones_cropped.png" \
# --background_images "backgrounds" \
# --orange_cone_histogram "orange.csv" \
# --background_histogram "backgrounds.csv" \
# --orange_cone_probability_table "ocp.csv" \
# --orange_cone_probability_object "ocp.pkl" \
# --skip_color "255,255,255" \
# --binsize 4
#
# (Caution -- don't run this in your elcano git repository unless you really
# want to change the csv files there, else you'll have a spurious change.)

if __name__ == '__main__':

    import argparse
    import os

    parser = argparse.ArgumentParser(
        description = """
        Generate table of conditional probability of an orange cone, given
        a color.
        """,
        usage = """
        python orange_cone_probabilities.py
            --directory [If provided, will cd to this before attempting to
              look up images. This is also where outputs will be written, else
              to current directory.]
            --orange_cone_images [Comma separated list of orange cone images.
              If this is a directory, all files in the directory will be processed.]
            --background_images [Comma separated list of background images.
              If this is a directory, all files in the directory will be processed.]
            --orange_cone_histogram [Name of file to receive the histogram of
              orange colors, as a csv file.]
            --background_histogram [Name of file to receive the histogram of
              background colors, as a csv file.]
            --orange_cone_probability_table [Name of file to receive the
              orange cone conditional probability table, as a csv file.]
            --orange_cone_probability_object [Name of file to receive the
              pickled OrangeConeProbability object.]
            --skip_color [Color to ignore when processing images, as an (r,g,b)
              tuple with r,g,b in the range 0-255. If orange cones were cropped
              and placed into a single image with a background color, supply
              that here. Default is white. For preference, use a transparent
              background and disable this by setting it to None. Background
              images with orange cones cropped out will likely require the
              holes to be transparent, as all colors are expected to be legal
              in backgrounds.]
            --binsize [At what granularity should r,g,b be binned in the
              probability table indices? Default is 4.]
        """)
    parser.add_argument(
        "--directory", dest=DIR,
        help="""
If provided, will cd to this before attempting to look up images.
This is also where outputs will be written, else to current directory.
""")
    parser.add_argument(
        "--orange_cone_images", dest=CONEIMGS,
        help="""
Comma separated list of orange cone images.
""")
    parser.add_argument(
        "--background_images", dest=BGIMGS,
        help="""
Comma separated list of background images.
""")
    parser.add_argument(
        "--orange_cone_histogram", dest=CONEHIST,
        help="""
Name of file to receive the histogram of orange colors, as a csv file.
""")
    parser.add_argument(
        "--background_histogram", dest=BGHIST,
        help="""
Name of file to receive the histogram of background colors, as a csv file.
""")
    parser.add_argument(
        "--orange_cone_probability_table", dest=CONETBL,
        help="""
Name of file to receive the orange cone conditional probability table,
as a csv file.
""")
    parser.add_argument(
        "--orange_cone_probability_object", dest=CONEOBJ,
        help="""
Name of file to receive the pickled OrangeConeProbability object.
""")
    parser.add_argument(
        "--skip_color", dest=SKIP,
        help="""
Color to ignore when processing images, as a comma-separated triple r,g,b with
values in the range 0-255. If orange cones were cropped and placed into a
single image with a background color other than white, supply that here.
For preference, use a transparent background and disable this by setting it
to False. Background images with orange cones cropped out will likely require
the holes to be transparent, as all colors are expected to be legal in
backgrounds.
""")
    parser.add_argument(
        "--binsize", dest=BIN,
        help="""
At what granularity should r,g,b be binned in the probability table indices?
Default is 4.
""")
    args = vars(parser.parse_args())

    if args[DIR]:
        os.chdir(args[DIR])
    if args[SKIP]:
        if args[SKIP] == "False":
            args[SKIP] = False
        else:
            args[SKIP] = tuple(int(n) for n in args[SKIP].split(","))
    else:
        args[SKIP] = SKIP_COLOR
    if args[BIN]:
        args[BIN] = int(args[BIN])
    else:
        args[BIN] = BINSIZE

    # First step is to histogram the orange cones by themselves.  This is done
    # first so we'll know the colors that are in the orange cones, which are a
    # very small subset of all r,g,b space.  Histograms and the final
    # probability table are stored in dicts, which are hash tables, keyed by
    # the r,g,b values, and we don't need to store values for anything but the
    # orange colors, so this yields a much smaller storage requirement.
    args[CONEIMGS] = [img.strip() for img in args[CONEIMGS].split(",")]
    total = process_images(args[CONEHIST], "", args[CONEIMGS], args[SKIP], args[BIN])
    print "Total # pixels included in cone histogram = %d" % total

    # Next histogram the backgrounds.
    args[BGIMGS] = [img.strip() for img in args[BGIMGS].split(",")]
    total = process_images(args[CONEHIST], args[BGHIST], args[BGIMGS], args[SKIP], args[BIN])
    print "Total # pixels included in background histogram = %d" % total

    # And finally, compute the conditional probability that a pixel belongs to
    # an orange cone, given the color.
    ocp = ocp_from_histograms(args[CONEHIST], args[BGHIST])
    ocp.write_ocp_csv(args[CONETBL])
    ocp.write_ocp(args[CONEOBJ])
    print "Orange cone probability table and object written."