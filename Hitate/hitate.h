#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

typedef cv::Point3_<uint8_t> Pixel;

const double _Eps = 1e-6;
const double _Delta = 1e-4;
const int _Max_Raytrace_Depth = 10;
const int _Max_Drfl_Depth = 2;
const double _Spec_Power = 10;