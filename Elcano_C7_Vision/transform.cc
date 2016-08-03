#include "transform.hh"
#include <tgmath.h>
#include <tuple>
#include <opencv2/core/mat.hpp>

/* Transform on-camera rectangles into vectors in 3D space */

namespace elcano
{
	static double
    get_z(
        uint64_t h,
        double fovy
    ) {
		h /= 2;
        double cf = cos(fovy / 2);
        return static_cast<double>(h) * cf / sqrt(1 - cf * cf);
	}
    
    static std::tuple<double, double>
    global_to_relative(
        std::tuple<double, double, double>       cam_angle,
        std::tuple<uint64_t, uint64_t, uint64_t> cam_pos,
        std::tuple<uint64_t, uint64_t, uint64_t> cone_pos,
        std::tuple<uint64_t, uint64_t>           img_size,
        std::tuple<uint64_t, uint64_t>           sensor_size,
        uint64_t                                 focal_length
    ) {
        cv::Mat df = (Mat_<double>(3, 1) <<
            std::get<0>(cone_pos) - std::get<0>(cam_pos),
            std::get<1>(cone_pos) - std::get<1>(cam_pos),
            std::get<2>(cone_pos) - std::get<2>(cam_pos)
        );
        
        cv::Mat ma = (Mat_<double>(3, 3) <<
            1,  0,                           0,
            0,  cos(std::get<0>(cam_angle)), sin(std::get<0>(cam_angle)),
            0, -sin(std::get<0>(cam_angle)), cos(std::get<0>(cam_angle))
        );
        
        cv::Mat mb = (Mat_<double>(3, 3) <<
            cos(std::get<1>(cam_angle)), 0, -sin(std::get<1>(cam_angle)),
            0,                           1,  0,
            sin(std::get<1>(cam_angle)), 0,  cos(std::get<1>(cam_angle))
        );
        
        cv::Mat mc = (Mat_<double>(3, 3) <<
             cos(std::get<2>(cam_angle)), sin(std::get<2>(cam_angle)), 0,
            -sin(std::get<2>(cam_angle)), cos(std::get<2>(cam_angle)), 0,
             0,                           0,                           1
        );
        
        double d = (df.t() * df).at(0, 0);
        
        cv::Mat cc = (Mat_<double>(2, 3) <<
            0, focal_length * std::get<0>(img_size) / (d * std::get<0>(sensor_size)), 0,
            0, 0, focal_length * std::get<1>(img_size) / (d * std::get<1>(sensor_size))
        );
        
        cv::Mat im = (Mat_<double>(2, 1) <<
            static_cast<double>(std::get<0>(img_width)) / 2,
            static_cast<double>(std::get<1>(img_width)) / 2
        );
        
        cv::Mat res = (cc * mc * mb * ma * df) + im;
        return std::tuple<double, double>(res.at(0, 0), res.at(1, 0));
    }
    
    static double
    distance_2d(
        std::tuple<double, double> camera,
        std::tuple<double, double> locale
    ) {
        double dx = std::get<0>(camera) - std::get<0>(locale);
        double dy = std::get<1>(camera) - std::get<1>(locale);
        return sqrt(dx * dx + dy * dy);
    }
    
    double
    compute_error(
        std::tuple<double, double, double>       cam_angle,
        std::tuple<uint64_t, uint64_t, uint64_t> cam_pos,
        std::tuple<uint64_t, uint64_t, uint64_t> cone_pos,
        std::tuple<uint64_t, uint64_t>           img_size,
        std::tuple<uint64_t, uint64_t>           sensor_size,
        uint64_t                                 focal_length,
        std::tuple<double, double>               camera_detected
    ) {
        std::tuple<double, double> locale_detected = global_to_relative(
            cam_angle, cam_pos, cone_pos, img_size, sensor_size, focal_length
        );
        
        return distance_2d(locale_detected, camera_detected);
    }
}
