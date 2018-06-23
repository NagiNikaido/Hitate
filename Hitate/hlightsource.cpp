#include "hlightsource.h"
#include <iostream>

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

HColor HLightSource::calcL(HIntersection inter, HVec3 shader)
{
	HColor res(0, 0, 0);
	HVec3 h = shader.normalized() - inter.ray.d;
	HMaterial m = inter.obj->getMaterial();
	double r1 = shader.normalized().dotPro(inter.norm);
	double r2 = h.dotPro(inter.norm) / sqrt(2);
	double d = shader.len2();
	if (r1 > _Eps && m.diff > _Eps) {
		res += m.color * getColor() * m.diff * r1 / d;
	}

	if (r2 > _Eps && m.spec > _Eps) {
		res += m.color * getColor() * m.spec * pow(r2, _Spec_Power) / d;
	}
	//std::cout << res.r << ' ' << res.g << ' ' << res.b << std::endl;
	return res;
}

HLightSource::~HLightSource()
{
}

HIntersection HLightSource::intersect(HRay ray)
{
	return HIntersection();
}

HColor HLightSource::calcShade(HObjectUnion * objUnion, HIntersection intersection, int shadQuality)
{
	return HColor(0, 0, 0);
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

HColor HPointLightSource::calcShade(HObjectUnion * objUnion, HIntersection intersection, int shadeQuality)
{
	HRay shadeRay(intersection.pos, pos - intersection.pos);
	double dist = (pos - intersection.pos).len();
	HIntersection inter = objUnion->intersect(shadeRay);
	if (inter.hit && inter.dis < dist) return HColor(0, 0, 0);
	return calcL(intersection, pos - intersection.pos);
}

HAreaLightSource::HAreaLightSource()
{
}

HAreaLightSource::HAreaLightSource(HColor _color, HVec3 _pos, HVec3 _dx, HVec3 _dy)
{
	metadata = "HAreaLightSource";
	material = HMaterial(_color);
	pos = _pos; dx = _dx; dy = _dy;
	norm = dx.crossPro(dy).normalized();
}

HAreaLightSource::~HAreaLightSource()
{
}

HColor HAreaLightSource::calcShade(HObjectUnion * objUnion, HIntersection intersection, int shadeQuality)
{
	HColor res(0, 0, 0);
	for (int i = -2; i < 2; i++) for (int j = -2; j < 2; j++) {
		for (int k = 0; k < shadeQuality; k++) {
			HVec3 tp = pos + dx * (randd() + i) / 2 + dy * (randd() + j) / 2;
			HIntersection inter = objUnion->intersect(HRay(intersection.pos, tp - intersection.pos));
			if (inter.hit && inter.dis < (tp - intersection.pos).len()) continue;
			res += calcL(intersection, tp - intersection.pos);
		}
	}
	return res / 16.0 / shadeQuality;
}

HIntersection HAreaLightSource::intersect(HRay ray)
{
	double d = ray.d.dotPro(norm);
	if (abs(d) < _Eps) return HIntersection();
	double l = (norm * pos.dotPro(norm) - ray.op).dotPro(norm) / d;
	if (l < _Eps) return HIntersection();
	HVec3 t = (ray.op + ray.d * l) - pos;
	if (abs(dx.dotPro(t)) > dx.len2()) return HIntersection();
	if (abs(dy.dotPro(t)) > dy.len2()) return HIntersection();
	return HIntersection(t + pos, norm, ray, true, true, l, this);
}
