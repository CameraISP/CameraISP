#include "ISPfunction.h"

int main(void) {

    ISPConfig ispConfig;
    ispConfig.loadConfig("config.json");
    ISPPipeline simulation;
    simulation.setConfig(ispConfig);
    simulation.run();

    
    imwrite("BayerRaw.bmp", com::ShowOutputImg(simulation.getResult().Bayer_raw, IMAGE_TYPE::GREY, 10));
    imwrite("LSC_calibration.bmp", com::ShowOutputImg(simulation.getResult().LSC_result, IMAGE_TYPE::GREY, 10));
    imwrite("AWB_result.bmp", com::ShowOutputImg(simulation.getResult().AWB_result, IMAGE_TYPE::RGB, 10));
    imwrite("BNR_result.bmp", com::ShowOutputImg(simulation.getResult().BNR_result, IMAGE_TYPE::RGB, 10));
    imwrite("Demosaic_result.bmp", com::ShowOutputImg(simulation.getResult().Demosaic_result, IMAGE_TYPE::RGB, 10));
    imwrite("CCM_result.bmp", com::ShowOutputImg(simulation.getResult().CCM_result, IMAGE_TYPE::RGB, 10));
    imwrite("Gamma_result.bmp", com::ShowOutputImg(simulation.getResult().Gamma_result, IMAGE_TYPE::RGB, 10));
    imwrite("CVITEK_Mask.bmp", simulation.getResult().RGB_result);

}