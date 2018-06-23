#pragma once

#include "hitate.h"

double randd();

struct HVec3 {
	double x, y, z;
	HVec3();
	HVec3(double _x, double _y, double _z);
	HVec3 operator-(void);
	double dotPro(HVec3 a);
	HVec3 crossPro(HVec3 a);
	HVec3& normalize();
	HVec3 normalized();
	double len();
	double len2();
	static HVec3 randomVec();
};

HVec3 operator+ (HVec3 a, HVec3 b);
HVec3 operator- (HVec3 a, HVec3 b);
HVec3 operator* (double a, HVec3 b);
HVec3 operator* (HVec3 a, double b);
HVec3 operator/ (HVec3 a, double b);
HVec3& operator+=(HVec3 &a, HVec3 b);
HVec3& operator-=(HVec3 &a, HVec3 b);
HVec3& operator*=(HVec3 &a, double b);
HVec3& operator/=(HVec3 &a, double b);

struct HColor {
	double r, g, b;
	HColor();
	HColor(double _r, double _g, double _b);
	void saturate();
	Pixel toPixel();
	HColor exp();
	HColor operator- (void);
};

HColor operator+ (HColor a, HColor b);
HColor operator* (HColor a, double b);
HColor operator* (double a, HColor b);
HColor operator/ (HColor a, double b); 
HColor operator* (HColor a, HColor b);
HColor& operator+= (HColor &a, HColor b);
HColor& operator*= (HColor &a, double b);
HColor& operator*= (HColor &a, HColor b);
HColor& operator/= (HColor &a, double b);

struct HRay {
	HVec3 op, d;
	HRay();
	HRay(HVec3 _op, HVec3 _d);
	HVec3 calcPoint(double t);
	HRay reflect(HVec3 hitPoint, HVec3 norm);
	HRay refract(HVec3 hitPoint, HVec3 norm, double n);
};
