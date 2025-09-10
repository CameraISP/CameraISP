#include "RawTransfer.h"

Mat RawTransfer::process(RTmethod method) {
    if (method == RTmethod::R2B) {
        const char* cPath = Path.c_str();
        BYTE* Image = (BYTE*)malloc(width * height * 2 * sizeof(BYTE));
        Mat Input(height, width, CV_16UC1);
        FILE* hFileTmp;
        hFileTmp = fopen(cPath, "rb");
        if (hFileTmp == NULL)
        {
            std::cerr << "No such file for RawTransfer!!" << std::endl;
        }
        fread(Image, sizeof(BYTE), width * height * 2, hFileTmp);
        int BLC_Rgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.R_clamp);
        int BLC_Grgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.Gr_clamp);
        int BLC_Gbgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.Gb_clamp);
        int BLC_Bgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.B_clamp);
        if (Bayer_ID == BayerID::RGGB) {
            for (int i = 0; i < Input.rows; i++) {
                for (int j = 0; j < Input.cols; j += 2) {
                    if (i % 2 == 0) {
                        Input.at<ushort>(i, j) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j)] + ((uint16_t)Image[2 * ((i * Input.cols) + j) + 1] << 8)) - blc.R_clamp) * BLC_Rgain) >> 10;
                        Input.at<ushort>(i, j + 1) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j + 1)] + ((uint16_t)Image[2 * ((i * Input.cols) + j + 1) + 1] << 8)) - blc.Gr_clamp) * BLC_Grgain) >> 10;
                    }
                    else {
                        Input.at<ushort>(i, j) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j)] + ((uint16_t)Image[2 * ((i * Input.cols) + j) + 1] << 8)) - blc.Gb_clamp) * BLC_Gbgain) >> 10;
                        Input.at<ushort>(i, j + 1) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j + 1)] + ((uint16_t)Image[2 * ((i * Input.cols) + j + 1) + 1] << 8)) - blc.B_clamp) * BLC_Bgain) >> 10;
                    }
                }
            }
        }
        else if (Bayer_ID == BayerID::BGGR) {
            for (int i = 0; i < Input.rows; i++) {
                for (int j = 0; j < Input.cols; j += 2) {
                    if (i % 2 == 0) {
                        Input.at<ushort>(i, j) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j)] + ((uint16_t)Image[2 * ((i * Input.cols) + j) + 1] << 8)) - blc.B_clamp) * BLC_Bgain) >> 10;
                        Input.at<ushort>(i, j + 1) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j + 1)] + ((uint16_t)Image[2 * ((i * Input.cols) + j + 1) + 1] << 8)) - blc.Gb_clamp) * BLC_Gbgain) >> 10;
                    }
                    else {
                        Input.at<ushort>(i, j) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j)] + ((uint16_t)Image[2 * ((i * Input.cols) + j) + 1] << 8)) - blc.Gr_clamp) * BLC_Grgain) >> 10;
                        Input.at<ushort>(i, j + 1) = (MAX(0, ((uint16_t)Image[2 * ((i * Input.cols) + j + 1)] + ((uint16_t)Image[2 * ((i * Input.cols) + j + 1) + 1] << 8)) - blc.R_clamp) * BLC_Rgain) >> 10;
                    }
                }
            }
        }
        else if (Bayer_ID == BayerID::GRBG) {

        }
        else if (Bayer_ID == BayerID::GBRG) {

        }
        fclose(hFileTmp);
        hFileTmp = NULL;
        free(Image);
        Image = NULL;

        return Input;
    }
    else if (method == RTmethod::T2B) {
        ifstream file;
        Mat Input(height, width, CV_16UC1);
        uint16_t value = 0;
        int BLC_Rgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.R_clamp);
        int BLC_Grgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.Gr_clamp);
        int BLC_Gbgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.Gb_clamp);
        int BLC_Bgain = (BIT_SIZE << 10) / (BIT_SIZE - blc.B_clamp);
        file.open(Path);
        if (file.is_open()) {
            if (Bayer_ID == BayerID::RGGB) {
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j += 2) {
                        if (i % 2 == 0) {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.R_clamp)) * BLC_Rgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.Gr_clamp)) * BLC_Grgain) >> 10;
                        }
                        else {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.Gb_clamp)) * BLC_Gbgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.B_clamp) * BLC_Bgain)) >> 10;
                        }
                    }
                }
            }
            else if (Bayer_ID == BayerID::BGGR) {
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j += 2) {
                        if (i % 2 == 0) {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.B_clamp)) * BLC_Bgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.Gb_clamp)) * BLC_Gbgain) >> 10;
                        }
                        else {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.Gr_clamp)) * BLC_Grgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.R_clamp) * BLC_Rgain)) >> 10;
                        }
                    }
                }
            }
            else if (Bayer_ID == BayerID::GBRG) {
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j += 2) {
                        if (i % 2 == 0) {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.Gb_clamp)) * BLC_Gbgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.B_clamp)) * BLC_Bgain) >> 10;
                        }
                        else {
                            file >> value;
                            Input.at<ushort>(i, j) = (MAX(0, (value - blc.R_clamp)) * BLC_Rgain) >> 10;
                            file >> value;
                            Input.at<ushort>(i, j + 1) = (MAX(0, (value - blc.Gr_clamp) * BLC_Grgain)) >> 10;
                        }
                    }
                }
            }
            file.close();
            return Input;
        }
        else {
            std::cerr << "No such file for RawTransfer!!" << endl;
        }
    }
}