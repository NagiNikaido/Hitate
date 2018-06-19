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

HEyeCamera::HEyeCamera(HVec3 _eye, HVec3 _front, HVec3 _refUp, double _fovX, double _fovY)
{
	eye = _eye;
	front = _front;
	right = front.crossPro(_refUp);
	up = right.crossPro(front);
	fovX = _fovX;
	fovY = _fovY;
	fovScaleX = tan(fovX * (CV_PI * 0.5 / 180)) * 2;
	fovScaleY = tan(fovY * (CV_PI * 0.5 / 180)) * 2;
}

HEyeCamera::~HEyeCamera()
{
}

HRay HEyeCamera::calcRay(double x, double y)
{
	HVec3 tmp = front + right * (x - .5)*fovScaleX + up * (y - .5)*fovScaleY;
	tmp.normalize();
	return HRay(eye,tmp);
}
