#ifndef ISPTUNINGFILE_H
#define ISPTUNINGFILE_H

struct BNR_parameters {
	int windowSize = 5;
	int panality = 10;
};
const struct BNR_parameters bnr_param;


struct EE_parameters {
	int edgeThr = 16;
	int textureThr = 4;
	float edgeStrength = 0.3;
	float textureStrength = 0.05;
};
const struct EE_parameters ee_param;

struct ISPTuning {
	BNR_parameters bnr() {
		return bnr_param;
	}
	EE_parameters ee() {
		return ee_param;
	}
};
const struct ISPTuning tuningFile;

#endif 
