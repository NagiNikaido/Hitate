#pragma once
#include "hitate.h"
#include "hobject.h"
#include "hcamera.h"
//#include "hlightsource.h"
#include <vector>

class HScene {
	HCamera *camera;
	std::vector<HObject *> objList;
//	HLightSource *lightSource;
public:
	HScene();
	~HScene();
	void setCamera(HCamera *_camera);
	void addObj(HObject *_obj);
	void clearObj();
	bool render(cv::Mat & canvas, int w, int h);
};