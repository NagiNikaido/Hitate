#pragma once

#include "hobject.h"
#include <algorithm>

struct HBox {
	double minx, miny, minz;
	double maxx, maxy, maxz;
	void update(HVec3 _p) {
		minx = std::min(minx, _p.x);
		miny = std::min(miny, _p.y);
		minz = std::min(minz, _p.z);
		maxx = std::max(maxx, _p.x);
		maxy = std::max(maxy, _p.y);
		maxz = std::max(maxz, _p.z);
	}
	void update(HBox box) {
		minx = std::min(minx, box.minx);
		miny = std::min(miny, box.miny);
		minz = std::min(minz, box.minz);
		maxx = std::max(maxx, box.maxx);
		maxy = std::max(maxy, box.maxy);
		maxz = std::max(maxz, box.maxz);
	}
	double &min$(int p) {
		if (p == 0) return minx;
		if (p == 1) return miny;
		if (p == 2) return minz;
		return minx;
	}
	double &max$(int p) {
		if (p == 0) return maxx;
		if (p == 1) return maxy;
		if (p == 2) return maxz;
		return maxx;
	}
	double area() {
		double a = maxx - minx, b = maxy - miny, c = maxz - minz;
		return (a*b + b * c + c * a) * 2;
	}
	bool in(HVec3 p) {
		for (int i = 0; i < 3; i++)
			if (p.$(i) <= min$(i) - _Eps || p.$(i) >= max$(i) + _Eps) return false;
		return true;
	}
	bool intersect(HRay ray, double &d) {
		double res = -1;
		for (int i = 0; i < 3; i++) if(abs(ray.d.$(i)) > _Eps){
			double p;
			if ((p = (min$(i) - ray.op.$(i)) / ray.d.$(i)) >= _Eps) {
				if (in(ray.calcPoint(p))) if (res < -_Eps || p < res) res = p;
			}
			if ((p = (max$(i) - ray.op.$(i)) / ray.d.$(i)) >= _Eps) {
				if (in(ray.calcPoint(p))) if (res < -_Eps || p < res) res = p;
			}
		}
		return res < _Eps ? false : (d=res, true);
	}
};
class HTriangle : public HObject {
public:
	HVec3 a, b, c, norm;
	HBox box;
	HObject *belongTo;
	HTriangle() { metadata = "HTriangle"; }
	HTriangle(HVec3 _a, HVec3 _b, HVec3 _c, HObject *_belongTo = nullptr, HMaterial _material=HMaterial());
	virtual ~HTriangle() {}
	virtual HIntersection intersect(HRay ray);
};