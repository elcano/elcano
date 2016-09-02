#pragma once

/*!
 * \file transform.hh
 * \brief Transform on-camera rectangles into vectors in 3D space
 */

namespace elcano
{
	/*!
	 * \brief Compute the Z-coordinate from the height and an angle
	 * \param h The height of the raw image
	 * \param fovy The FOV in the y-direction of the camera
	 * \return The Z-coordinate of the image
	 *  ___________h___________
	 * /                       \
	 * |_______________________|_
	 *  \        image        /  \
	 *   \                   /   |
	 *    \                 /    |
	 *     \               /     |
	 *      \             /      |
	 *       \           /       |_ Z-coordinate
	 *        \         /        |
	 *         \  fovy /         |
	 *          \ _|_ /          |
	 *           \   /           |
	 *            \ /____________/
	 *            /_\
	 *           camera
	 */
	double
	get_z(
		uint64_t h,
		double   fovy
	);

	/*!
	 * \brief Translate from 3D space to a 2D point on the image
	 * \param cam_angle The angle that the camera is facing
	 * \param can_pos The position of the camera, in 3D space
	 * \param cone_pos The position of the cone, in 3D space
	 * \param img_size The size of the image from the camera
	 * \param sensor_size The size of the camera itself
	 * \param focal_length The focal length of the camera
	 * \return The location of the cone in 2D space
	 */
	std::tuple<double, double>
	global_to_relative(
		std::tuple<double, double, double>    cam_angle,
		std::tuple<int64_t, int64_t, int64_t> cam_pos,
		std::tuple<int64_t, int64_t, int64_t> cone_pos,
		std::tuple<uint64_t, uint64_t>        img_size,
		std::tuple<double, double>            sensor_size,
		double                                focal_length
	);

	/*!
	 * \brief Compute euclidian distance between 2 points in 2D space
	 * \param camera The first position
	 * \param locale The second position
	 * \return The distance between `camera` and `locale`
	 */
	double
	distance_2d(
		std::tuple<double, double> camera,
		std::tuple<double, double> locale
	);

	/*!
	 * \brief Compute the distance between GPS data and camera data
	 * \param cam_angle The angle that the camera is facing
	 * \param can_pos The position of the camera, in 3D space
	 * \param cone_pos The position of the cone, in 3D space
	 * \param img_size The size of the image from the camera
	 * \param sensor_size The size of the camera itself
	 * \param focal_length The focal length of the camera
	 * \param detected_loc Data from the camera
	 * \return The length of the error
	 */
	double
	compute_error(
		std::tuple<double, double, double>    cam_angle,
		std::tuple<int64_t, int64_t, int64_t> cam_pos,
		std::tuple<int64_t, int64_t, int64_t> cone_pos,
		std::tuple<uint64_t, uint64_t>        img_size,
		std::tuple<double, double>            sensor_size,
		double                                focal_length,
		std::tuple<double, double>            detected_loc
	);
}
