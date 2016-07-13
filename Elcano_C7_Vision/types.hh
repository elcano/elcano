#pragma once

#include <vector>
#include <stdint.h>

/* Types used internally within the vision program (HEADER ONLY) */

namespace elcano
{
	struct Rectangle
	{
		double x, y, w, h;
	};
	
	union Vector3
	{
		struct { uint64_t x, y, z; };
		uint64_t as_longs[3];
		uint8_t as_bytes[24];
	};
	
	using Rectangles = std::vector<Rectangle>;
	using Vectors3 = std::vector<Vector3>;
}
