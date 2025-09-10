#ifndef BNR_H
#define BNR_H
#include "Common.h"

struct GF_coef {
	float slope_a;
	float const_b;
};

enum BNRmethod {
	Guided = 0,
	Gaussian
};


class BNR {
public:
	BNR(Mat input, BayerID Bayer_ID, int window_size, float panality):
		input(input), 
		Bayer_ID(Bayer_ID), 
		window_size(window_size), 
		panality(panality)
	{}
	cv::Mat process(BNRmethod method);
private:
	Mat input;
	BayerID Bayer_ID;
	int window_size;
	float panality;
	GF_coef Guided_filter(Mat roi, Mat guided, float panality);
	cv::Mat Guided(Mat input, BayerID Bayer_ID, int window_size, float panality);
};



#endif
