# Provides functions for processing a training set of images separated into
# orange cones and non-orange-cone backgrounds, and generating a table of the
# conditional probability of an orange cone given a color.  Also provides a
# class to hold said table and perform lookups of the probability given a
# color.

# Allow division to return a float even when arguments are integers.
from __future__ import division

import math
from PIL import Image

# A background color to be excluded from images.  Default for the orange cone
# images is white.  If a transparent background is used, then is not needed.
SKIP_COLOR = (255, 255, 255)

# There are too few images to have a valid probability per each r, g, b value
# for r, g, b, in 0..255.  So bin the r, g, b, values.  This is the default
# bin width.
BINSIZE = 4

# Constants for keys in command-line arguments.
DIR = "DIR"
CONEIMGS = "CONEIMGS"
BGIMGS = "BGIMGS"
CONEHIST = "CONEHIST"
BGHIST = "BGHIST"
CONETBL = "CONETBL"
CONEOBJ = "CONEOBJ"
BIN = "BIN"
SKIP = "SKIP"
RGBIMG = "RGBIMG"
OCPIMG = "OCPIMG"
ALTPRI = "ALTPRI"
GRAY = "GRAY"

# Appropriate image formats for modes.  Only TIFF supports floating point
# numbers for pixels.
IMAGE_FORMAT_BY_MODE = dict(F = "TIFF", L = "PNG", RGB = "PNG", RGBA = "PNG")

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
# that can be included in the call as alt_prior.  If you want grayscale
# integers in the range 0-255 rather than gloating point probabilities in the
# range 0-1, set the grayscale parameter to True.
#
# prob = ocp(rgb, alt_prior, grayscale)

class OrangeConeProbability:

    # @table - Dict of color versus probability P(orange cone | color), for
    # orange cone colors only.  All other colors are assumed to have probability
    # zero.
    # @prior - P(orange cone) constant default value, derived from training
    # images.  This can (should) be adjusted according to the probability of
    # orange cone pixels in the current camera view, based on location and
    # orientations of the robot, by supplying an alt_prior when reading out the
    # conditional probability.  Set this to the fraction of the image that is
    # expected to be covered by orange cone.
    # @binsize - The granularity of binning of the R, G, B values.  The values
    # are still stored as 0-255 8-bit values, but values from K * binsize to
    # (K+1) * binsize - 1 are combined and indexed by K * binsize.
    def __init__(self, prior, table, binsize):
        self.prior = float(prior)
        self.table = table
        self.binsize = int(binsize)

    # Given an pixel as a tuple (r, g, b, a), return the conditional
    # probability of an orange cone given that color.  An alternative prior
    # probability of orange cone can be supplied.  This should be set to the
    # fraction of the image that the orange cone is expected to occupy.
    # If grayscale is True, scale the 0-1 probability into the range 0-255.
    def __call__(self, rgba, alt_prior=None, grayscale=False):
        rgb = rgb_bin(rgba, self.binsize)
        prob = self.table.get(rgb, 0)
        if prob != 0 and alt_prior != None:
            prob = prob * alt_prior / self.prior
        if grayscale:
            prob = int(math.floor(prob * 255))
        return prob
        
    # Given an OrangeConeProbability object, write out a csv file representing
    # the object.  The only purpose of this is to have something readable in
    # other languages.  For saving and restoring in Python, it's better to
    # serialize out the object.
    def write_ocp_csv(self, ocp_file):
        import csv
        with open(ocp_file, "w") as ocp_handle:
            writer = csv.writer(ocp_handle)
            writer.writerow([self.prior, self.binsize])
            for rgb, prob in self.table.iteritems():
                r,g,b = rgb
                writer.writerow([r,g,b,prob])

    # Serialize out an OrangeConeProbability object.
    def write_ocp(self, ocp_file):
        import cPickle as pickle
        with open(ocp_file, "wb") as ocp_handle:
            pickle.dump(self, ocp_handle, pickle.HIGHEST_PROTOCOL)

    # Convert an RGB image to orange cone probabilities. The new image has
    # a single band.
    # @img - Image object
    # @alt_prior - Alternative prior probability for orange cone in this image.
    # @grayscale - if True, probabilities are converted to grayscale in the
    # range 0-255.
    # Returns an image in mode "F" (32-bit floating point) if grayscale is
    # False (the default), or in mode "L" (8-bit integer) if grayscale is True.
    def rgb_to_ocp_image(self, img, alt_prior=None, grayscale=False):
        img = img.convert("RGBA")
        img_pixels = img.load()
        w,h = img.size
        if grayscale:
            prob_img = Image.new("L", (w,h))
        else:
            prob_img = Image.new("F", (w,h))
        prob_pixels = prob_img.load()
        for x in range(0,w):
            for y in range(0,h):
                rgba = img_pixels[x,y]
                prob_pixels[x,y] = self(rgba, alt_prior, grayscale)
        return prob_img

# -----------------------------------------------------------------------------

# Compute the RGB index for the hash table.  We do not need a bin for every
# RGB value, and do not have enough training images to support that anyway.
# So, lump together the adjacent R, G, and B values in bins of size binsize.
def rgb_bin(rgba, binsize):
    if binsize == 1:
        rgb = rgba[0:3]
    else:
        r,g,b,_a = rgba
        # math.floor returns a float
        r = int(math.floor(r/binsize) * binsize)
        g = int(math.floor(g/binsize) * binsize)
        b = int(math.floor(b/binsize) * binsize)
        rgb = (r,g,b)
    return rgb

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
                rgb = rgb_bin(rgba, binsize)
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
        writer.writerow([listtotal[0], binsize])
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
        total, binsize = hist_reader.next()
        total = int(total)
        hist = dict()
        for row in hist_reader:
            r,g,b,count = row
            hist[(int(r),int(g),int(b))] = int(count)
        return (total, binsize, hist)

# Read in orange and background histograms, and construct
# P(orange cone | color) = P(color | orange cone) P(orange cone) / P(color)
# The orange histogram holds the raw counts for P(color | orange cone).
# The background histogram likewise holds the raw counts for
# P(color | background).
# Return an OrangeConeProbability object.
def ocp_from_histograms(orange_file, background_file):
    or_total, or_binsize, or_hist = read_histogram(orange_file)
    bg_total, bg_binsize, bg_hist = read_histogram(background_file)
    if or_binsize != bg_binsize:
        raise ValueError("Histograms must have the same binsize.")
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
    return OrangeConeProbability(p_or, p_or_per_color, or_binsize)

# Read in a saved OrangeConeProbability csv file.  Return an
# OrangeConeProbability object.
def read_ocp_csv(ocp_file):
    import csv
    with open(ocp_file, "r") as ocp_handle:
        reader = csv.reader(ocp_handle)
        prior, binsize = reader.next()
        table = dict()
        for row in reader:
            r,g,b,prob = row
            table[(int(r),int(g),int(b))] = float(prob)
    return OrangeConeProbability(prior, table, binsize)

# Read in a serialized OrangeConeProbability object.
def read_ocp(ocp_file):
    import cPickle as pickle
    with open(ocp_file, "rb") as ocp_handle:
        ocp = pickle.load(ocp_handle)
    return ocp