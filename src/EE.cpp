#include "EE.h"


cv::Mat EE::Edge_enhancement(Mat Y_img, int edgeThr, int textureThr, float edgeStrength, float textureStrength) { 
	// YUV map(uchar type) input, EEmap(int16_t type)
	Mat EEmap;
	Filter conv_matrix;
	EEmap = com::Conv_filter(Y_img, IMAGE_TYPE::GREY, conv_matrix.CVITEK_Mask);
	//imwrite("EdgeDT.bmp", ShowOutputImg(EdgeDT, IMAGE_TYPE::GREY, 10));
	Mat result;
	int16_t EEvalue;
	result = Y_img.clone();
	for (int i = 0; i < Y_img.rows; i++) {
		for (int j = 0; j < Y_img.cols; j++) {
			EEvalue = EEmap.at<int16_t>(i, j);
			float blendingStrength;
			if (abs(EEvalue) >= edgeThr) {
				if (EEvalue > 0) {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 + edgeStrength));
				}
				else {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 - edgeStrength));
				}				
			}
			else if (abs(EEvalue) >= textureThr) {
				float diff_a, diff_b;
				diff_a = abs(EEvalue) - textureThr;
				diff_b = edgeThr - abs(EEvalue);
				blendingStrength = (diff_a * edgeStrength + diff_b * textureStrength) / (edgeThr - textureThr);
				if (EEvalue > 0) {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 + blendingStrength));
				}
				else {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 - blendingStrength));
				}
			}
			else {
				blendingStrength = (abs(EEvalue) * textureStrength) / textureThr;
				if (EEvalue > 0) {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 + blendingStrength));
				}
				else {
					result.at<uchar>(i, j) = MIN(255, Y_img.at<uchar>(i, j) * (1 - blendingStrength));
				}
			}
		}
	}

	return result;
}

cv::Mat EE::process() {
	return EE::Edge_enhancement(Y_img, edgeThr, textureThr, edgeStrength, textureStrength);
}