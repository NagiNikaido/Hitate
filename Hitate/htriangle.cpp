#include "htriangle.h"

HTriangle::HTriangle(HVec3 _a, HVec3 _b, HVec3 _c, HObject *_belongTo, HMaterial _material)
{
	metadata = "HTriangle";
	a = _a, b = _b, c = _c, material = _material;
	box.maxx = box.minx = a.x;
	box.maxy = box.miny = a.y;
	box.maxz = box.minz = a.z;
	box.update(b); box.update(c);
	norm = (b - a).cross(c - a).normalized();
	belongTo = _belongTo;
}

HIntersection HTriangle::intersect(HRay ray)
{
	double t = ray.d.dot(norm), d;
	HIntersection inter;
	if (abs(t) < _Eps) return inter;
	d = -(ray.op - a).dot(norm);
	if ((inter.dis = d / t) < _Eps) return inter;
	inter.pos = ray.calcPoint(inter.dis);
	if (inter.front = (t < -_Eps)) inter.norm = norm;
	else inter.norm = -norm;
	HVec3 db = b - a, dc = c - a, dp = inter.pos - a;
	double D = db.x*dc.y - db.y*dc.x;
	double x = (dc.y*dp.x - dc.x*dp.y) / D;
	if (x < 0 || x > 1) return inter;
	double y = (-db.y*dp.x + db.x*dp.y) / D;
	if (y < 0 || (x + y) > 1) return inter;
	inter.hit = true;
	inter.obj = belongTo;
	return inter;
}
