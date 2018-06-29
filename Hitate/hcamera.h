#pragma once
#include "hitate.h"
#include "hutils.h"

class HCamera {
public:
	HCamera();
	virtual ~HCamera();
	virtual HColor calcPixel(double dx, double dy, int px, int py, std::function<HColor(HRay)> RT) = 0;
};

class HPinholeCamera : public HCamera {
public:
	HPinholeCamera();
	HPinholeCamera(HVec3 _eye, HVec3 _front, HVec3 _refUp, double _fovX, double _YXRatio);
	virtual ~HPinholeCamera();
	virtual HColor calcPixel(double dx, double dy, int px, int py, std::function<HColor(HRay)> RT);
private:
	HVec3 eye, front, up, right;
	//double fovX, fovY, fovScaleX, fovScaleY;
	double fovScaleX, fovScaleY;
};

class HApertureCamera : public HCamera {
public:
	HApertureCamera();
	HApertureCamera(HVec3 _oC, double _r, double _f, HVec3 _front, HVec3 _refUp, double _fovX, double _YXRatio);
	virtual ~HApertureCamera();
	virtual HColor calcPixel(double dx, double dy, int px, int py, std::function<HColor(HRay)> RT);
private:
	HVec3 oC, front, up, right;
	double r, u, f;
	double fovScaleX, fovScaleY;
};