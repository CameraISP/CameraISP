#include "TM.h"

cv::Mat TM::Gamma_process(Mat input, float gma_coef, int BIT_SIZE) {
    Mat result = input.clone();
    float normalize_factor = ((BIT_SIZE - 1) / (pow((BIT_SIZE - 1), 1 / gma_coef)));
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            result.at<Vec3s>(i, j)[0] = pow(input.at<Vec3s>(i, j)[0], 1 / gma_coef) * normalize_factor;
            result.at<Vec3s>(i, j)[1] = pow(input.at<Vec3s>(i, j)[1], 1 / gma_coef) * normalize_factor;
            result.at<Vec3s>(i, j)[2] = pow(input.at<Vec3s>(i, j)[2], 1 / gma_coef) * normalize_factor;
        }
    }
    return result;
}

cv::Mat TM::process() {
    return TM::Gamma_process(input, gma_coef, BIT_SIZE);
}