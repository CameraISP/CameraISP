#ifndef TM_H
#define TM_H
#include"Common.h"



class TM {
public:
	TM(Mat input, float gma_coef, int BIT_SIZE): input(input), gma_coef(gma_coef),BIT_SIZE(BIT_SIZE){}
	cv::Mat process();
private:
	cv::Mat Gamma_process(Mat input, float gma_coef, int BIT_SIZE);
	Mat input; 
	float gma_coef;
	int BIT_SIZE;
};

#endif
