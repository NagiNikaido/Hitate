#include "hobject.h"

HObject::HObject()
{
	metadata = "HObject";
}

std::string HObject::getMetadata()
{
	return metadata;
}

HMaterial HObject::getMaterial()
{
	return material;
}

void HObject::setMaterial(HMaterial _material)
{
	material = _material;
}

HObject::~HObject()
{
}

HIntersection::HIntersection()
{
	hit = false;
	obj = nullptr;
}

HIntersection::HIntersection(HVec3 _pos, HVec3 _norm, HRay _ray,bool _hit, bool _front, double _dis, HObject *_obj)
{
	pos = _pos, norm = _norm, ray = _ray, hit = _hit, front = _front, dis = _dis, obj = _obj;
}

void HIntersection::update(HIntersection tr)
{
	if (!hit && tr.hit || tr.hit && tr.dis < dis) {
		*this = tr;
	}
}

HSphere::HSphere()
{
	metadata = "HSphere";
}

HSphere::HSphere(HVec3 _o, double _r, HMaterial _material)
{
	metadata = "HSphere";
	o = _o, r = _r, material = _material;
}

HSphere::~HSphere()
{
}

HIntersection HSphere::intersect(HRay ray)
{
	double b = ray.d.dotPro(o - ray.op), c = (ray.op - o).len2();
	double D = b * b - c + r * r;
	HIntersection res;
	if (D>_Eps) {
		D = sqrt(D);
		double x1 = b - D, x2 = b + D;
		if (x2 < _Eps) res.hit = false;
		else {
			if (x1 > _Eps) {
				res.dis = x1;
				res.pos = ray.calcPoint(res.dis);
				res.front = true;
				res.norm = (res.pos - o) / r;
			}
			else {
				res.dis = x2;
				res.pos = ray.calcPoint(res.dis);
				res.front = false;
				res.norm = (o - res.pos) / r;
			}
			res.hit = true;
			res.obj = this;
			res.ray = ray;
		}
	}
	else {
		res.hit = false;
	}
	return res;
}

HPlane::HPlane()
{
	metadata = "HPlane";
}

HPlane::HPlane(HVec3 _p, HVec3 _norm, HMaterial _material)
{
	metadata = "HPlane";
	p = _p, norm = _norm, material = _material;
	norm.normalize();
}

HPlane::~HPlane()
{
}

HIntersection HPlane::intersect(HRay ray)
{
	double a = norm.dotPro(ray.d), b = norm.dotPro(ray.op - p);
	double l;
	if (abs(b) < _Eps || abs(a) < _Eps || (l=-b/a)<_Eps) {
		return HIntersection();
	}
	return HIntersection(ray.calcPoint(l), a < 0 ? norm : -norm, ray, true, a < 0, l, this);
}

