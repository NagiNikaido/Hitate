#include "hobjectunion.h"

HObjectUnion::HObjectUnion()
{
}

HObjectUnion::~HObjectUnion()
{
}

HObjectVector::HObjectVector()
{
}

HObjectVector::~HObjectVector()
{
	for (auto obj : objList) delete obj;
}

HIntersection HObjectVector::intersect(HRay ray)
{
	HIntersection res;
	res.hit = false;
	for (auto obj : objList) {
		HIntersection tr = obj->intersect(ray);
		res.update(tr);
	}
	return res;
}

void HObjectVector::addObj(HObject *obj)
{
	objList.push_back(obj);
}

void HObjectVector::clearObj()
{
	objList.clear();
}

bool HObjectVector::empty()
{
	return objList.empty();
}

size_t HObjectVector::size()
{
	return objList.size();
}


