#pragma once

#include "hitate.h"
#include "hutils.h"

class HObject {
public:
	HObject();
	virtual ~HObject();
	virtual HIntersection intersect(HRay) = 0;
};

class HSphere : public HObject {
public:
	HVec3 o;
	double r;
	HSphere();
	HSphere(HVec3, double);
	virtual ~HSphere();
	virtual HIntersection intersect(HRay);
};

class HPlane : public HObject {
public:
	HVec3 p, norm;
	HPlane();
	HPlane(HVec3, HVec3);
	virtual ~HPlane();
	virtual HIntersection intersect(HRay);
};