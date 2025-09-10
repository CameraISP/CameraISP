#include"AWB.h"

cv::Mat AWB::AWB_Calculate(Mat input,BayerID Bayer_ID, int BIT_SIZE, WB_GAIN* wbgain) {
	Mat result(input.rows, input.cols, CV_16SC3);
	float statistic_R = 0, statistic_Gr = 0, statistic_Gb = 0, statistic_B = 0;
	int countR = 0, countGr = 0, countGb = 0, countB = 0;
	if (Bayer_ID == BayerID::RGGB) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j+=2) {
				if (i % 2 == 0) {
					statistic_R += input.at<ushort>(i, j);
					statistic_Gr += input.at<ushort>(i, j + 1);
					countR++;
					countGr++;
				}
				else {
					statistic_Gb += input.at<ushort>(i, j);
					statistic_B += input.at<ushort>(i, j + 1);
					countGb++;
					countB++;
				}
			}
		}
	}
	else if (Bayer_ID == BayerID::BGGR) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j += 2) {
				if (i % 2 == 0) {
					statistic_B += input.at<ushort>(i, j);
					statistic_Gb += input.at<ushort>(i, j + 1);
					countB++;
					countGb++;
				}
				else {
					statistic_Gr += input.at<ushort>(i, j);
					statistic_R += input.at<ushort>(i, j + 1);
					countGr++;
					countR++;
				}
			}
		}
	}
	else if (Bayer_ID == BayerID::GBRG) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j += 2) {
				if (i % 2 == 0) {
					statistic_Gb += input.at<ushort>(i, j);
					statistic_B += input.at<ushort>(i, j + 1);
					countB++;
					countGb++;
				}
				else {
					statistic_R += input.at<ushort>(i, j);
					statistic_Gr += input.at<ushort>(i, j + 1);
					countGr++;
					countR++;
				}
			}
		}
	}

	statistic_R /= countR;
	statistic_Gr /= countGr;
	statistic_Gb /= countGb;
	statistic_B /= countB;

	wbgain->R_Gain = (ushort)((((statistic_Gr + statistic_Gb) / 2) / statistic_R) *1024);
	wbgain->B_Gain = (ushort)((((statistic_Gr + statistic_Gb) / 2) / statistic_B) *1024);

	if (Bayer_ID == BayerID::RGGB) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j += 2) {
				if (i % 2 == 0) {
					result.at<Vec3s>(i, j)[0] = 0;
					result.at<Vec3s>(i, j)[1] = 0;
					result.at<Vec3s>(i, j)[2] = MIN(((int)input.at<ushort>(i, j) * wbgain->R_Gain) >> 10, BIT_SIZE - 1);
					result.at<Vec3s>(i, j + 1)[0] = 0;
					result.at<Vec3s>(i, j + 1)[1] = input.at<ushort>(i, j + 1);
					result.at<Vec3s>(i, j + 1)[2] = 0;


				}
				else {
					result.at<Vec3s>(i, j)[0] = 0;
					result.at<Vec3s>(i, j)[1] = input.at<ushort>(i, j);
					result.at<Vec3s>(i, j)[2] = 0;
					result.at<Vec3s>(i, j + 1)[0] = MIN(((int)input.at<ushort>(i, j + 1) * wbgain->B_Gain) >> 10, BIT_SIZE - 1);
					result.at<Vec3s>(i, j + 1)[1] = 0;
					result.at<Vec3s>(i, j + 1)[2] = 0;
				}

			}
		}
	}
	else if (Bayer_ID == BayerID::BGGR) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j += 2) {
				if (i % 2 == 0) {
					result.at<Vec3s>(i, j)[0] = MIN(((int)input.at<ushort>(i, j) * wbgain->B_Gain) >> 10, BIT_SIZE - 1);
					result.at<Vec3s>(i, j)[1] = 0;
					result.at<Vec3s>(i, j)[2] = 0;
					result.at<Vec3s>(i, j + 1)[0] = 0;
					result.at<Vec3s>(i, j + 1)[1] = input.at<ushort>(i, j + 1);
					result.at<Vec3s>(i, j + 1)[2] = 0;
				}
				else {
					result.at<Vec3s>(i, j)[0] = 0;
					result.at<Vec3s>(i, j)[1] = input.at<ushort>(i, j);
					result.at<Vec3s>(i, j)[2] = 0;
					result.at<Vec3s>(i, j + 1)[0] = 0;
					result.at<Vec3s>(i, j + 1)[1] = 0;
					result.at<Vec3s>(i, j + 1)[2] = MIN(((int)input.at<ushort>(i, j + 1) * wbgain->R_Gain) >> 10, BIT_SIZE - 1);
				}

			}
		}
	}
	else if (Bayer_ID == BayerID::GBRG) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j += 2) {
				if (i % 2 == 0) {
					result.at<Vec3s>(i, j)[0] = 0;
					result.at<Vec3s>(i, j)[1] = input.at<ushort>(i, j);
					result.at<Vec3s>(i, j)[2] = 0;
					result.at<Vec3s>(i, j + 1)[0] = MIN(((int)input.at<ushort>(i, j + 1) * wbgain->B_Gain) >> 10, BIT_SIZE - 1);
					result.at<Vec3s>(i, j + 1)[1] = 0;
					result.at<Vec3s>(i, j + 1)[2] = 0;
				}
				else {
					result.at<Vec3s>(i, j)[0] = 0;
					result.at<Vec3s>(i, j)[1] = 0;
					result.at<Vec3s>(i, j)[2] = MIN(((int)input.at<ushort>(i, j) * wbgain->R_Gain) >> 10, BIT_SIZE - 1);
					result.at<Vec3s>(i, j + 1)[0] = 0;
					result.at<Vec3s>(i, j + 1)[1] = input.at<ushort>(i, j + 1);
					result.at<Vec3s>(i, j + 1)[2] = 0;
				}

			}
		}
	}
	return result;
}

// polymophism
cv::Mat AWB::process(AWBmethod method){
	if (method == AWBmethod::GreyWorld) {
		return AWB::AWB_Calculate(input, Bayer_ID, BIT_SIZE, &wbgain);
	}
	else if (method == AWBmethod::WhitePatch) {
		//
	}
	else {
		return AWB::AWB_Calculate(input, Bayer_ID, BIT_SIZE, &wbgain); // Default GreyWorld method
	}
	
}