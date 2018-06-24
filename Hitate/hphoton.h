#pragma once

#include "hitate.h"
#include "hutils.h"
#include "hobjectunion.h"
#include <utility>
#include <vector>
#include <algorithm>

struct HPhoton {
	HRay ray;
	HColor color;
};

inline bool operator<(const HPhoton &a, const HPhoton &b) {
	return true;
}

struct HNPhotons {
	double max_dist;
	int max_sample;
	typedef std::pair<double, HPhoton*> PDH;
	PDH *lst;
	int size;
	HNPhotons(double _max_dist, int _max_sample);
	~HNPhotons();
	double bound();
	void push(double dist2, HPhoton *p);
};

class HPhotonMap {
	HPhoton *php;
	int stored, cnt, total, *axis;
	HVec3 axis_min, axis_max;
	void build(HPhoton *tph, int c, int l, int r);
	void query(int c, HVec3 p, HVec3 n, HNPhotons &nph);
public:
	HPhotonMap(int _stored, int _total);
	~HPhotonMap();
	void insertPhoton(HPhoton p);
	void buildStructure();
	HColor calcL(HIntersection inter, double max_dist, int max_sample);
	int count() { return cnt; }

};