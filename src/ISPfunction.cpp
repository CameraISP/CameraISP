#include "ISPfunction.h"

void ISPConfig::loadConfig(std::string configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open config, using default config.json" << std::endl;
        std::ifstream file("config.json");
    }
    nlohmann::json cfg;
    file >> cfg;
    // read json
    rawPath = cfg.value("PATH", "sample.txt");
    Img_width = cfg.value("IMG_WIDTH", 1920);
    Img_height = cfg.value("IMG_HEIGHT", 1080);
    BIT_SIZE = cfg.value("BIT_SIZE", 1024);
    blc.B_clamp = cfg.value("BLC_B", 64);
    blc.Gb_clamp = cfg.value("BLC_Gb", 64);
    blc.Gr_clamp = cfg.value("BLC_Gr", 64);
    blc.R_clamp = cfg.value("BLC_R", 64);
    Bayer_ID = cfg.value("BAYER_ID", BayerID::BGGR);
    CT = cfg.value("CT", 5000);
    Gamma_coef = cfg.value("GMA", 2.2);
}

void ISPPipeline::run() {
    

    //ISP pipeline start
    RawTransfer RawTF(mSensorConfig.rawPath, mSensorConfig.Img_width, mSensorConfig.Img_height, mSensorConfig.blc, mSensorConfig.Bayer_ID, mSensorConfig.BIT_SIZE);
    mProcessResult.Bayer_raw = RawTF.process(RTmethod::T2B);

    //--------------------------- Bayer domain << Start >> --------------------------------//
    Calibration Calib(mSensorConfig.CT, mSensorConfig.BIT_SIZE);
    mProcessResult.LSC_result = Calib.process(mProcessResult.Bayer_raw, Items::LSC_Apply);

    AWB awb(mProcessResult.LSC_result, mSensorConfig.Bayer_ID, mSensorConfig.BIT_SIZE);
    mProcessResult.AWB_result = awb.process(AWBmethod::GreyWorld);
 
    BNR bnr(mProcessResult.AWB_result, mSensorConfig.Bayer_ID, mTuningFile.bnr().windowSize, mTuningFile.bnr().panality);
    mProcessResult.BNR_result = bnr.process(BNRmethod::Guided);
    //--------------------------- Bayer domain << End >> --------------------------------//

    demosaic Demosaic(mProcessResult.BNR_result, mSensorConfig.Bayer_ID);
    mProcessResult.Demosaic_result = Demosaic.process(DemosaicMethod::AHD);

    //--------------------------- RGB domain << Start >> --------------------------------//
    mProcessResult.CCM_result = Calib.process(mProcessResult.Demosaic_result, Items::CCM_Apply);

    TM Gamma(mProcessResult.CCM_result, mSensorConfig.Gamma_coef, mSensorConfig.BIT_SIZE);
    mProcessResult.Gamma_result = Gamma.process();

    //--------------------------- RGB domain << End >> --------------------------------//

    vector<Mat> sYUV;
    mProcessResult.YUV_result = com::RGB2YUV(mProcessResult.Gamma_result, mSensorConfig.BIT_SIZE);
    sYUV = com::Separate_YUV(mProcessResult.YUV_result);

    //--------------------------- YUV domain << Start >> --------------------------------//

    EE Edge_enhancement(sYUV[0], mTuningFile.ee().edgeThr, mTuningFile.ee().textureThr, mTuningFile.ee().edgeStrength, mTuningFile.ee().textureStrength);
    mProcessResult.EE_result = Edge_enhancement.process();

    sYUV[0] = mProcessResult.EE_result.clone();
    mProcessResult.YUV_result = com::Merge_YUV(sYUV);
    //--------------------------- YUV domain << End >> --------------------------------//

    mProcessResult.RGB_result = com::YUV2RGB(mProcessResult.YUV_result);
}
