#include"Demosaic.h"

float demosaic::Alpha_blend(float value_a, float value_b, float ratio_a, float ratio_b) {
	float result;
	if ((ratio_a + ratio_b) == 0) {
		result = (value_a + value_b) / 2;
	}
	else {
		result = ((value_a * ratio_b) + (value_b * ratio_a)) / (ratio_a + ratio_b);
	}	
	return result;
}
cv::Mat demosaic::Nearest(Mat input, BayerID Bayer_ID) {
	cv::Mat result = input.clone();
	if (Bayer_ID == BayerID::RGGB) {

		for (int i = 0; i < input.rows; i += 2) {
			for (int j = 0; j < input.cols; j += 2) {
				//R
				result.at<Vec3s>(i, j)[0] = input.at<Vec3s>(i + 1, j + 1)[0];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gr
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i + 1, j + 1)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i, j)[2];
				//Gb
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i + 1, j + 1)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i, j)[2];
				//B
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[2] = input.at<Vec3s>(i, j)[2];
			}
		}
	}
	else if (Bayer_ID == BayerID::BGGR) {
		for (int i = 0; i < input.rows; i += 2) {
			for (int j = 0; j < input.cols; j += 2) {
				//B
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j)[0];
			}
		}
	}
	else if (Bayer_ID == BayerID::GBRG) {
		for (int i = 0; i < input.rows; i += 2) {
			for (int j = 0; j < input.cols; j += 2) {
				//Gb
				result.at<Vec3s>(i, j)[0] = input.at<Vec3s>(i, j + 1)[0];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j)[1] + input.at<Vec3s>(i + 1, j + 1)[1]) / 2;
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j)[2];
				//B
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j)[1] + input.at<Vec3s>(i + 1, j + 1)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j)[2];
				//R
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j + 1)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j)[1] + input.at<Vec3s>(i + 1, j + 1)[1]) / 2;
				//Gr				
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j + 1)[0];
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j)[1] + input.at<Vec3s>(i + 1, j + 1)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[2] = input.at<Vec3s>(i + 1, j)[2];
			}
		}
	}
	else {
		// Other BayerID
	}

	return result;
}

cv::Mat demosaic::Bilinear(Mat input, BayerID Bayer_ID) {
	Mat result = input.clone();
	if (Bayer_ID == BayerID::BGGR) {
		int16_t tmp_r1, tmp_r2;
		int16_t tmp_g1, tmp_g2;
		int16_t tmp_b1, tmp_b2;
		for (int i = 2; i < input.rows - 2; i += 2) {
			for (int j = 2; j < input.cols - 2; j += 2) {
				//B
				tmp_r1 = (input.at<Vec3s>(i - 1, j - 1)[2] + input.at<Vec3s>(i - 1, j + 1)[2]) / 2;
				tmp_r2 = (input.at<Vec3s>(i + 1, j - 1)[2] + input.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				tmp_g1 = (input.at<Vec3s>(i - 1, j)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				tmp_g2 = (input.at<Vec3s>(i, j - 1)[1] + input.at<Vec3s>(i, j + 1)[1]) / 2;
				result.at<Vec3s>(i, j)[2] = (tmp_r1 + tmp_r2) / 2;
				result.at<Vec3s>(i, j)[1] = (tmp_g1 + tmp_g2) / 2;
				//Gb
				tmp_b1 = (input.at<Vec3s>(i, j)[0] + input.at<Vec3s>(i, j + 2)[0]) / 2;
				tmp_r1 = (input.at<Vec3s>(i + 1, j + 1)[2] + input.at<Vec3s>(i - 1, j + 1)[2]) / 2;
				result.at<Vec3s>(i, j + 1)[0] = tmp_b1;
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = tmp_r1;
				//Gr
				tmp_b1 = (input.at<Vec3s>(i, j)[0] + input.at<Vec3s>(i + 2, j)[0]) / 2;
				tmp_r1 = (input.at<Vec3s>(i + 1, j - 1)[2] + input.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				result.at<Vec3s>(i + 1, j)[0] = tmp_b1;
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = tmp_r1;
				//R
				tmp_b1 = (input.at<Vec3s>(i, j)[0] + input.at<Vec3s>(i, j + 2)[0]) / 2;
				tmp_b2 = (input.at<Vec3s>(i + 2, j)[0] + input.at<Vec3s>(i + 2, j + 2)[0]) / 2;
				tmp_g1 = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 2, j + 1)[1]) / 2;
				tmp_g2 = (input.at<Vec3s>(i + 1, j)[1] + input.at<Vec3s>(i + 1, j + 2)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[1] = (tmp_g1 + tmp_g2) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = (tmp_b1 + tmp_b2) / 2;
			}
		}
	}
	else {
		// Other BayerID
	}

	return result;
}

cv::Mat demosaic::AHD(Mat input, BayerID Bayer_ID) {
	Mat result = input.clone();
	if (Bayer_ID == BayerID::BGGR) {
		// bounding case
		for (int i = 0; i < 2; i += 2) {
			for (int j = 0; j < input.cols; j += 2) {
				//B
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j)[0];
			}
		}
		for (int i = input.rows-2; i < input.rows; i += 2) {
			for (int j = 0; j < input.cols; j += 2) {
				//B
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j)[0];
			}
		}
		for (int i = 2; i < input.rows-2; i += 2) {
			for (int j = 0; j < 2; j += 2) {
				//B
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j)[0];
			}
		}
		for (int i = 2; i < input.rows-2; i += 2) {
			for (int j = input.cols-2; j < input.cols; j += 2) {
				//B
				result.at<Vec3s>(i, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				result.at<Vec3s>(i, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i, j + 1)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = input.at<Vec3s>(i, j)[0];
				result.at<Vec3s>(i + 1, j)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j)[2] = input.at<Vec3s>(i + 1, j + 1)[2];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = (input.at<Vec3s>(i, j + 1)[1] + input.at<Vec3s>(i + 1, j)[1]) / 2;
				result.at<Vec3s>(i + 1, j + 1)[0] = input.at<Vec3s>(i, j)[0];
			}
		}
		//AHD method
		Mat AHD_map = input.clone();
		int AHD_BGR[3];
		int channel = 0;
		for (int i = 2; i < AHD_map.rows - 3; i+=2) {
			for (int j = 2; j < AHD_map.cols - 3; j+=2) {
				int direct_idxValue[8] = {0};
				int B_diff[8], Gb_diff[8], Gr_diff[8], R_diff[8];
				for (int m = 0; m < 2; m++) {
					for (int n = 0; n < 2; n++) {
						int a = i + m;
						int b = j + n;
						if (m == 0 && n == 0) { channel = 0; }
						else if (m == 1 && n == 1) { channel = 2; }
						else { channel = 1; }
						direct_idxValue[0] = direct_idxValue[0] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a - 2, b - 2)[channel]);
						direct_idxValue[1] = direct_idxValue[1] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a - 2, b)[channel]);
						direct_idxValue[2] = direct_idxValue[2] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a - 2, b + 2)[channel]);
						direct_idxValue[3] = direct_idxValue[3] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a, b + 2)[channel]);
						direct_idxValue[4] = direct_idxValue[4] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a + 2, b + 2)[channel]);
						direct_idxValue[5] = direct_idxValue[5] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a + 2, b)[channel]);
						direct_idxValue[6] = direct_idxValue[6] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a + 2, b - 2)[channel]);
						direct_idxValue[7] = direct_idxValue[7] + abs(AHD_map.at<Vec3s>(a, b)[channel] - AHD_map.at<Vec3s>(a, b - 2)[channel]);
					}
				}
				int direct_min = direct_idxValue[0];
				int direct_idx = 0;
				for (int idx = 1; idx < 8; idx++) {
					if (direct_idxValue[idx] < direct_min) {
						direct_min = direct_idxValue[idx];
						direct_idx = idx;
					}
				}
				short B_g[8], B_r[8], Gb_r[8], Gb_b[8], Gr_r[8], Gr_b[8], R_b[8], R_g[8];
				//B's G/R
				B_g[0] = (AHD_map.at<Vec3s>(i - 1, j)[1] + AHD_map.at<Vec3s>(i, j - 1)[1]) / 2;
				B_g[1] = AHD_map.at<Vec3s>(i - 1, j)[1];
				B_g[2] = (AHD_map.at<Vec3s>(i - 1, j)[1] + AHD_map.at<Vec3s>(i, j + 1)[1]) / 2;
				B_g[3] = AHD_map.at<Vec3s>(i, j + 1)[1];
				B_g[4] = (AHD_map.at<Vec3s>(i, j + 1)[1] + AHD_map.at<Vec3s>(i + 1, j)[1]) / 2;
				B_g[5] = AHD_map.at<Vec3s>(i + 1, j)[1];
				B_g[6] = (AHD_map.at<Vec3s>(i, j - 1)[1] + AHD_map.at<Vec3s>(i + 1, j)[1]) / 2;
				B_g[7] = AHD_map.at<Vec3s>(i, j - 1)[1];

				B_r[0] = AHD_map.at<Vec3s>(i - 1, j - 1)[2];
				B_r[1] = (AHD_map.at<Vec3s>(i - 1, j - 1)[2] + AHD_map.at<Vec3s>(i - 1, j + 1)[2]) / 2;
				B_r[2] = AHD_map.at<Vec3s>(i - 1, j + 1)[2];
				B_r[3] = (AHD_map.at<Vec3s>(i - 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				B_r[4] = AHD_map.at<Vec3s>(i + 1, j + 1)[2];
				B_r[5] = (AHD_map.at<Vec3s>(i + 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j - 1)[2]) / 2;
				B_r[6] = AHD_map.at<Vec3s>(i + 1, j - 1)[2];
				B_r[7] = (AHD_map.at<Vec3s>(i - 1, j - 1)[2] + AHD_map.at<Vec3s>(i + 1, j - 1)[2]) / 2;

				//Gb's R/B
				Gb_r[0] = (AHD_map.at<Vec3s>(i - 1, j - 1)[2] + AHD_map.at<Vec3s>(i - 1, j + 1)[2]) / 2;
				Gb_r[1] = AHD_map.at<Vec3s>(i - 1, j + 1)[2];
				Gb_r[2] = (AHD_map.at<Vec3s>(i - 1, j + 1)[2] + AHD_map.at<Vec3s>(i - 1, j + 3)[2]) / 2;
				Gb_r[3] = (AHD_map.at<Vec3s>(i - 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gb_r[4] = (AHD_map.at<Vec3s>(i + 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 3)[2]) / 2;
				Gb_r[5] = AHD_map.at<Vec3s>(i + 1, j + 1)[2];
				Gb_r[6] = (AHD_map.at<Vec3s>(i + 1, j - 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gb_r[7] = (AHD_map.at<Vec3s>(i - 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;

				Gb_b[0] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i - 2, j)[0]) / 2;
				Gb_b[1] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				Gb_b[2] = (AHD_map.at<Vec3s>(i - 2, j + 2)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				Gb_b[3] = AHD_map.at<Vec3s>(i, j + 2)[0];
				Gb_b[4] = (AHD_map.at<Vec3s>(i + 2, j + 2)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				Gb_b[5] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				Gb_b[6] = (AHD_map.at<Vec3s>(i + 2, j)[0] + AHD_map.at<Vec3s>(i, j)[0]) / 2;
				Gb_b[7] = AHD_map.at<Vec3s>(i, j)[0];

				//Gr's R/B
				Gr_r[0] = (AHD_map.at<Vec3s>(i + 1, j - 1)[2] + AHD_map.at<Vec3s>(i - 1, j - 1)[2]) / 2;
				Gr_r[1] = (AHD_map.at<Vec3s>(i + 1, j - 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gr_r[2] = (AHD_map.at<Vec3s>(i - 1, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gr_r[3] = AHD_map.at<Vec3s>(i + 1, j + 1)[2];
				Gr_r[4] = (AHD_map.at<Vec3s>(i + 3, j + 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gr_r[5] = (AHD_map.at<Vec3s>(i + 1, j - 1)[2] + AHD_map.at<Vec3s>(i + 1, j + 1)[2]) / 2;
				Gr_r[6] = (AHD_map.at<Vec3s>(i + 3, j - 1)[2] + AHD_map.at<Vec3s>(i + 1, j - 1)[2]) / 2;
				Gr_r[7] = AHD_map.at<Vec3s>(i + 1, j - 1)[2];

				Gr_b[0] = (AHD_map.at<Vec3s>(i, j - 2)[0] + AHD_map.at<Vec3s>(i, j)[0]) / 2;
				Gr_b[1] = AHD_map.at<Vec3s>(i, j)[0];
				Gr_b[2] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				Gr_b[3] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i + 2, j)[0]) / 2;
				Gr_b[4] = (AHD_map.at<Vec3s>(i + 2, j)[0] + AHD_map.at<Vec3s>(i + 2, j + 2)[0]) / 2;
				Gr_b[5] = AHD_map.at<Vec3s>(i + 2, j)[0];
				Gr_b[6] = (AHD_map.at<Vec3s>(i + 2, j - 2)[0] + AHD_map.at<Vec3s>(i + 2, j)[0]) / 2;
				Gr_b[7] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i + 2, j)[0]) / 2;

				//R's G/B
				R_g[0] = (AHD_map.at<Vec3s>(i, j + 1)[1] + AHD_map.at<Vec3s>(i + 1, j)[1]) / 2;
				R_g[1] = AHD_map.at<Vec3s>(i, j + 1)[1];
				R_g[2] = (AHD_map.at<Vec3s>(i, j + 1)[1] + AHD_map.at<Vec3s>(i + 1, j + 2)[1]) / 2;
				R_g[3] = AHD_map.at<Vec3s>(i + 1, j + 2)[1];
				R_g[4] = (AHD_map.at<Vec3s>(i + 1, j + 2)[1] + AHD_map.at<Vec3s>(i + 2, j + 1)[1]) / 2;
				R_g[5] = AHD_map.at<Vec3s>(i + 2, j + 1)[1];
				R_g[6] = (AHD_map.at<Vec3s>(i + 1, j)[1] + AHD_map.at<Vec3s>(i + 2, j + 1)[1]) / 2;
				R_g[7] = AHD_map.at<Vec3s>(i + 1, j)[1];

				R_b[0] = AHD_map.at<Vec3s>(i, j)[0];
				R_b[1] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i, j + 2)[0]) / 2;
				R_b[2] = AHD_map.at<Vec3s>(i, j + 2)[0];
				R_b[3] = (AHD_map.at<Vec3s>(i, j + 2)[0] + AHD_map.at<Vec3s>(i + 2, j + 2)[0]) / 2;
				R_b[4] = AHD_map.at<Vec3s>(i + 2, j + 2)[0];
				R_b[5] = (AHD_map.at<Vec3s>(i + 2, j + 2)[0] + AHD_map.at<Vec3s>(i + 2, j)[0]) / 2;
				R_b[6] = AHD_map.at<Vec3s>(i + 2, j)[0];
				R_b[7] = (AHD_map.at<Vec3s>(i, j)[0] + AHD_map.at<Vec3s>(i + 2, j)[0]) / 2;

				//B
				result.at<Vec3s>(i, j)[2] = B_r[direct_idx];
				result.at<Vec3s>(i, j)[1] = B_g[direct_idx];
				//Gb
				result.at<Vec3s>(i, j + 1)[0] = Gb_b[direct_idx];
				result.at<Vec3s>(i, j + 1)[2] = Gb_r[direct_idx];
				//Gr
				result.at<Vec3s>(i + 1, j)[0] = Gr_b[direct_idx];
				result.at<Vec3s>(i + 1, j)[2] = Gr_r[direct_idx];
				//R
				result.at<Vec3s>(i + 1, j + 1)[1] = R_g[direct_idx];
				result.at<Vec3s>(i + 1, j + 1)[0] = R_b[direct_idx];
			}
		}
	}
	else {
		// Other BayerID
	}
	return result;
}

cv::Mat demosaic::process(DemosaicMethod Dmethod) {
	if (Dmethod == DemosaicMethod::AHD) {
		return demosaic::AHD(input, Bayer_ID);
	}
	else if (Dmethod == DemosaicMethod::NEAREAST) {
		return demosaic::Nearest(input, Bayer_ID);
	}
	else if (Dmethod == DemosaicMethod::BILINEAR) {
		return demosaic::Bilinear(input, Bayer_ID);
	}
}