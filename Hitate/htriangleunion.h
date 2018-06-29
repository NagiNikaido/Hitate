#pragma once

#include "htriangle.h"
#include <string>

struct Node {
	HTriangle **tri;
	int sp,size;
	HBox box;
	Node *lc, *rc;
	~Node() {
		delete[] tri;
	}
};

class HTriangleUnion {
	Node *rt;
	HTriangle *lst;
	HObject *belongTo;
	HBox axis_box;
	int cnt;
	void build(Node *rt);
	HIntersection trace(Node *rt, HRay ray)	;
	void free(Node *c);
public:
	HTriangleUnion(HObject *_belongTo, HVec3 pos, double size_c, std::string fileName);
	~HTriangleUnion();
	HIntersection intersect(HRay ray);
};