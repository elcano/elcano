import numpy as np
import math as m

# Prototype code for the image-to-world location system. Requires numpy.

# Convert a global coordinate to a relative coordinate
# (roll, pitch, yaw) = camera_angle
# (x, y, z) = camera_pos, cone_pos (global coordinates)
# (width, height) = img_size, sensor_size (sizes)
# Returns a 2x1 Matrix with the [X Y] pixel of the cone
def _global_to_relative(camera_angle,
                        camera_pos,
                        cone_pos,
                        focal_length,
                        img_size,
                        sensor_size):

    (a, b, c)                     = camera_angle
    (X1w, Y1w, Z1w)               = camera_pos
    (X2w, Y2w, Z2w)               = cone_pos
    (img_width, img_height)       = img_size
    (sensor_width, sensor_height) = sensor_size

    wP = sensor_width / img_width; hP = sensor_height / img_height
    d  = m.sqrt((X1w - X2w)**2 + (Y1w - Y2w)**2 + (Z1w - Z2w)**2)

    df = np.array([(X2w - X1w),
                   (Y2w - Y1w),
                   (Z2w - Z1w)])

    ma = np.array([(1,  0       , 0       ),
                   (0,  m.cos(a), m.sin(a)),
                   (0, -m.sin(a), m.cos(a))])

    mb = np.array([(m.cos(b), 0, -m.sin(b)),
                   (0       , 1,  0       ),
                   (m.sin(b), 0,  m.cos(b))])

    mc = np.array([( m.cos(c), m.sin(c), 0),
                   (-m.sin(c), m.cos(c), 0),
                   (0        , 0       , 1)])

    cc = np.array([(0, focal_length / (d * wP), 0),
                   (0, 0, focal_length / (d * hP))])

    im = np.array([(img_width / 2),
                   (img_height / 2)])

    converted_to_camera = mc.dot(mb).dot(ma).dot(df)
    return cc.dot(converted_to_camera) + im

# Create a vector from a 2d position within the perspective space
# x, y = the position we are calculating from
# h    = the height of the image
# fovy = the fov along the y-axis of the image
#  _  ____________________________________ 
# /  |                   ____x____        | \
# |  |                  |         *       |  \
# |  |                  y       (x, y)    |   \
# |  |                  |                 |    \
# h  |        origin -> *                 |     > fovy (in radians)
# |  |                (0, 0)              |    /  of the camera
# |  |                                    |   /
# |  |                                    |  /
# \_ |____________________________________| /
#
def _create_vector_from_2d_position(x,
                                    y,
                                    h,
                                    fovy):

    fovy /= 2; h /= 2
    z = h * m.cos(fovy) / m.sqrt(1 - m.cos(fovy)**2)

    return np.array([(x),
                     (y),
                     (z)])

# Tiny test suite for the _global_to_relative function
def _test_global_to_relative():
    print('Testing Global->Relative Transformation')
    print(_global_to_relative((0, 0, 0), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0, 0), (0, 0, 1), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0.3, 0), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0, 0.3), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))

# Tiny test suite for the _create_vector_from_2d_position function
def _test_create_vector_from_2d_position():
    print('Testing 2D->3D Transformation')
    print(_create_vector_from_2d_position(100, 100, 480, 90 * m.pi / 180))
    print(_create_vector_from_2d_position(100, 100, 480, 160 * m.pi / 180))
    print(_create_vector_from_2d_position(-100, 100, 480, 90 * m.pi / 180))

if __name__ == "__main__":
    _test_global_to_relative()
    _test_create_vector_from_2d_position()
