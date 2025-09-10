#ifndef EE_H
#define EE_H
#include"Common.h"

const struct Filter {
	Mat LaplaceNeg_Mask = (Mat_<float>(3, 3) << 0, -1.0F / 8, 0, -1.0F / 8, 4.0F / 8, -1.0F / 8, 0, -1.0F / 8, 0);
	Mat LaplacePos_Mask = (Mat_<float>(3, 3) << 0, 1.0F / 8, 0, 1.0F / 8, -4.0F / 8, 1.0F / 8, 0, 1.0F / 8, 0);
	Mat CVITEK_Mask = (Mat_<float>(5, 5) <<
		-1, -2, -2, -2, -1,
		-2, 0, 4, 0, -2,
		-2, 4, 12, 4, -2,
		-2, 0, 4, 0, -2,
		-1, -2, -2, -2, -1);

	Mat SobelVert_Mask = (Mat_<float>(3, 3) <<
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1);
	Mat SobelHorz_Mask = (Mat_<float>(3, 3) <<
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1);

	Mat SobelVert_Mask_neg = (Mat_<float>(3, 3) <<
		1, 0, -1,
		2, 0, -2,
		1, 0, -1);
	Mat SobelHorz_Mask_neg = (Mat_<float>(3, 3) <<
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1);

	Mat Adaptive_Mask = (Mat_<float>(3, 3) <<
		1, 1, 1,
		1, 1, 1,
		1, 1, 1);
};



class EE {
public:
	EE(Mat Y_img, int edgeThr, int textureThr, float edgeStrength, float textureStrength) : 
		Y_img(Y_img),
		edgeThr(edgeThr),
		textureThr(textureThr),
		edgeStrength(edgeStrength),
		textureStrength(textureStrength)
	{}
	cv::Mat process();
private:
	cv::Mat Edge_enhancement(Mat Y_img, int edgeThr, int textureThr, float edgeStrength, float textureStrength);
	Mat Y_img;
	int edgeThr;
	int textureThr;
	float edgeStrength;
	float textureStrength;
};

#endif
