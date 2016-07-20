#pragma once

#include "types.hh"

/* Transform on-camera rectangles into vectors in 3D space */

namespace elcano
{
	Vector3 transform(Rectangle);
	Vectors3 transform(Rectangles);
}
