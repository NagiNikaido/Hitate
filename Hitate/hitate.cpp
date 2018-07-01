#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include "hitate.h"
#include "hraytracer.hpp"
#include "hcamera.h"

using namespace std;

const int h = 960;
const int w = 1280;
int main() {
	HRaytracer scene;
	cv::Mat canvas;
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(0, 0, 1), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.8, 0)));
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(0, 1, 0), HMaterial(HColor(0.6, 0.0, 0.0), HColor(0, 0, 0), 0, 0, 0.8, 0)));
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(1, 0, 0), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.8, 0)));
	scene.addObj(new HPlane(HVec3(0, 4, 4), HVec3(0, 0, -1), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.8, 0)));
	scene.addObj(new HPlane(HVec3(0, 4, 4), HVec3(0, -1, 0), HMaterial(HColor(0.0, 0.6, 0.0), HColor(0, 0, 0), 0, 0, 0.8, 0)));
	scene.addObj(new HSphere(HVec3(5, 1, 0.5), 0.5, HMaterial(HColor(1., 1., 1.), HColor(0, 0, 0), 0.9, 0.0, 0.05, 0.05, 0, 0)));
	scene.addObj(new HPoly(HVec3(3, 2, 1), 1, "bunny.obj", HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 1, 0, 0, 0)));
	scene.addObj(new HSphere(HVec3(4, 3, 0.5), 0.5, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0x66 / 255.0, 0xcc / 255.0, 1.), 0.1, 0.8, 0.05, 0.05, 1.7, 0)));
	scene.addLightSource(new HAreaLightSource(HColor(15, 15, 15), HVec3(2, 2, 4 - 10 * _Eps), HVec3(0, 0.7, 0), HVec3(0.7, 0, 0)));
	scene.setBackground(HColor(0.2, 0.2, 0.2));

	scene.setShadeQuality(4);
	scene.setNxSSAA(2);
	scene.setCamera(new HPinholeCamera(HVec3(10, 2, 2), HVec3(-1, 0, 0), HVec3(0, 0, 1), 70, .75));
	cout << (scene.render(canvas, w , h ) ? 1 : 0) << endl;
	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	cv::imshow("Hitate canvas", canvas);
	cv::imwrite("Hitate_canvas_2xSSAA.png", canvas, compression_params);
	cv::waitKey(0);

	scene.setCamera(new HApertureCamera(HVec3(10, 2, 2), 0.2, 6, HVec3(-1, 0, 0), HVec3(0, 0, 1), 70, .75, 512));
	scene.setNxSSAA(1);
	cout << (scene.render(canvas, w, h) ? 1 : 0) << endl;
	cv::imshow("Hitate canvas", canvas);
	cv::imwrite("Hitate_canvas_aperture.png", canvas, compression_params);
	cv::waitKey(0);
	return 0;
}