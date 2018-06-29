#include "hcamera.h"

HCamera::HCamera()
{
}

HCamera::~HCamera()
{
}

HParallelCamera::HParallelCamera()
{
	depth = 10.0;
	norm = HVec3(0, 0, -1);
}

HParallelCamera::~HParallelCamera()
{
}

HRay HParallelCamera::calcRay(double x, double y)
{
	return HRay(HVec3(x - .5, y - .5, depth), norm);
}

HEyeCamera::HEyeCamera()
{
}

HEyeCamera::HEyeCamera(HVec3 _eye, HVec3 _front, HVec3 _refUp, double _fovX, double _YXRatio)
{
	eye = _eye;
	front = _front.normalized();
	right = front.cross(_refUp).normalized();
	up = right.cross(front);
	//fovX = _fovX;
	fovScaleX = tan(_fovX * (CV_PI * 0.5 / 180)) * 2;
	fovScaleY = fovScaleX * _YXRatio;
}

HEyeCamera::~HEyeCamera()
{
}

HRay HEyeCamera::calcRay(double x, double y)
{
	HVec3 tmp = front + right * (x - .5) * fovScaleX + up * (y - .5) * fovScaleY;
	tmp.normalize();
	return HRay(eye,tmp);
}
