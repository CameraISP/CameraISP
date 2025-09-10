#include"Common.h"



cv::Mat com::ShowOutputImg(Mat input, IMAGE_TYPE ImageType, char input_bit) {
    if (ImageType == IMAGE_TYPE::RGB) {
        int shift_bit = input_bit - 8;
        cv::Mat result(input.rows, input.cols, CV_8UC3);
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                result.at<Vec3b>(i, j)[0] = MIN((uchar)(input.at<Vec3s>(i, j)[0] >> shift_bit),255);
                result.at<Vec3b>(i, j)[1] = MIN((uchar)(input.at<Vec3s>(i, j)[1] >> shift_bit), 255);
                result.at<Vec3b>(i, j)[2] = MIN((uchar)(input.at<Vec3s>(i, j)[2] >> shift_bit), 255);
            }
        }
        return result;
    }
    else if (ImageType == IMAGE_TYPE::GREY) {
        int shift_bit = input_bit - 8;
        Mat result(input.rows, input.cols, CV_8UC1);
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                result.at<uchar>(i, j) = MIN((uchar)(input.at<short>(i, j) >> shift_bit), 255);
            }
        }
        return result;
    }
   
}

cv::Mat com::Conv_filter(Mat input, IMAGE_TYPE ImageType, Mat conv_matrix) {

    if (ImageType == IMAGE_TYPE::RGB) {
        int Mask = conv_matrix.rows;
        Mat result(input.rows, input.cols, CV_16SC3, Scalar(0, 0, 0));

        for (int i = 0; i < result.rows - Mask; i++) {
            for (int j = 0; j < result.cols - Mask; j++) {
                for (int r = 0; r < Mask; r++) {
                    for (int s = 0; s < Mask; s++) {
                        result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[0] = result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[0] + ((float)input.at<Vec3s>(i + r, j + s)[0] * conv_matrix.at<float>(r, s));
                        result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[1] = result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[1] + ((float)input.at<Vec3s>(i + r, j + s)[1] * conv_matrix.at<float>(r, s));
                        result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[2] = result.at<Vec3s>(i + (Mask / 2), j + (Mask / 2))[2] + ((float)input.at<Vec3s>(i + r, j + s)[2] * conv_matrix.at<float>(r, s));
                        // cout << (input.at<Vec3s>(i + r, j + s)[0] * EdgeDetectMask[EdgeDT_ID].at<int>(r, s)) << endl;
                    }
                }
            }
        }

        return result;
    }
    else if (ImageType == IMAGE_TYPE::GREY) {
        int Mask = conv_matrix.rows;
        Mat result(input.rows, input.cols, CV_16SC1, Scalar(0));
        float scale_factor = 0;
        // scale_factor created for range of 1023
        for (int r = 0; r < Mask; r++) {
            for (int s = 0; s < Mask; s++) {
                scale_factor = scale_factor + abs(conv_matrix.at<float>(r, s));
            }
        }
        scale_factor = (scale_factor / 2) * 1023; // divide 2 because result is summed by + and -
        int max = 0;
        for (int i = 0; i < result.rows - Mask; i++) {
            for (int j = 0; j < result.cols - Mask; j++) {
                for (int r = 0; r < Mask; r++) {
                    for (int s = 0; s < Mask; s++) {
                        result.at<int16_t>(i + (Mask / 2), j + (Mask / 2)) = result.at<int16_t>(i + (Mask / 2), j + (Mask / 2)) + (input.at<uchar>(i + r, j + s) * conv_matrix.at<float>(r, s));
                    }
                }
            }
        }
        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                result.at<int16_t>(i, j) = (float)result.at<int16_t>(i, j) * 1023 / scale_factor; // scale EdgeDT to range of 1023
            }
        }
        return result;
    }
}


cv::Mat com::RGB2YUV(Mat input, int BIT_SIZE) {
    Mat YUV(input.rows, input.cols, CV_8UC3);
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            int R = input.at<Vec3s>(i, j)[2];
            int G = input.at<Vec3s>(i, j)[1];
            int B = input.at<Vec3s>(i, j)[0];
            YUV.at<Vec3b>(i, j)[0] = ((R2Ymask[0] * R) + (R2Ymask[1] * G) + (R2Ymask[2] * B)) / (BIT_SIZE / 256);
            YUV.at<Vec3b>(i, j)[1] = (((R2Ymask[3] * R) + (R2Ymask[4] * G) + (R2Ymask[5] * B)) / (BIT_SIZE / 256)) + 128;
            YUV.at<Vec3b>(i, j)[2] = (((R2Ymask[6] * R) + (R2Ymask[7] * G) + (R2Ymask[8] * B)) / (BIT_SIZE / 256)) + 128;
        }
    }
    return YUV;
}


cv::Mat com::YUV2RGB(Mat input) {
    Mat RGB(input.rows, input.cols, CV_8UC3);
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            int Y = (int)input.at<Vec3b>(i, j)[0];
            int U = (int)input.at<Vec3b>(i, j)[1] - 128;
            int V = (int)input.at<Vec3b>(i, j)[2] - 128;
            RGB.at<Vec3b>(i, j)[2] = MAX(0, MIN(255, Y + (Y2Rmask[0] * V)));
            RGB.at<Vec3b>(i, j)[1] = MAX(0, MIN(255, Y + (Y2Rmask[1] * U + Y2Rmask[2] * V)));
            RGB.at<Vec3b>(i, j)[0] = MAX(0, MIN(255, Y +( Y2Rmask[3] * U)));
        }
    }
    return RGB;
}

std::vector<cv::Mat> com::Separate_YUV(Mat input) {
    vector<Mat> result;
    Mat result_Y(input.rows,input.cols,CV_8U), result_U(input.rows, input.cols, CV_8U), result_V(input.rows, input.cols, CV_8U);
    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            result_Y.at<uchar>(i, j) = input.at<Vec3b>(i, j)[0];
            result_U.at<uchar>(i,j) = input.at<Vec3b>(i, j)[1];
            result_V.at<uchar>(i,j) = input.at<Vec3b>(i, j)[2];
        }
    }
    result.push_back(result_Y);
    result.push_back(result_U);
    result.push_back(result_V);

    return result;
}

cv::Mat com::Merge_YUV(vector<Mat> input) {
    Mat result(input[0].rows, input[0].cols, CV_8UC3);
    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            result.at<Vec3b>(i, j)[0] = input[0].at<uchar>(i, j);
            result.at<Vec3b>(i, j)[1] = input[1].at<uchar>(i, j);
            result.at<Vec3b>(i, j)[2] = input[2].at<uchar>(i, j);
        }
    }
    return result;
}