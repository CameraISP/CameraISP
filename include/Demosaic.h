#ifndef DEMOSAIC_H
#define DEMOSAIC_H
#include"Common.h"

enum DemosaicMethod {
	NEAREAST = 0,
	BILINEAR,
	AHD //Adaptive Homogeneity-Directed
};



class demosaic {
public:
	demosaic(Mat input, BayerID Bayer_ID): input(input), Bayer_ID(Bayer_ID) {}
	cv::Mat process(DemosaicMethod Dmethod);
private:
	cv::Mat AHD(Mat input, BayerID Bayer_ID);
	cv::Mat Nearest(Mat input, BayerID Bayer_ID);
	cv::Mat Bilinear(Mat input, BayerID Bayer_ID);
	float Alpha_blend(float value_a, float value_b, float ratio_a, float ratio_b);
	Mat input;
	BayerID Bayer_ID;
	DemosaicMethod Dmethod;
};

#endif
