#pragma once
#include "hitate.h"
#include "hobject.h"
#include "hcamera.h"
#include "hobjectunion.h"
#include "hlightsource.h"
#include "hpoly.h"
#include <vector>

template<class Object_Union_T = HObjectVector>
class HScene {
protected:
	HCamera *camera;
	Object_Union_T *objUnion;
	std::vector<HLightSource *> lightSourceList;
	HColor background;
	int shadeQuality, drflQuality;
public:
	HScene()
	{
		camera = nullptr;
		objUnion = new Object_Union_T;
		background = HColor(0, 0, 0);
		shadeQuality = 1;  drflQuality = 16;
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
	void setShadeQuality(int _shadeQuality)
	{
		shadeQuality = _shadeQuality;
	}
	void setDrflQuality(int _drflQuality)
	{
		drflQuality = _drflQuality;
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
	virtual bool render(cv::Mat & canvas, int w, int h)
	{
		if (!camera || objUnion->empty()) return false;
		objUnion->buildStructure();
		double dx = 1. / w, dy = 1. / h;
		double sx = dx / 2, sy = dy / 2;
		canvas.forEach<Pixel>([&](Pixel& pixel, const int pos[]) -> void {
			HRay ray = camera->calcRay(sx + dx * pos[1], 1 - sy - dy * pos[0]);
			
			HIntersection res = objUnion->intersect(ray);

			HColor rc;
			if (!res.hit) rc = background;
			else rc = HColor(1. / res.dis, 1. / res.dis, 1. / res.dis);

			pixel = rc.toPixel();
			
		});
		return true;
	}
};