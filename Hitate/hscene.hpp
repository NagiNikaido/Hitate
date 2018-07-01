#pragma once
#include "hitate.h"
#include "hobject.h"
#include "hcamera.h"
#include "hobjectunion.h"
#include "hlightsource.h"
#include "hpoly.h"
#include <vector>

class HScene {
protected:
	HCamera *camera;
	HObjectVector *objUnion;
	std::vector<HLightSource *> lightSourceList;
	HColor background;
public:
	HScene()
	{
		camera = nullptr;
		objUnion = new HObjectVector;
		background = HColor(0, 0, 0);
	}
	virtual ~HScene()
	{
		if (camera) delete camera;
		delete objUnion;
		for (auto lsrc : lightSourceList)
			delete lsrc;
	}
	void setCamera(HCamera *_camera)
	{
		if (camera) delete camera;
		camera = _camera;
	}
	void setBackground(HColor _background)
	{
		background = _background;
	}
	void addLightSource(HLightSource *_lightSource)
	{
		lightSourceList.push_back(_lightSource);
	}
	void clearLightSource()
	{
		lightSourceList.clear();
	}
	void addObj(HObject *_obj)
	{
		objUnion->addObj(_obj);
	}
	void clearObj()
	{
		objUnion->clearObj();
	}
	HIntersection lightIntersect(HRay ray) {
		HIntersection inter;
		for (auto light : lightSourceList)
			inter.update(light->intersect(ray));
		return inter;
	}
};