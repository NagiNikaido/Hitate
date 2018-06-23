#pragma once

#include "hscene.hpp"

template<class Object_Union_T = HObjectVector>
class HRaytracer : public HScene<typename Object_Union_T> {
	HColor calcDiff(HIntersection inter)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		HColor res = material.color * this->background * material.diff;
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
			res = light.obj->getMaterial().color.saturated();
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
public:
	HRaytracer(){}
	virtual bool render(cv::Mat & canvas, int w, int h)
	{
		if (!this->camera || this->objUnion->empty()) return false;
		this->objUnion->buildStructure();
		double dx = 1. / w, dy = 1. / h;
		double sx = dx / 2, sy = dy / 2;
		
		canvas.forEach<Pixel>([&](Pixel& pixel, const int pos[]) -> void {
			HRay ray = this->camera->calcRay(sx + dx * pos[1], 1 - sy - dy * pos[0]);
			pixel = rayTrace(ray, 1, 0).toPixel();
		});
		
		return true;
	}
};