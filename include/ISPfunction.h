#ifndef ISPFUNCTION_H
#define ISPFUNCTION_H

#include "Common.h"
#include "json.hpp"
#include "ISPtuningFile.h"
#include "RawTransfer.h"
#include "Demosaic.h"
#include "Calibration.h"
#include "TM.h"
#include "EE.h"
#include "BNR.h"
#include "AWB.h"

struct ISPConfig {
public:
    std::string rawPath;
    int Img_width;
    int Img_height;
    int BIT_SIZE;
    BLC blc; // R,G,G,B
    BayerID Bayer_ID;
    int CT;
    float Gamma_coef;
public:
    void loadConfig(std::string configPath);
};

struct ProcessResult {
    Mat Bayer_raw;
    Mat LSC_result;
    Mat AWB_result;
    Mat BNR_result;
    Mat Demosaic_result;
    Mat CCM_result;
    Mat Gamma_result;
    Mat YUV_result;
    Mat EE_result;
    Mat RGB_result;
};


class ISPPipeline {
public:

    void setConfig(ISPConfig config) { mSensorConfig = config; }
    void run();
    ProcessResult getResult() {
        return mProcessResult;
    }
private:
    ProcessResult mProcessResult;
    ISPConfig mSensorConfig;
    ISPTuning mTuningFile;
};

#endif


