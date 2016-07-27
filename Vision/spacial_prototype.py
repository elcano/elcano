import numpy as np
import math as m

# Prototype code for the image-to-world location system. Requires numpy.

# Generate a frustum matrix for a perspective projection
# fov = field of view (y axis)
# aspect = ratio of the image
# near, far = the distance bounds
def _perspective(fov,
                 aspect,
                 near,
                 far):

    assert(fov > 0 and aspect != 0)
    h = 1 / m.tan(fov / 2.0)
    w = h / aspect
    q = far / (near - far)

    return np.array([(w, 0, 0      , 0),
                     (0, h, 0      , 0),
                     (0, 0, q      , 1),
                     (0, 0, -q*near, 0)])

# Find the pseudo-inverse of a perspective matrix
# See _perspective for more information on fov, aspect, near, and far
def _inverse_perspective(fov,
                         aspect,
                         near,
                         far):

    return np.linalg.pinv(_perspective(fov, aspect, near, far))

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

# Tiny test suite for the _perspective function
def _test_perspective():
    print('Testing Perspective Matrix Generation')
    print(_perspective(0.5, 1   , 1, 50))
    print(_perspective(1.2, 0.75, 1, 50))

# Tiny test suite for the _inverse_perspective function
def _test_inverse_perspective():
    print('Testing Inverted Perspective Matrix Generation')
    print(_inverse_perspective(0.5, 1   , 1, 50))
    print(_inverse_perspective(1.2, 0.75, 1, 50))

# Tiny test suite for the _global_to_relative function
def _test_global_to_relative():
    print('Testing Global->Relative Transformation')
    print(_global_to_relative((0, 0, 0), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0, 0), (0, 0, 1), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0.3, 0), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))
    print(_global_to_relative((0, 0, 0.3), (0, 0, 0), (10, 2, 0), 0.01, (320, 240), (0.05, 0.05)))

if __name__ == "__main__":
    _test_perspective()
    _test_inverse_perspective()
    _test_global_to_relative()
