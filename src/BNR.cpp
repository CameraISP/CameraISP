#include "BNR.h"


GF_coef BNR::Guided_filter(Mat roi, Mat guided, float panality) {
	// setting parameters regarding guided filter formula
	float mean_I = 0, mean_P = 0, corr_II = 0, corr_IP = 0;
	int size = roi.rows * roi.cols;
	// sum value and get mean value
	for (int i = 0; i < roi.rows; i++) {
		for (int j = 0; j < roi.cols; j++) {
			mean_P = mean_P + roi.at<float>(i, j);
			mean_I = mean_I + guided.at<float>(i, j);
			corr_II = corr_II + (guided.at<float>(i, j) * guided.at<float>(i, j));
			corr_IP = corr_IP + (guided.at<float>(i, j) * roi.at<float>(i, j));
		}
	}
	mean_P = mean_P / size;
	mean_I = mean_I / size;
	corr_II = corr_II / size;
	corr_IP = corr_IP / size;
	// step 2
	float var_I, cov_IP;
	var_I = corr_II - mean_I * mean_I;
	cov_IP = corr_IP - mean_I * mean_P;
	// step 3
	GF_coef coef;
	coef.slope_a = cov_IP / (var_I + (panality * panality));
	coef.const_b = mean_P - coef.slope_a * mean_I;

	return coef;
}

cv::Mat BNR::Guided(Mat input, BayerID Bayer_ID, int window_size, float panality) {
	//input will be AWB_result with <Vec3s> type
	Mat BNR_result = input.clone();
	Mat inputR(input.rows/2, input.cols/2, CV_32FC1);
	Mat inputB(input.rows / 2, input.cols / 2, CV_32FC1);
	Mat inputGr(input.rows / 2, input.cols / 2, CV_32FC1);
	Mat inputGb(input.rows / 2, input.cols / 2, CV_32FC1);
	Mat guided_I(input.rows / 2, input.cols / 2, CV_32FC1);
	Mat input_roi;
	Mat guided_roi;
	GF_coef coef;
	if (Bayer_ID == BayerID::RGGB) {

	}
	else if (Bayer_ID == BayerID::BGGR) {
		// put input's R/G/B channel to inputR, inputB and guided_I(G channel)
		float G1, G2;
		for (int i = 0; i < inputR.rows; i++) {
			for (int j = 0; j < inputR.cols; j++) {
				inputR.at<float>(i, j) = (float)input.at<Vec3s>((i + 1) * 2 - 1, (j + 1) * 2 - 1)[2];				
				inputB.at<float>(i, j) = (float)input.at<Vec3s>(i * 2, j * 2)[0];
				inputGb.at<float>(i, j) = (float)input.at<Vec3s>(i * 2, (j + 1) * 2 - 1)[1];
				inputGr.at<float>(i, j) = (float)input.at<Vec3s>((i + 1) * 2 - 1, j * 2)[1];
				G1 = (float)input.at<Vec3s>(i * 2, (j + 1) * 2 - 1)[1];
				G2 = (float)input.at<Vec3s>((i + 1) * 2 - 1, j * 2)[1];
				guided_I.at<float>(i, j) = (G1 + G2) / 2.0;
			}
		}
		for (int i = 0; i < inputR.rows- window_size; i++) {
			for (int j = 0; j < inputR.cols- window_size; j++) {
				Rect roi(j, i, window_size, window_size);
				guided_roi = inputR(roi);
				//BNR at R channel
				input_roi = inputR(roi);				
				coef = Guided_filter(input_roi, guided_roi, panality);
				inputR.at<float>(i + window_size / 2, j + window_size / 2) = MIN(inputR.at<float>(i + window_size / 2, j + window_size / 2) * coef.slope_a + coef.const_b, 1023);
				//BNR at B channel
				guided_roi = inputB(roi);
				input_roi = inputB(roi);
				coef = Guided_filter(input_roi, guided_roi, panality);
				inputB.at<float>(i + window_size / 2, j + window_size / 2) = MIN(inputB.at<float>(i + window_size / 2, j + window_size / 2) * coef.slope_a + coef.const_b, 1023);
				//BNR at Gb channel
				guided_roi = inputGb(roi);
				input_roi = inputGb(roi);
				coef = Guided_filter(input_roi, guided_roi, panality);
				inputGb.at<float>(i + window_size / 2, j + window_size / 2) = MIN(inputGb.at<float>(i + window_size / 2, j + window_size / 2) * coef.slope_a + coef.const_b, 1023);
				//BNR at Gr channel
				guided_roi = inputGr(roi);
				input_roi = inputGr(roi);
				coef = Guided_filter(input_roi, guided_roi, panality);
				inputGr.at<float>(i + window_size / 2, j + window_size / 2) = MIN(inputGr.at<float>(i + window_size / 2, j + window_size / 2) * coef.slope_a + coef.const_b, 1023);
			}
		}
		for (int i = 0; i < inputR.rows; i++) {
			for (int j = 0; j < inputR.cols; j++) {
				BNR_result.at<Vec3s>(i * 2, j * 2)[0] = (short)inputB.at<float>(i, j);
				BNR_result.at<Vec3s>(i * 2, (j + 1) * 2 - 1)[1] = (short)inputGb.at<float>(i, j);
				BNR_result.at<Vec3s>((i + 1) * 2 - 1, j * 2)[1] = (short)inputGr.at<float>(i, j);
				BNR_result.at<Vec3s>((i + 1) * 2 - 1, (j + 1) * 2 - 1)[2] = (short)inputR.at<float>(i, j);
			}
		}
	}
	else if (Bayer_ID == BayerID::GBRG) {
		
	}

	return BNR_result;
}

cv::Mat BNR::process(BNRmethod method) {
	if (method == BNRmethod::Guided) {
		return BNR::Guided(input, Bayer_ID, window_size, panality);
	}
	else if (method == BNRmethod::Gaussian) {
		//
	}
}