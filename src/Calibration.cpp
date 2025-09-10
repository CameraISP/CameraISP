#include "Calibration.h"

void Calibration::CCM_interpolation(int CT, int* CCM) {
	for (int i = 0; i < CCM_SIZE - 1; i++) {
		if (CT >= CT_CCM[0]) {
			for (int j = 0; j < 9; j++) {
				CCM[j] = CCM_Calib[i][j];
			}
		}
		else if (CT <= CT_CCM[CCM_SIZE - 1]) {
			for (int j = 0; j < 9; j++) {
				CCM[j] = CCM_Calib[CCM_SIZE - 1][j];
			}
		}
		else {
			if (CT - CT_CCM[i] < 0 && CT - CT_CCM[i + 1] >= 0) {
				float alpha = (float)(CT_CCM[i] - CT) / (CT_CCM[i] - CT_CCM[i + 1]);
				for (int j = 0; j < 9; j++) {
					CCM[j] = (CCM_Calib[i + 1][j] * alpha) + (CCM_Calib[i][j] * (1 - alpha));
				}
			}
		}
	}
}

cv::Mat Calibration::CCM_process(cv::Mat input, int CT, int BIT_SIZE) {
	cv::Mat result(input.rows, input.cols, CV_16SC3);
	int CCM[9];
	CCM_interpolation(CT, CCM);
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int R = ((float)CCM[0] / 1024) * input.at<Vec3s>(i, j)[2] + ((float)CCM[1] / 1024) * input.at<Vec3s>(i, j)[1] + ((float)CCM[2] / 1024) * input.at<Vec3s>(i, j)[0];
			int G = ((float)CCM[3] / 1024) * input.at<Vec3s>(i, j)[2] + ((float)CCM[4] / 1024) * input.at<Vec3s>(i, j)[1] + ((float)CCM[5] / 1024) * input.at<Vec3s>(i, j)[0];
			int B = ((float)CCM[6] / 1024) * input.at<Vec3s>(i, j)[2] + ((float)CCM[7] / 1024) * input.at<Vec3s>(i, j)[1] + ((float)CCM[8] / 1024) * input.at<Vec3s>(i, j)[0];

			R = MAX(0, R);
			R = MIN(BIT_SIZE - 1, R);

			G = MAX(0, G);
			G = MIN(BIT_SIZE - 1, G);

			B = MAX(0, B);
			B = MIN(BIT_SIZE - 1, B);

			result.at<Vec3s>(i, j)[2] = R;
			result.at<Vec3s>(i, j)[1] = G;
			result.at<Vec3s>(i, j)[0] = B;
		}
	}
	return result;
}

cv::Mat Calibration::LSC_calibration(cv::Mat input, int BIT_SIZE) {
	ifstream file;
	float value;
	cv::Mat result = input.clone();
	file.open("LSC_GnTable.txt");
	if (file.is_open()) {
		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j++) {
				file >> value;
				result.at<ushort>(i, j) = int(min(float(BIT_SIZE - 1), float(input.at<ushort>(i, j)) * value));
			}
		}
		return result;
	}
	else {
		cout << "LSC_GnTable not found! Skip LSC calibration." << endl;
		return input;
	}
}

cv::Mat Calibration::process(cv::Mat input, Items item) {
	// switch is better
	if (item == Items::CCM_Apply) {
		return Calibration::CCM_process(input, mCT, mBIT_SIZE);
	}
	if (item == Items::LSC_Apply) {
		return Calibration::LSC_calibration(input, mBIT_SIZE);
	}
}