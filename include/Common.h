#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <fstream>
#include"string"
#include"cstdlib"
#include"stdlib.h"
#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>


using namespace std;
using namespace cv;

typedef unsigned char BYTE;

enum class IMAGE_TYPE {
    GREY = 0,
    RGB,
    YUV
};

enum class BayerID {
	RGGB = 0,
	BGGR,
	GBRG,
	GRBG
};

const float R2Ymask[9] = {
	0.299,0.587,0.114,
	(-0.169),(-0.331),0.5,
	0.5,  (-0.419),(-0.081)
};

const float Y2Rmask[4] = {
	1.4075,
	-0.3455,-0.7169,
	1.779
};

//class common {
//public:
//	cv::Mat RGB2YUV(Mat input, int BIT_SIZE);
//	cv::Mat YUV2RGB(Mat input);
//	std::vector<cv::Mat> Separate_YUV(Mat input);
//	cv::Mat Merge_YUV(std::vector<cv::Mat> input);
//	cv::Mat ShowOutputImg(Mat input, IMAGE_TYPE ImageType, char input_bit);
//	cv::Mat Conv_filter(Mat input, IMAGE_TYPE ImageType, Mat conv_matrix);
//private:
//
//};

namespace com {
	cv::Mat RGB2YUV(Mat input, int BIT_SIZE);
	cv::Mat YUV2RGB(Mat input);
	std::vector<cv::Mat> Separate_YUV(Mat input);
	cv::Mat Merge_YUV(std::vector<cv::Mat> input);
	cv::Mat ShowOutputImg(Mat input, IMAGE_TYPE ImageType, char input_bit);
	cv::Mat Conv_filter(Mat input, IMAGE_TYPE ImageType, Mat conv_matrix);
}


#endif

