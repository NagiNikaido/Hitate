#include "hlightsource.h"

HLightSource::HLightSource()
{
	metadata = "HLightSource";
}

void HLightSource::setColor(HColor _color)
{
	material.color = _color;
}

HColor HLightSource::getColor()
{
	return material.color;
}

HLightSource::~HLightSource()
{
}

HIntersection HLightSource::intersect(HRay ray)
{
	return HIntersection();
}

double HLightSource::calcShade(HObjectUnion * objUnion, HIntersection intersection)
{
	return 0.;
}

HPointLightSource::HPointLightSource()
{
	metadata = "HPointLightSource";
}

HPointLightSource::HPointLightSource(HColor _color, HVec3 _pos)
{
	metadata = "HPointLightSource";
	material = HMaterial(_color);
	pos = _pos;
}

HPointLightSource::~HPointLightSource()
{
}

double HPointLightSource::calcShade(HObjectUnion * objUnion, HIntersection intersection)
{
	HRay shadeRay(intersection.pos, pos - intersection.pos);
	double dist = (pos - intersection.pos).len();
	intersection = objUnion->intersect(shadeRay);
	if (intersection.hit && intersection.dis < dist) return 0;
	return 1;
}
