#pragma once

#include "hitate.h"
#include "hutils.h"
#include "hobject.h"
#include "hobjectunion.h"

class HLightSource : public HObject{
public:
	HLightSource();
	void setColor(HColor _color);
	HColor getColor();
	HColor calcL(HIntersection, HVec3);
	virtual ~HLightSource();
	virtual HVec3 getG() = 0;
	virtual HIntersection intersect(HRay ray);
	virtual HColor calcShade(HObjectUnion *objUnion, HIntersection intersection, int shadeQuality = 1);
};

class HPointLightSource : public HLightSource{
private:
	HVec3 pos;
public:
	HPointLightSource();
	HPointLightSource(HColor _color, HVec3 _pos);
	virtual ~HPointLightSource();
	virtual HVec3 getG() { return pos; }
	virtual HColor calcShade(HObjectUnion *objUnion, HIntersection intersection, int shadeQuality = 1);
};

class HAreaLightSource : public HLightSource {
private:
	HVec3 pos, dx, dy, norm;
public:
	HAreaLightSource();
	HAreaLightSource(HColor _color, HVec3 _pos, HVec3 _dx, HVec3 _dy);
	virtual ~HAreaLightSource();
	virtual HVec3 getG() { return pos; }
	virtual HColor calcShade(HObjectUnion *objUnion, HIntersection intersection, int shadeQuality = 1);
	virtual HIntersection intersect(HRay ray);
};