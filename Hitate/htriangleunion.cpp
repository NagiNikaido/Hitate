#include "htriangleunion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Node * newNode(int size, HBox box) {
	Node *p = new Node;
	p->size = size;
	p->box = box;
	p->lc = p->rc = nullptr;
	p->tri = new HTriangle*[size];
	p->sp = -1;
	return p;
}
void HTriangleUnion::build(Node *rt)
{
	double cc = rt->box.area() * rt->size, mc = cc;
	double sp;
	HBox lbox, rbox;
	for (int k = 0; k < 3; k++) {
		sort(rt->tri, rt->tri + rt->size, [=](HTriangle* a, HTriangle *b) {
			return a->box.min$(k) < b->box.min$(k);
		});
		lbox = rbox = rt->box;
		for (int i = 0, j = 0; i < rt->size; i++) {
			double pos = rt->tri[i]->box.min$(k);
			for (; j < rt->size && rt->tri[j]->box.max$(k) <= pos; j++);
			lbox.max$(k) = rbox.min$(k) = pos;
			double c = lbox.area() * j + rbox.area() * (rt->size - j);
			if (c < mc) mc = c, rt->sp = k, sp = pos;
		}

		sort(rt->tri, rt->tri + rt->size, [=](HTriangle *a, HTriangle *b) {
			return a->box.max$(k) < b->box.max$(k);
		});
		lbox = rbox = rt->box;
		for (int i = 0, j = 0; i < rt->size; i++) {
			double pos = rt->tri[i]->box.max$(k);
			for (; j < rt->size && rt->tri[j]->box.min$(k) <= pos; j++);
			lbox.max$(k) = rbox.min$(k) = pos;
			double c = lbox.area() * j + rbox.area() * (rt->size - j);
			if (c < mc) mc = c, rt->sp = k, sp = pos;
		}
	}
	if (rt->sp == -1) return;
	int lsize = 0, rsize = 0;
	lbox = rbox = rt->box;
	lbox.max$(rt->sp) = sp;
	rbox.min$(rt->sp) = sp;
	for (int i = 0; i < rt->size; i++) {
		if (rt->tri[i]->box.min$(rt->sp) <= sp) lsize++;
		if (rt->tri[i]->box.max$(rt->sp) >= sp) rsize++;
	}
	if (!lsize || !rsize || lbox.area() * lsize + rbox.area() * rsize >= cc) {
		rt->sp = -1;
		return;
	}
	rt->lc = newNode(lsize, lbox);
	rt->rc = newNode(rsize, rbox);
	for (int i = 0, j = 0, k = 0; i < rt->size; i++) {
		if (rt->tri[i]->box.min$(rt->sp) <= sp) rt->lc->tri[j++] = rt->tri[i];
		if (rt->tri[i]->box.max$(rt->sp) >= sp) rt->rc->tri[k++] = rt->tri[i];
	}
	build(rt->lc);
	build(rt->rc);
}

HIntersection HTriangleUnion::trace(Node *rt, HRay ray) {
	double d, ld, rd;
	HIntersection res;
	if (!rt->box.intersect(ray, d)) return res;
	if (rt->sp == -1) {
		for (int i = 0; i < rt->size; i++) {
			HIntersection t = rt->tri[i]->intersect(ray);
			if (rt->box.in(t.pos)) res.update(rt->tri[i]->intersect(ray));
		}
		return res;
	}
	bool lhit = rt->lc->box.intersect(ray, ld);
	bool rhit = rt->rc->box.intersect(ray, rd);
	if (!lhit && !rhit) return res;
	else if (!lhit) return trace(rt->rc, ray);
	else if (!rhit) return trace(rt->lc, ray);
	else {
		if (ld < rd) {
			res = trace(rt->lc, ray);
			if (res.hit) return res;
			return trace(rt->rc, ray);
		}
		else {
			res = trace(rt->rc, ray);
			if (res.hit) return res;
			return trace(rt->lc, ray);
		}
	}
}

void HTriangleUnion::free(Node *c)
{
	if (!c) return;
	free(c->lc); free(c->rc);
	delete c;
}

HTriangleUnion::HTriangleUnion(HObject * _belongTo, HVec3 pos, double size_c, std::string fileName)
{
	belongTo = _belongTo;
	ifstream fin(fileName);
	string t;
	HVec3 *plst;
	int pc;
	fin >> t >> pc >> cnt;
	plst = new HVec3[pc + 1];
	lst = new HTriangle[cnt + 1];
	for (int i = 1; i <= pc; i++) {
		fin >> t >> plst[i].x >> plst[i].y >> plst[i].z;
		plst[i] = plst[i] * size_c + pos;
	}
	for (int i = 1; i <= cnt; i++) {
		int a, b, c;
		fin >> t >> a >> b >> c;
		lst[i] = HTriangle(plst[a], plst[b], plst[c], belongTo);
		if (i == 1) axis_box = lst[i].box;
		else axis_box.update(lst[i].box);
	}
	delete[] plst;
	rt = newNode(cnt, axis_box);
	for (int i = 0; i < cnt; i++) rt->tri[i] = lst + i + 1;
	build(rt);
}

HTriangleUnion::~HTriangleUnion()
{
	free(rt);
	delete[] lst;
}

HIntersection HTriangleUnion::intersect(HRay ray)
{
	return trace(rt, ray);
}
