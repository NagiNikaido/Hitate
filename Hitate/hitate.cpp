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
	HRaytracer<> scene;
	//HScene<> scene;
	cv::Mat canvas(h * 2, w * 2, CV_8UC3, cv::Scalar(255, 255, 255));
	scene.setCamera(new HEyeCamera(HVec3(7, 2, 2.5), HVec3(-10, 0, -.5), HVec3(0, 0, 1), 70, .75));
	/*
	scene.addObj(new HSphere(HVec3(0, 0, 0), 0.25, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.8, 0, 0.15, 0.05)));
	scene.addObj(new HSphere(HVec3(0.75, 0, 0), .25, HMaterial(HColor(0.6, 0.5, 0), HColor(0, 0, 0), 0.9, 0, 0.05, 0.05)));
	scene.addObj(new HSphere(HVec3(0, 0.75, 0), .25, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.8, 0, 0.15, 0.05)));
	scene.addObj(new HSphere(HVec3(0, 1.5, 0), .25, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.8, 0, 0.15, 0.05)));
	scene.addObj(new HSphere(HVec3(0, 0, 0.75), .25, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.8, 0, 0.15, 0.05)));
	scene.addObj(new HPlane(HVec3(0, 0, -0.5), HVec3(0, 0, 1), HMaterial(HColor(1., 1., 1.), HColor(0, 0, 0), 0, 0, 0.3, 0.3)));
	*/
	/*
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(0, 0, 1), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.9, 0)));
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(0, 1, 0), HMaterial(HColor(0.6, 0.0, 0.0), HColor(0, 0, 0), 0, 0, 0.9, 0)));
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(1, 0, 0), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.9, 0)));
	scene.addObj(new HPlane(HVec3(0, 4, 4), HVec3(0, 0, -1), HMaterial(HColor(1.0, 1.0, 1.0), HColor(0, 0, 0), 0, 0, 0.9, 0)));
	scene.addObj(new HPlane(HVec3(0, 4, 4), HVec3(0, -1, 0), HMaterial(HColor(0.0, 0.6, 0.0), HColor(0, 0, 0), 0, 0, 0.9, 0)));
	scene.addObj(new HSphere(HVec3(2, 1, 0.5), 0.5, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0x66 / 255.0, 0xcc / 255.0, 1.), 0.3, 0.6, 0.05, 0.05, 1.3, 0)));
	scene.addLightSource(new HAreaLightSource(HColor(8, 8, 8), HVec3(9, 2, 2), HVec3(0, 0, 1), HVec3(0, 1, 0)));
	scene.setBackground(HColor(0.2, 0.2, 0.2));
	*/
	
	scene.setCamera(new HEyeCamera(HVec3(1, -0.5, 1), HVec3(-1, 0.4, -0.8), HVec3(0, 0, 1), 70, .75));
	scene.addLightSource(new HAreaLightSource(HColor(8, 8, 8), HVec3(3, 1, 1), HVec3(0, 1, 0), HVec3(1, 0, 0)));
	scene.addObj(new HPlane(HVec3(0, 0, 0), HVec3(0, 0, 1), HMaterial(HColor(.6, .6, .6), HColor(0, 0, 0), 0, 0, 1., 0, 0, 0 )));
	scene.addObj(new HSphere(HVec3(0, 0, .2), .2, HMaterial(HColor(0, 0, 0), HColor(5, 5, 0), 0, 1, 0, 0, 1.7, 0)));
	scene.setShadeQuality(4);
	cout << (scene.render(canvas, w * 2, h * 2) ? 1 : 0) << endl;
	cv::resize(canvas, canvas, cv::Size(canvas.size[1] / 2, canvas.size[0] / 2));
	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	cv::imshow("Hitate canvas", canvas);
	cv::imwrite("Hitate_canvas.png", canvas, compression_params);
	cout << canvas.at<Pixel>(514, 529) << endl;
	cv::waitKey(0);
	return 0;
}