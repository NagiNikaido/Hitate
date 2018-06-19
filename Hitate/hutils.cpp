#include "hutils.h"
#include <cmath>

double HVec3::dotPro(HVec3 b)
{
	return x * b.x + y * b.y + z * b.z;
}

HVec3::HVec3()
{
}

HVec3::HVec3(double _x, double _y, double _z)
{
	x = _x, y = _y, z = _z;
}

HVec3 HVec3::operator-(void)
{
	return HVec3(-x, -y, -z);
}

HVec3 HVec3::crossPro(HVec3 b)
{
	return HVec3(y*b.z - z * b.y, z*b.x - x * b.z, x*b.y - y * b.x);
}

void HVec3::normalize()
{
	double r = this->len();
	x /= r, y /= r, z /= r;
}

double HVec3::len()
{
	return std::sqrt(x*x + y * y + z * z);
}

double HVec3::len2()
{
	return x * x + y * y + z * z;
}

HVec3 operator+(HVec3 a, HVec3 b)
{
	return HVec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

HVec3 operator-(HVec3 a, HVec3 b)
{
	return HVec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

HVec3 operator*(double a, HVec3 b)
{
	return HVec3(a*b.x, a*b.y, a*b.z);
}

HVec3 operator*(HVec3 a, double b)
{
	return HVec3(a.x*b, a.y*b, a.z*b);
}

HVec3 operator/(HVec3 a, double b)
{
	return HVec3(a.x / b, a.y / b, a.z / b);
}

HColor operator+(HColor a, HColor b)
{
	return HColor(a.r + b.r, a.g + b.g, a.b + b.b);
}

HColor operator*(HColor a, double b)
{
	return HColor(a.r*b, a.g*b, a.b*b);
}

HColor operator*(double a, HColor b)
{
	return HColor(a*b.r, a*b.g, a*b.b);
}

HColor::HColor()
{
}

HColor::HColor(double _r, double _g, double _b)
{
	r = _r, g = _g, b = _b;
}

void HColor::saturate()
{
	r = r > 1.0 ? 1.0 : r;
	g = g > 1.0 ? 1.0 : g;
	b = b > 1.0 ? 1.0 : b;
}

Pixel HColor::toPixel()
{
	Pixel res;
	res.x = cv::saturate_cast<uint8_t>(255 * b);
	res.y = cv::saturate_cast<uint8_t>(255 * g);
	res.z = cv::saturate_cast<uint8_t>(255 * r);
	return res;
}

HRay::HRay()
{
}

HRay::HRay(HVec3 _op, HVec3 _d)
{
	op = _op, d = _d;
}

HVec3 HRay::calcPoint(double t)
{
	return op+t*d;
}

HIntersection::HIntersection()
{
}

HIntersection::HIntersection(HVec3 _pos, HVec3 _norm, bool _isHit, double _dis)
{
	pos = _pos, norm = _norm, isHit = _isHit, dis = _dis;
}

void HIntersection::update(HIntersection tr)
{
	if (!isHit && tr.isHit || tr.isHit && tr.dis < dis) {
		*this = tr;
	}
}
