#ifndef RAWTRANSFER_H
#define RAWTRANSFER_H
#include"Common.h"

struct BLC {
	int R_clamp;
	int Gr_clamp;
	int Gb_clamp;
	int B_clamp;
};

enum RTmethod {
	R2B = 0,
	T2B
};
class RawTransfer {
public:
	RawTransfer(std::string Path, int width, int height, BLC blc, BayerID Bayer_ID, int BIT_SIZE): 
		Path(Path), 
		width(width),
		height(height),
		blc(blc),
		Bayer_ID(Bayer_ID),
		BIT_SIZE(BIT_SIZE){}
	cv::Mat process(RTmethod method);

private:

	void readRAW(); // read raw image to cv::Mat

private:
	std::string Path;
	int width;
	int height;
	BLC blc;
	BayerID Bayer_ID;
	int BIT_SIZE;

	cv::Mat mRaw;
};




#endif
