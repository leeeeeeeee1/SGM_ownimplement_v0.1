#include "SGM.h"
void SGM::loaddata(Mat leftimage, Mat rightimage, unsigned short * leftdata, unsigned short *rightdata)
{ 
	time_t start=time_t(nullptr);
	_Height = leftimage.rows;
	_Width = rightimage.cols;
	if (rightimage.rows != _Height || rightimage.cols != _Width)
		throw "左右图片尺寸不一致\n";
    //转换为32为 并转换成灰度图

	Mat left_gray, right_gray;
	cvtColor(leftimage, left_gray, COLOR_BGR2GRAY);
	cvtColor(rightimage, right_gray, COLOR_BGR2GRAY);
	left_gray.convertTo(left_gray, CV_16UC1);
	right_gray.convertTo(right_gray, CV_16UC1);
	leftdata = new unsigned short[_Width*_Height];
	rightdata = new unsigned short[_Width*_Height];
	for(int y=0;y<_Height;y++)
		for (int x = 0; x < _Width; x++)
		{
			leftdata[y*_Width + x] =left_gray.at<unsigned short>(y, x);
			rightdata[y*_Width + x] = right_gray.at<unsigned short>(y, x);
		}
	time_t finish=time_t(nullptr);
	cout << "*********数据转换结束，耗时" << (finish - start )<< "秒" << endl;
}
//sonbel 因子卷积求梯度
void SGM::sobelcap()
{
	time_t start = time_t(nullptr);
	_Soble_leftimg = new  unsigned short[_Width*_Height];
	_Soble_rightimg = new unsigned short[_Width*_Height];
	for (int y = 0; y < _Height; y++)
	{
		if (y != 0 && y != _Height - 1)
		{
		 //y-1,x-1 y-1,x+1 y,x-1 y,x+1 ,y+1,x-1 y+1,x+1
			for (int x = 0; x < _Width-1; x++)
			{
				if (x != 0 && x != _Width - 1)
				{
					int tempL = 1 * _Originaldata_leftimg[(y - 1)*_Width + x + 1] + 2 * _Originaldata_leftimg[y*_Width + x + 1] + 1 * _Originaldata_leftimg[(y + 1)*_Width + x + 1]
						- (_Originaldata_leftimg[(y - 1)*_Width + x - 1] + 2 * _Originaldata_leftimg[y*_Width + x - 1] + 1 * _Originaldata_leftimg[(y + 1)*_Width + x - 1]);
					int tempR = 1 * _Originaldata_rightimg[(y - 1)*_Width + x + 1] + 2 * _Originaldata_rightimg[y*_Width + x + 1] + 1 * _Originaldata_rightimg[(y + 1)*_Width + x + 1]
						- (_Originaldata_rightimg[(y - 1)*_Width + x - 1] + 2 * _Originaldata_rightimg[y*_Width + x - 1] + 1 * _Originaldata_rightimg[(y + 1)*_Width + x - 1]);
					if (tempL < -_SoblecapValue)
					{
						tempL = 0;
						tempR = 0;
					}
					else if (tempL > _SoblecapValue)
					{
						tempL *= 2;
						tempR *= 2;
					}
					else
					{
						tempL += _SoblecapValue;
						tempR += _SoblecapValue;
					}
					_Soble_leftimg[y*_Width + x] = tempL;
					_Soble_rightimg[y*_Width + x] = tempR;

				}
				else
				{
					_Soble_leftimg[y*_Width + x] = _Originaldata_leftimg[y*_Width+x];
					_Soble_rightimg[y*_Width + x] =_Originaldata_rightimg[y*_Width+x];
				}
			}	
		}
		else
		{
			int i = 0;
			while (i < _Width)
			{
				_Soble_leftimg[y*_Width + x] = _Originaldata_leftimg[y*_Width + x];
				_Soble_rightimg[y*_Width + x] = _Originaldata_rightimg[y*_Width + x];
				++i;
			}
		}
	}
	delete [] _Originaldata_leftimg;
	delete [] _Originaldata_rightimg;
	time_t finish = time_t(nullptr);
	cout << "********图片梯度计算完成，耗时" << finish - start << "秒" << endl;

}

Mat SGM::compute(const Mat leftimage,const  Mat rightimage)
{
	
	loaddata(leftimage, rightimage, _Originaldata_leftimg, _Originaldata_rightimg);
	sobelcap();
	
	return Mat();






}