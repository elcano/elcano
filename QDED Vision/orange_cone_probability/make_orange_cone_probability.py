# Run all three parts:
# Histogram the cones.
# Histogram the backgrounds.
# Compute the conditional probability of orange cone given color.
#
# Example command line, for work in a directory called "vision_work", a single
# cone image "orange_cones.png" and background images in "backgrounds":
#
# python make_orange_cone_probabilities.py \
# --directory "vision_work" \
# --orange_cone_images "orange_cones.png" \
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

    from orange_cone_probability import *
    import argparse
    import os

    parser = argparse.ArgumentParser(
        description = """
        Generate table of conditional probability of an orange cone, given
        a color.
        """,
        usage = """
        python make_orange_cone_probabilities.py
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