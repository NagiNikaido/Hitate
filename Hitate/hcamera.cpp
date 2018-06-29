#include "hcamera.h"

HCamera::HCamera()
{
}

HCamera::~HCamera()
{
}

HPinholeCamera::HPinholeCamera()
{
}

HPinholeCamera::HPinholeCamera(HVec3 _eye, HVec3 _front, HVec3 _refUp, double _fovX, double _YXRatio)
{
	eye = _eye;
	front = _front.normalized();
	right = front.cross(_refUp).normalized();
	up = right.cross(front);
	//fovX = _fovX;
	fovScaleX = tan(_fovX * (CV_PI * 0.5 / 180)) * 2;
	fovScaleY = fovScaleX * _YXRatio;
}

HPinholeCamera::~HPinholeCamera()
{
}

HColor HPinholeCamera::calcPixel(double dx, double dy, int px, int py, std::function<HColor(HRay)> RT)
{
	double x = dx * px + dx / 2,
		   y = 1 - dy * py - dy / 2;
	HVec3 tmp = front + right * (x - .5) * fovScaleX + up * (y - .5) * fovScaleY;
	tmp.normalize();
	return RT(HRay(eye, tmp));
}

HApertureCamera::HApertureCamera()
{
}

HApertureCamera::HApertureCamera(HVec3 _oC, double _r, double _f, HVec3 _front, HVec3 _refUp, double _fovX, double _YXRatio)
{
	oC = _oC; r = _r; f = _f;
	front = _front.normalized();
	right = front.cross(_refUp).normalized();
	up = right.cross(front);
	fovScaleX = tan(_fovX * (CV_PI * 0.5 / 180)) * 2;
	fovScaleY = fovScaleX * _YXRatio;
}

HApertureCamera::~HApertureCamera()
{
}

HColor HApertureCamera::calcPixel(double dx, double dy, int px, int py, std::function<HColor(HRay)> RT)
{
	double x = dx * px + dx / 2,
		y = 1 - dy * py - dy / 2;
	HVec3 tmp = front + right * (x - .5) * fovScaleX + up * (y - .5) * fovScaleY;
	tmp.normalize();
	HVec3 pos = oC + tmp * f;
	HColor res(0, 0, 0);
	for (int i = 0; i < 128; i++) {
		do {
			x = randd(), y = randd();
		} while (x * x + y * y > 1);
		HVec3 st = oC + up * r*y + right * r*x;
		res += RT(HRay(st, pos - st));
	}
	res /= 128;
	return res;
}
