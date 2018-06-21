#pragma once

#include "hitate.h"
#include "hobject.h"
#include <vector>

class HObjectUnion {
public:
	HObjectUnion();
	virtual ~HObjectUnion();
	virtual HIntersection intersect(HRay) = 0;
	virtual void addObj(HObject *) = 0;
	virtual void clearObj() = 0;
	virtual bool empty() = 0;
	virtual size_t size() = 0;
	virtual void buildStructure() {};
};

class HObjectVector : public HObjectUnion {
private:
	std::vector<HObject *> objList;
public:
	HObjectVector();
	virtual ~HObjectVector();
	virtual HIntersection intersect(HRay);
	virtual void addObj(HObject *);
	virtual void clearObj();
	virtual bool empty();
	virtual size_t size();
};