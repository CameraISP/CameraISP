#ifndef CALIBRATION_H
#define CALIBRATION_H
#include "Common.h"
#include "AWB.h"

#define CCM_SIZE 4


const int CT_CCM[4] = {
	6500,
	5000,
	4150,
	2850
};

// CVI CCM
//int CCM_Calib[4][9] = {
//	{1952,-848,-80,-264,1580,-292,116,-792,1700}, //6500K
//	{1851,-595,-232,-386,1819,-409,111,-898,1811},//5000K
//	{1572,-292,-256,-492,1752,-236,56,-744,1712},//3900K
//	{1688,-608,-56,-396,1528,-108,216,-1152,1960}//3000K
//};

// Raspberry CCM IMX219 (CCM before Gamma)
//int CCM_Calib[4][9] = {
//	{2071, -975, -74, -311, 1893, -556, -120, -934, 2078},//5000K(Jimmy room)
//	{2071, -975, -74, -311, 1893, -556, -120, -934, 2078},//5000K(Jimmy room)
//	{2071, -975, -74, -311, 1893, -556, -120, -934, 2078},//5000K(Jimmy room)
//	{2071, -975, -74, -311, 1893, -556, -120, -934, 2078}//5000K(Jimmy room)
//};

// Raspberry CCM IMX219 (CCM after Gamma)
const int CCM_Calib[4][9] = {
	{1659,-537,-99,-537,1901,-339,5,-1110,2129},//D65
	{1992,-1040,71,-713,2128,-394,-25,-942,1991},//D50
	{1915,-767,-125,-300,1932,-608,-117,-1086,2226},//CWF(4150K)
	{2412,-1248,-139,-593,2310,-691,-445,-206,1675}//A(2850K)
};

enum Items {
	CCM_Apply = 0,
	LSC_Apply
};

class Calibration {
public:
	Calibration(int CT, int BIT_SIZE): mCT(CT), mBIT_SIZE(BIT_SIZE){}
	cv::Mat process(cv::Mat input ,Items item);

private:
	void CCM_interpolation(int CT, int* CCM);
	//int CT_Get(WB_GAIN wbgain, int* CT) {};
	cv::Mat CCM_process(cv::Mat input, int CT, int BIT_SIZE);
	cv::Mat LSC_calibration(cv::Mat input, int BIT_SIZE);

private:
	int mCT;
	int mBIT_SIZE;
};


#endif
