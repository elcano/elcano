#pragma once

#include "types.hh"

/* Transform on-camera rectangles into vectors in 3D space */

namespace elcano
{
	double
    compute_error(
        std::tuple<double, double, double>       cam_angle,
        std::tuple<uint64_t, uint64_t, uint64_t> cam_pos,
        std::tuple<uint64_t, uint64_t, uint64_t> cone_pos,
        std::tuple<uint64_t, uint64_t>           img_size,
        std::tuple<uint64_t, uint64_t>           sensor_size,
        uint64_t                                 focal_length,
        std::tuple<double, double>               detected_loc
    );
}
