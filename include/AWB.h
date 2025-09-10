#ifndef AWB_H
#define AWB_H
#include"Common.h"


struct WB_GAIN {
	ushort R_Gain;
	ushort B_Gain;
};

enum AWBmethod {
	GreyWorld = 0,
	WhitePatch
};

class AWB {
public:
	AWB(cv::Mat input, BayerID Bayer_ID, int BIT_SIZE): input(input), Bayer_ID(Bayer_ID), BIT_SIZE(BIT_SIZE) {}
	cv::Mat process(AWBmethod method);
	WB_GAIN getWBgain() {
		return wbgain;
	}
private:
	Mat input;
	WB_GAIN wbgain;
	BayerID Bayer_ID;
	int BIT_SIZE;
	cv::Mat AWB_Calculate(Mat input, BayerID Bayer_ID, int BIT_SIZE, WB_GAIN* wbgain);

};

#endif
