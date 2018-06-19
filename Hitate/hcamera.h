#pragma once
#include "hitate.h"
#include "hutils.h"

class HCamera {
public:
	HCamera();
	virtual ~HCamera();
	virtual HRay calcRay(double x, double y) = 0;
};

class HParallelCamera : public HCamera {
public:
	HParallelCamera();
	HParallelCamera(double _depth, HVec3 _norm)
	{
		depth = _depth, norm = _norm;
		norm.normalize();
	}
	virtual ~HParallelCamera();
	virtual HRay calcRay(double x, double y);
private:
	double depth;
	HVec3 norm;
};

class HEyeCamera : public HCamera {
public:
	HEyeCamera();
	HEyeCamera(HVec3 _eye, HVec3 _front, HVec3 _refUp, double _fovX, double _fovY);
	virtual ~HEyeCamera();
	virtual HRay calcRay(double x, double y);
private:
	HVec3 eye, front, up, right;
	double fovX, fovY, fovScaleX, fovScaleY;
};