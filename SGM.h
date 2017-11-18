#pragma once
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>

#define USE_REFINE
#define RIGHT_DISP
using namespace std;
using namespace cv;
class SGM
{
public:
	inline SGM():_SADwindowRadius(4),_CensusWindowRadius(2),_SoblecapValue(25),_MaxDispairity(256)
	{ cout << "使用加载AD_CENSUS的SGM算法" << endl; }
	inline~SGM() {}
	//只留一个公有接口 compute
	Mat compute(const Mat leftimage,const Mat rightimage);

private :
	//参数部分
	int _Height;
	int _Width;
	int _SADwindowRadius;
	int _CensusWindowRadius;
	int _SoblecapValue;
	int _MaxDispairity;
	
	unsigned short * _Originaldata_leftimg;
	unsigned short * _Originaldata_rightimg;

	unsigned short *_Soble_leftimg;
	unsigned short *_Soble_rightimg;

	unsigned short *Ad_Census_Img;
	unsigned short *Ad_Census_Img;

	//私有接口
	void loaddata(Mat leftimage, Mat rightimage, unsigned short * leftdata, unsigned short *rightdata);
	void sobelcap();

};

