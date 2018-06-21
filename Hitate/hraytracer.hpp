#pragma once

#include "hscene.hpp"

template<class Object_Union_T = HObjectVector>
class HRaytracer : public HScene<typename Object_Union_T> {
	HColor calcDiff(HIntersection inter, HRay ray)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		HColor res = material.color * this->background * material.diff;
//		if (res.r > _Eps || res.g > _Eps || res.b > _Eps) printf("Here!");
		for (auto light : this->lightSourceList) {
			double shade = light->calcShade(this->objUnion, inter);
			if (shade < _Eps) continue;
			HVec3 h = (light->getG() - inter.pos).normalized() - ray.d;
			double r1 = (light->getG() - inter.pos).normalized().dotPro(inter.norm);
			double r2 = h.dotPro(inter.norm) / sqrt(2);
			
			if (r1 > _Eps && material.diff > _Eps) {
				res += material.color * light->getColor() * material.diff * r1 * shade;
			}
			
			if (r2 > _Eps && material.spec > _Eps) {
				res += material.color * light->getColor() * material.spec * pow(r2, _Spec_Power) * shade;
			}
			
		}

		return res;
	}
	HColor calcRefl(HIntersection inter, HRay ray, double weight, int depth)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		//HColor res = material.color * this->background * material.diff;
		ray = ray.reflect(inter.pos, inter.norm);
		return material.color * material.refl * rayTrace(ray, weight*material.refl, depth + 1);
		//return HColor(0, 0, 0);
	}
	HColor calcRefr(HIntersection inter, HRay ray, double weight, int depth)
	{
		HObject *obj = inter.obj;
		HMaterial material = obj->getMaterial();
		ray = ray.refract(inter.pos, inter.norm, inter.front ? material.rindex : 1 / material.rindex);

		HColor rr = rayTrace(ray, weight*material.refr, depth + 1);
		if (inter.front) return rr * material.refr;
		HColor tr = (material.absor * -inter.dis).exp();
		return rr * tr * material.refr;
	}
	HColor rayTrace(HRay ray, double weight, int depth)
	{
		if (!depth || weight<_Eps) return HColor(0, 0, 0);
		HIntersection light, object;
		HColor res(0, 0, 0);
		light = this->lightIntersect(ray);
		object = this->objUnion->intersect(ray);
		if (light.hit && !(object.hit || object.dis > light.dis)) {
			res = light.obj->getMaterial().color;
		}
		if (object.hit) {
			HObject *obj = object.obj;
			if (obj->getMaterial().diff > _Eps || obj->getMaterial().spec > _Eps)
				res += calcDiff(object, ray);
			if (obj->getMaterial().refl > _Eps)
				res += calcRefl(object, ray, weight, depth);
			if (obj->getMaterial().refr > _Eps)
				res += calcRefr(object, ray, weight, depth);
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
		/*
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++) {
				HRay ray = this->camera->calcRay(sx + dx * x, 1 - sx - dy * y);
				canvas.at<Pixel>(y, x) = rayTrace(ray, 1, _Max_Raytrace_Depth).toPixel();
			}
		*/
		
		canvas.forEach<Pixel>([&](Pixel& pixel, const int pos[]) -> void {
			HRay ray = this->camera->calcRay(sx + dx * pos[1], 1 - sy - dy * pos[0]);
			pixel = rayTrace(ray, 1, _Max_Raytrace_Depth).toPixel();
			/*
			if (pos[0] == 514 && pos[1] == 529) {
				std::cout << pixel << endl;
				auto object = this->objUnion->intersect(ray);
				std::cout << object.hit << endl;
				std::cout << ray.d.x << ',' << ray.d.y << ',' << ray.d.z << endl;
				std::cout << ray.op.x << ',' << ray.op.y << ',' << ray.op.z << endl;
				std::cout << object.pos.x << ',' <<object.pos.y << ',' <<object.pos.z << endl;
				std::cout << object.norm.x << ',' << object.norm.y << ',' << object.norm.z << endl;
				for (auto light : this->lightSourceList) {
					cout << light->getMetadata() << endl;
					double shade = light->calcShade(this->objUnion, object);
					cout << shade << endl;
				}
			}
			*/
		});
		
		return true;
	}
};