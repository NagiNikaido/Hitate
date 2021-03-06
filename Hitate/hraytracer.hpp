#pragma once

#include "hscene.hpp"
#include "hphoton.h"
#include <mutex>

class HRaytracer : public HScene {
	HPhotonMap *photonMap;
	int max_store_photons, max_emit_photons, max_sample_photons;
	double max_sample_dist;
	int shadeQuality, drflQuality, NxSSAA;
	HColor calcDiff(HIntersection inter)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		HColor res = material.color * this->background * material.diff
			+ material.color * photonMap->calcL(inter, max_sample_dist, max_sample_photons) * material.diff;
		HRay ray = inter.ray;
		for (auto light : this->lightSourceList) 
			res += light->calcShade(this->objUnion, inter, this->shadeQuality);
		return res;
	}
	HColor calcRefl(HIntersection inter, double weight, int depth)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		HRay ray = inter.ray.reflect(inter.pos, inter.norm);
		
		return material.color * material.refl * rayTrace(ray, weight*material.refl, depth - 1);
	}
	HColor calcRefr(HIntersection inter, double weight, int depth)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		HRay ray = inter.ray.refract(inter.pos, inter.norm, inter.front ? 1 / material.rindex : material.rindex);

		HColor rr = rayTrace(ray, weight*material.refr, depth + 1);
		if (inter.front) return rr * material.refr;
		HColor tr = (material.absor * -inter.dis).exp();
		return rr * tr * material.refr;
	}
	HColor rayTrace(HRay ray, double weight, int depth)
	{
		if (depth>=_Max_Raytrace_Depth || weight<_Eps) return HColor(0, 0, 0);
		HIntersection light, object;
		HColor res(0, 0, 0);
		light = this->lightIntersect(ray);
		object = this->objUnion->intersect(ray);
		if (light.hit && (!object.hit || object.dis > light.dis)) {
			return light.obj->getMaterial().color.saturated();
		}
		if (object.hit) {
			HObject *obj = object.obj;
			if (obj->getMaterial().diff > _Eps || obj->getMaterial().spec > _Eps)
				res += calcDiff(object);
			if (obj->getMaterial().refl > _Eps)
				res += calcRefl(object, weight, depth);
			if (obj->getMaterial().refr > _Eps)
				res += calcRefr(object, weight, depth);
		}
		return res;
	}

	bool photonDiff(HPhoton ph, HIntersection inter, int depth, double &p)
	{
		HMaterial M = inter.obj->getMaterial();
		double de = (M.diff + M.spec) *M.color.energy();
		if (de <= randd()*p) {
			p -= de; return false;
		}
		ph.ray.d = HVec3::randomVec();
		if (ph.ray.d.dot(inter.norm) < _Eps) ph.ray.d = -ph.ray.d;
		ph.color = ph.color * M.color / M.color.energy();
		photonTrace(ph, depth + 1);
		return true;
	}
	bool photonRefl(HPhoton ph, HIntersection inter, int depth, double &p)
	{
		HMaterial M = inter.obj->getMaterial();
		double de = M.refl * M.color.energy();
		if (de <= randd()*p) {
			p -= de; return false;
		}
		ph.ray = inter.ray.reflect(inter.pos, inter.norm);
		ph.color = ph.color * M.color / M.color.energy();
		photonTrace(ph, depth + 1);
		return true;
	}
	bool photonRefr(HPhoton ph, HIntersection inter, int depth, double &p)
	{
		HMaterial M = inter.obj->getMaterial();
		double de = M.refr;
		if (!inter.front) {
			HColor ab = (M.absor * -inter.dis).exp();
			ph.color = ph.color * ab / ab.energy();
			de *= ab.energy();
		}
		if (de <= randd()*p) {
			p -= de; return false;
		}
		ph.ray = inter.ray.refract(inter.pos, inter.norm, inter.front ? 1 / M.rindex : M.rindex);
		photonTrace(ph, depth + 1);
		return true;
	}
	void photonTrace(HPhoton ph, int depth)
	{
		if (depth >= _Max_Photontrace_Depth) return;
		HIntersection object = this->objUnion->intersect(ph.ray);
		if (object.hit) {
			HMaterial M = object.obj->getMaterial();
			ph.ray.op = object.pos;
			if (object.obj->getMaterial().diff > _Eps && depth) photonMap->insertPhoton(ph);
			double p = 1;
			if (photonDiff(ph, object, depth, p)) return;
			if (photonRefl(ph, object, depth, p)) return;
			if (photonRefr(ph, object, depth, p)) return;
		}
	}
	void buildPhotonMap()
	{
		photonMap = new HPhotonMap(max_store_photons, max_emit_photons);
		double sumE = 0;
		for (auto light : this->lightSourceList)
			sumE += light->getColor().energy();
		double E = sumE / max_emit_photons;
		int count = 0;
		for (auto light : this->lightSourceList) {
			for (double cE = light->getColor().energy(); cE >= E; cE -= E) {
				HPhoton photon = light->emitPhoton();
				photon.color *= sumE;
				photonTrace(photon, 0);
				if ((++count) % 1000000 == 0) {
					printf("A total of %d photons have been emitted.\n", count);
					printf("           %d photons were stored.\n", photonMap->count());
				}
			}
		}

		photonMap->buildStructure();
		printf("photon map done.\n");
	}
public:
	HRaytracer() {
		shadeQuality = 1;  drflQuality = 16; NxSSAA = 1;
		max_store_photons = 1000000;
		max_emit_photons = 10000000;
		max_sample_photons = 500;
		max_sample_dist = 1;
	}
	virtual ~HRaytracer() {
		if(photonMap) delete photonMap;
	}
	void setShadeQuality(int _shadeQuality)
	{
		shadeQuality = _shadeQuality;
	}
	void setDrflQuality(int _drflQuality)
	{
		drflQuality = _drflQuality;
	}
	void setNxSSAA(int _NxSSAA)
	{
		NxSSAA = _NxSSAA;
	}
	bool render(cv::Mat & canvas, int w, int h)
	{
		srand(1998 - 06 - 07 + time(0));
		if (!this->camera || this->objUnion->empty()) return false;
		this->objUnion->buildStructure();
		this->buildPhotonMap();
		int ww = w * NxSSAA, hh = h * NxSSAA;
		canvas = cv::Mat(hh, ww, CV_8UC3, cv::Scalar(255, 255, 255));
		double dx = 1. / ww, dy = 1. / hh;
		double sx = dx / 2, sy = dy / 2;
		int sum = 0, total = ww * hh;
		std::mutex mtx;
		canvas.forEach<Pixel>([&](Pixel& pixel, const int pos[]) -> void {
			pixel = this->camera->calcPixel(dx, dy, pos[1], pos[0],
				[this](HRay ray) -> HColor {
					return this->rayTrace(ray, 1, 0);
			}).toPixel();
			mtx.lock();
			if ((++sum) % 10000 == 0) printf("%.2lf%% was done.\n", 100.*sum / total);
			mtx.unlock();
		});
		if (NxSSAA > 1) cv::resize(canvas, canvas, cv::Size(canvas.size[1] / NxSSAA, canvas.size[0] / NxSSAA));
		return true;
	}
};