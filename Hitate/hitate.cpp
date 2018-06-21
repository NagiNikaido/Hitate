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
	cv::Mat canvas(h, w, CV_8UC3, cv::Scalar(255, 255, 255));
	scene.setCamera(new HEyeCamera(HVec3(10, 10, 10), HVec3(-1, -1, -1), HVec3(0, 0, 1), 70, .75));
	scene.addObj(new HSphere(HVec3(0, 0, 0), 1, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.9, 0, 0.45, 0.8)));
	scene.addObj(new HSphere(HVec3(3, 0, 0), 1, HMaterial(HColor(0.6, 0.5, 0), HColor(0, 0, 0), 0.9, 0, 0.45, 0.8)));
	scene.addObj(new HSphere(HVec3(0, 3, 0), 1, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.9, 0, 0.45, 0.8)));
	scene.addObj(new HSphere(HVec3(0, 6, 0), 1, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.9, 0, 0.45, 0.8)));
	scene.addObj(new HSphere(HVec3(0, 0, 3), 1, HMaterial(HColor(0x66 / 255.0, 0xcc / 255.0, 1.), HColor(0, 0, 0), 0.9, 0, 0.45, 0.8)));
	//scene.addObj(new HPlane())
	scene.setBackground(HColor(0x66 / 255.0, 0xcc / 255.0, 1.));
	scene.addLightSource(new HPointLightSource(HColor(1, 1, 1), HVec3(-5, 4, 25)));
	cout << (scene.render(canvas, w, h) ? 1 : 0) << endl;
	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	cv::imshow("Hitate canvas", canvas);
	cv::imwrite("Hitate_canvas.png", canvas, compression_params);
	cout << canvas.at<Pixel>(514, 529) << endl;
	cv::waitKey(0);
	return 0;
}