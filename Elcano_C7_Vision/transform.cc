#include "transform.hh"

/* Transform on-camera rectangles into vectors in 3D space */

namespace elcano
{
	Vector3 transform(Rectangle in)
	{
		Vector3 out;
		
		return out;
	}
	
	Vectors3 transform(Rectangles in)
	{
		Vectors3 out;
		
		for (Rectangle r : in)
			out.push_back(transform(r));
		
		return out;
	}
}
