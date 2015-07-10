#ifndef PREPROCESS_H_
#define PREPROCESS_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
namespace gnp_m
{
	cv::Mat FindFOVMask(cv::Mat input);
	cv::Mat MakeGradientImage(cv::Mat input);
	cv::Mat FindFoviaMask(cv::Mat input);
	cv::Mat FindOpticDiskMask(cv::Mat input);
	cv::Mat ExtractChannel(cv::Mat input, int channel);
	cv::Mat FindVesselMask(cv::Mat input);
	cv::Mat SubtractVesselMap(cv::Mat input, cv::Mat vesselMap);
	void ErodeMap(cv::Mat input, int radius);
	cv::Mat FindHueMask(cv::Mat input, cv::Mat mask);
}
#endif