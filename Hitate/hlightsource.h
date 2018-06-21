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
	virtual ~HLightSource();
	virtual HVec3 getG() = 0;
	virtual HIntersection intersect(HRay ray);
	virtual double calcShade(HObjectUnion *objUnion, HIntersection intersection);
};

class HPointLightSource : public HLightSource{
private:
	HVec3 pos;
public:
	HPointLightSource();
	HPointLightSource(HColor _color, HVec3 _pos);
	virtual ~HPointLightSource();
	virtual HVec3 getG() { return pos; }
	virtual double calcShade(HObjectUnion *objUnion, HIntersection intersection);
};