#include <cstdio>
#include <algorithm>
#include <vector>
#include "hitate.h"
#include "hscene.h"
#include "hcamera.h"

using namespace std;

const int h = 960;
const int w = 960;
int main() {
	HScene scene;
	cv::Mat canvas(h, w, CV_8UC3, cv::Scalar(255, 255, 255));
	scene.setCamera(new HEyeCamera(HVec3(0, 0, -10), HVec3(0, 0, 1), HVec3(0, -1, 0), 160, 160));
	scene.addObj(new HSphere(HVec3(0, 0, 0), 8));
	scene.render(canvas, w, h);
	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
	cv::imshow("Hitate canvas", canvas);
	cv::imwrite("Hitate_canvas.png", canvas, compression_params);
	cv::waitKey(0);
	return 0;
}