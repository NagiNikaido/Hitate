#pragma once

#include "hitate.h"

struct HVec3 {
	double x, y, z;
	HVec3();
	HVec3(double _x, double _y, double _z);
	HVec3 operator-(void);
	double dotPro(HVec3 a);
	HVec3 crossPro(HVec3 a);
	void normalize();
	double len();
	double len2();
};

HVec3 operator+ (HVec3 a, HVec3 b);
HVec3 operator- (HVec3 a, HVec3 b);
HVec3 operator* (double a, HVec3 b);
HVec3 operator* (HVec3 a, double b);
HVec3 operator/ (HVec3 a, double b);

struct HColor {
	double r, g, b;
	HColor();
	HColor(double _r, double _g, double _b);
	void saturate();
	Pixel toPixel();
};

HColor operator+ (HColor a, HColor b);
HColor operator* (HColor a, double b);
HColor operator* (double a, HColor b);

struct HRay {
	HVec3 op, d;
	HRay();
	HRay(HVec3 _op, HVec3 _d);
	HVec3 calcPoint(double t);
};

struct HIntersection {
	HVec3 pos, norm;
	bool isHit;
	double dis;
	HIntersection();
	HIntersection(HVec3 _pos, HVec3 _norm, bool _isHit, double _dis);
	void update(HIntersection tr);
};