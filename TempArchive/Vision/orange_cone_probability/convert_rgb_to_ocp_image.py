# Convert one RGB image, or a directory of images, into grayscale or raw
# probability images, representing the conditional probability of each pixel
# being part of an orange cone, given its color.
#
# Command line options:
# --directory
#     If given, will cd here first.
# --in_directory
#     Location of the input images.
# --out_directory
#     Output images will be written here.
# --grayscale
#     If present, output images will be integer 0-255 grayscale images.
#     Default is 32-bit floating point probabilities.
# --orange_cone_probability_object
#     OrangeConeProbability object will be read in from file specified with
#     this option.
# --orange_cone_probability_table
#     OrangeConeProbability will be constructed from this csv file.
# Converted images are written as PNG regardless of input type.
#
# python convert_rgb_to_ocp_image.py \
# --directory "C:\work_dir"
# --in_directory "input_images" \
# --out_directory "ocp_images" \
# --orange_cone_probability_object "ocp.pkl"
#
# To read the OrangeConeProbability object in from a csv file, instead use:
#
# --orange_cone_probability_table "ocp.csv"

from orange_cone_probability import *
import argparse
import os
from scipy import misc
from scipy import ndimage

parser = argparse.ArgumentParser(
    description = """
    Convert an RGB image to conditional probability of an orange cone.
    """,
    usage = """
    python convert_rgb_to_ocp_image.py
        --directory [If given, will cd here first.]
        --in_directory [Location of input images.]
        --out_directory [Where outputs will be written.]
        --orange_cone_probability_table [File containing the orange cone
          conditional probability table, as a csv file.]
        --orange_cone_probability_object [File containing the pickled
          OrangeConeProbability object.]
        --grayscale [If this option is present, will scale probabilities to
          integers in the range 0-255.]
    """)
parser.add_argument(
    "--directory", dest=DIR,
    help="""
If provided, will cd here first.
""")
parser.add_argument(
    "--in_directory", dest=INDIR,
    help="""
Location of input images.
""")
parser.add_argument(
    "--out_directory", dest=OUTDIR,
    help="""
Where to write output images.
""")
parser.add_argument(
    "--orange_cone_probability_table", dest=CONETBL,
    help="""
File containing the orange cone conditional probability table, as a csv file.
""")
parser.add_argument(
    "--orange_cone_probability_object", dest=CONEOBJ,
    help="""
File containing the pickled OrangeConeProbability object.
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

in_files = os.listdir(args[INDIR])
if args[GRAY]:
    img_ext = IMAGE_SUFFIX_BY_DTYPE.get("uint8")
else:
    img_ext = IMAGE_SUFFIX_BY_DTYPE.get("float32")

out_files = ["%s.%s" % (f.rpartition(".")[0], img_ext) for f in in_files]
in_paths = [os.path.join(args[INDIR], f) for f in in_files]
out_paths = [os.path.join(args[OUTDIR], f) for f in out_files]

for in_path, out_path in zip(in_paths, out_paths):
    rgb_img = ndimage.imread(in_path, mode="RGBA")
    ocp_img = ocp.rgb_to_ocp_image(rgb_img, alt_prior=alt_prior, grayscale=args[GRAY])
    misc.imsave(out_path, ocp_img)