#pragma once

#include "hitate.h"
#include "hutils.h"
#include "hmaterial.h"
#include <string>


struct HIntersection;

class HObject {
protected:
	std::string metadata;
	HMaterial material;
public:
	HObject();
	std::string getMetadata();
	HMaterial getMaterial();
	void setMaterial(HMaterial _material);
	virtual ~HObject();
	virtual HIntersection intersect(HRay) = 0;
};

struct HIntersection {
	HVec3 pos, norm;
	bool hit, front;
	double dis;
	HObject *obj;
	HIntersection();
	HIntersection(HVec3 _pos, HVec3 _norm, bool _hit, bool _front, double _dis, HObject * _obj);
	void update(HIntersection tr);
};

class HSphere : public HObject {
public:
	HVec3 o;
	double r;
	HSphere();
	HSphere(HVec3 _o, double _r, HMaterial _material = HMaterial());
	virtual ~HSphere();
	virtual HIntersection intersect(HRay);
};

class HPlane : public HObject {
public:
	HVec3 p, norm;
	HPlane();
	HPlane(HVec3 _p, HVec3 _norm, HMaterial _material =HMaterial());
	virtual ~HPlane();
	virtual HIntersection intersect(HRay);
};