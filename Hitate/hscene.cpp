#include "hscene.h"

HScene::HScene()
{
	camera = nullptr;
}

HScene::~HScene()
{
	if (camera) delete camera;
	for (auto p : objList) {
		if (p) delete p;
	}
}

void HScene::setCamera(HCamera * _camera)
{
	camera = _camera;
}

void HScene::addObj(HObject * _obj)
{
	objList.push_back(_obj);
}

void HScene::clearObj()
{
	objList.clear();
}

bool HScene::render(cv::Mat & canvas, int w, int h)
{
	if (!camera || !objList.size()) return false;
	double dx = 1. / w, dy = 1. / h;
	double sx = dx / 2, sy = dy / 2;
	canvas.forEach<Pixel>([&](Pixel& pixel, const int pos[]) -> void {
		HRay ray = camera->calcRay(sx + dx * pos[1], sy + dy * pos[0]);
		HIntersection res;
		res.isHit = false;
		for (auto obj : objList) {
			HIntersection tr = obj->intersect(ray);
			res.update(tr);
		}
		HColor rc;
		if (!res.isHit) rc = HColor(0, 0, 0);
		else rc = HColor(1. / res.dis, 1. / res.dis, 1. / res.dis);

		pixel = rc.toPixel();
	});
	return true;
}
