#pragma once

#include "hobject.h"
#include "htriangleunion.h"

class HPoly : public HObject {
	HTriangleUnion* triangleUnion;
public:
	HPoly();
	HPoly(HVec3 pos, double size_c, std::string file_name, HMaterial _material = HMaterial());
	virtual ~HPoly();
	virtual HIntersection intersect(HRay ray);
};
