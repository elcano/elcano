# Convert one RGB image into a grayscale image representing the conditional
# probability of each pixel being part of an orange cone, given its color.
#
# Example command line:  If --directory given, will cd there first.
# OrangeConeProbability object will be read in from file specified by
# --orange_cone_probability_object.  Image to convert is given by --image.
# Converted image is written as PNG regardless of input type.
#
# python convert_rgb_to_ocp_image.py \
# --directory "vision_work" \
# --image "test\\IMG_0445.JPG" \
# --orange_cone_probability_object "ocp.pkl" \
#
# To read the OrangeConeProbability object in from a csv file, instead use:
#
# --orange_cone_probability_table "ocp.csv" \

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
        python convert_rgb_to_ocp_image.py
            --directory [If provided, will cd to this before attempting to
              look up images. This is also where outputs will be written, else
              to current directory.]
            --orange_cone_probability_table [Name of file to receive the
              orange cone conditional probability table, as a csv file.]
            --orange_cone_probability_object [Name of file to receive the
              pickled OrangeConeProbability object.]
            --image [RGB image to convert.]
        """)
    parser.add_argument(
        "--directory", dest=DIR,
        help="""
If provided, will cd to this before attempting to look up images.
This is also where outputs will be written, else to current directory.
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
        "--rgb_image", dest=RGBIMG,
        help="""
RGB image file to convert.
""")
    parser.add_argument(
        "--ocp_image", dest=OCPIMG,
        help="""
Output file for converted image.
""")
    parser.add_argument(
        "--grayscale", action="store_true", dest=GRAY,
        help="""
Flag for whether to produce a grayscale image with probabilities scaled to 0-255.
If not present, the raw probabilities will be written.
""")
    parser.add_argument(
        "--alt_prior", dest=ALTPRI,
        help="""
Alternative prior probability of orange cone in the supplied image.
""")

    args = vars(parser.parse_args())

    if args[DIR]:
        os.chdir(args[DIR])
    if args[CONEOBJ]:
        ocp = read_ocp(args[CONEOBJ])
    elif args[CONEHIST]:
        ocp = read_ocp_csv([CONEHIST])
    else:
        raise ValueError("Must specify either --orange_cone_probability_table or --orange_cone_probability_object")
    alt_prior = args[ALTPRI] if args[ALTPRI] else None

    with open(args[RGBIMG], "rb") as imghandle:
        rgb_img = Image.open(imghandle)
        ocp_img = ocp.rgb_to_ocp_image(rgb_img, alt_prior=alt_prior, grayscale=args[GRAY])
        img_format = IMAGE_FORMAT_BY_MODE.get(ocp_img.mode, "PNG")
        ocp_img.save(args[OCPIMG], img_format)