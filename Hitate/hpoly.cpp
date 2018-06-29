#include "hpoly.h"

HPoly::HPoly()
{
	metadata = "HPoly";
}

HPoly::HPoly(HVec3 pos, double size_c, std::string file_name, HMaterial _material)
{
	metadata = "HPoly";
	material = _material;
	triangleUnion = new HTriangleUnion(this, pos, size_c, file_name);
}

HPoly::~HPoly()
{
	delete triangleUnion;
}

HIntersection HPoly::intersect(HRay ray)
{
	return triangleUnion->intersect(ray);
}


