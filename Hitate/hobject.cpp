#include "hobject.h"

HObject::HObject()
{
}

HObject::~HObject()
{
}

HSphere::HSphere()
{
}

HSphere::HSphere(HVec3 _o, double _r)
{
	o = _o, r = _r;
}

HSphere::~HSphere()
{
}

HIntersection HSphere::intersect(HRay ray)
{
	double a = ray.d.len2(), b = ray.d.dotPro(ray.op - o), c = (ray.op - o).len2();
	double D = b * b - a * (c - r * r);
	HIntersection res;
	if(D>=-_Eps) {
		res.dis = (-b - sqrt(abs(D))) / a;
		res.pos = ray.calcPoint(res.dis);
		res.norm = (res.pos - o) / r;
		res.isHit = true;
	}
	else {
		res.isHit = false;
	}
	return res;
}

HPlane::HPlane()
{
}

HPlane::HPlane(HVec3 _p, HVec3 _norm)
{
	p = _p, norm = _norm;
}

HPlane::~HPlane()
{
}

HIntersection HPlane::intersect(HRay ray)
{
	double a = norm.dotPro(ray.d), b = norm.dotPro(ray.op - p);
	if (abs(a) < _Eps) {
		return HIntersection(ray.op, norm, true, 0.0);
	}
	else if (abs(b) < _Eps) {
		return HIntersection();
	}
	else {
		HIntersection res;
		res.dis = -b / a;
		res.pos = ray.calcPoint(res.dis);
		res.norm = norm;
		res.isHit = true;
		return res;
	}
}

