#include "hutils.h"
#include <cmath>

double HVec3::dot(HVec3 b)
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

HVec3 HVec3::cross(HVec3 b)
{
	return HVec3(y*b.z - z * b.y, z*b.x - x * b.z, x*b.y - y * b.x);
}

HVec3& HVec3::normalize()
{
	double r = this->len();
	x /= r, y /= r, z /= r;
	return *this;
}

HVec3 HVec3::normalized()
{
	double r = this->len();
	return HVec3(x / r, y / r, z / r);
}

double HVec3::len()
{
	return std::sqrt(x*x + y * y + z * z);
}

double HVec3::len2()
{
	return x * x + y * y + z * z;
}

double & HVec3::$(int axis)
{
	if (axis == 0) return x;
	if (axis == 1) return y;
	if (axis == 2) return z;
	return x;
}

HVec3 HVec3::randomVec()
{
	double x, y, z;
	do {
		x = 2 * randd() - 1, y = 2 * randd() - 1, z = 2 * randd() - 1;
	} while (x*x + y * y + z * z > 1 || x * x + y * y + z * z < _Eps);
	return HVec3(x, y, z).normalized();
}

double randd()
{
	return double(rand()) / RAND_MAX;
}

HVec3 operator+(HVec3 a, HVec3 b)
{
	return HVec3(a.x + b.x, a.y + b.y, a.z + b.z);
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

HVec3 & operator+=(HVec3 & a, HVec3 b)
{
	a.x += b.x; a.y += b.y; a.z += b.z;
	return a;
}

HVec3 & operator-=(HVec3 & a, HVec3 b)
{
	a.x -= b.x; a.y -= b.y; a.z -= b.z;
	return a;
}

HVec3 & operator*=(HVec3 & a, double b)
{
	a.x *= b; a.y *= b; a.z *= b;
	return a;
}

HVec3 & operator/=(HVec3 & a, double b)
{
	a.x /= b; a.y /= b; a.z /= b;
	return a;
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

HColor operator/(HColor a, double b)
{
	return HColor(a.r / b, a.g / b, a.b / b);
}

HColor operator*(HColor a, HColor b)
{
	return HColor(a.r*b.r, a.g*b.g, a.b*b.b);
}

HColor & operator+=(HColor & a, HColor b)
{
	a.r += b.r; a.g += b.g; a.b += b.b;
	return a;
}

HColor & operator*=(HColor & a, double b)
{
	a.r *= b; a.g *= b; a.b *= b;
	return a;
}

HColor & operator*=(HColor & a, HColor b)
{
	a.r *= b.r; a.g *= b.g; a.b *= b.b;
	return a;
}

HColor & operator/=(HColor & a, double b)
{
	a.r /= b; a.g /= b; a.b /= b;
	return a;
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

double HColor::energy()
{
	return (r + g + b) / 3;
}

HColor HColor::saturated()
{
	return HColor(r > 1.0 ? 1.0 : r, g > 1.0 ? 1.0 : g, b > 1.0 ? 1.0 : b);
}

Pixel HColor::toPixel()
{
	Pixel res;
	res.x = cv::saturate_cast<uint8_t>(255 * b);
	res.y = cv::saturate_cast<uint8_t>(255 * g);
	res.z = cv::saturate_cast<uint8_t>(255 * r);
	return res;
}

HColor HColor::exp()
{
	return HColor(::exp(r), ::exp(g), ::exp(b));
}

HColor HColor::operator-(void)
{
	return HColor(-r, -g, -b);
}

HRay::HRay()
{
}

HRay::HRay(HVec3 _op, HVec3 _d)
{
	op = _op, d = _d.normalized();
}

HVec3 HRay::calcPoint(double t)
{
	return op + t * d;
}

HRay HRay::reflect(HVec3 hitPoint, HVec3 norm)
{
	return HRay(hitPoint, d - norm * 2 * d.dot(norm));
}

HRay HRay::refract(HVec3 hitPoint, HVec3 norm, double n)
{
	double cosi = -d.dot(norm), cost2 = 1 - n * n * (1 - cosi * cosi);
	if (cost2 > _Eps) return HRay(hitPoint, d * n + norm * (n * cosi - sqrt(cost2)));
	return reflect(hitPoint, norm);
}