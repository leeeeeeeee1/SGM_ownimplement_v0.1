#include "SGM.h"
void SGM::loaddata(Mat leftimage, Mat rightimage, unsigned short * leftdata, unsigned short *rightdata)
{ 
	time_t start=time_t(nullptr);
	_Height = leftimage.rows;
	_Width = rightimage.cols;
	if (rightimage.rows != _Height || rightimage.cols != _Width)
		throw "����ͼƬ�ߴ粻һ��\n";
    //ת��Ϊ32Ϊ ��ת���ɻҶ�ͼ

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
	cout << "*********����ת����������ʱ" << (finish - start )<< "��" << endl;
}
//sonbel ���Ӿ�����ݶ�
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
			int x = 0;
			while (x < _Width)
			{
				_Soble_leftimg[y*_Width + x] = _Originaldata_leftimg[y*_Width + x];
				_Soble_rightimg[y*_Width + x] = _Originaldata_rightimg[y*_Width + x];
				++x;
			}
		}
	}
	delete [] _Originaldata_leftimg;
	delete [] _Originaldata_rightimg;
	time_t finish = time_t(nullptr);
	cout << "********ͼƬ�ݶȼ�����ɣ���ʱ" << finish - start << "��" << endl;

}
//ע�� ǧ��ע�� ���㺣���������ڴ��۾ۺϵ���һ�� ����ۺϵĴ��� �� ���������һ��֮�ͣ� 
//�����˹��̣� �ȼ���ֵ ����sobel��Censusͼ �ټ��㵥��Ĵ��� ��SAD���ھۺ� �ٶ�̬�Ż� ���WTA
void SGM::addCensus()
{
	Ad_Census_LeftImg = new unsigned short[_Width*_Height];
	Ad_Census_LeftImg = new unsigned short[_Width*_Height];

	for (int y = 0; y < _Height; y++)
	{
	
			unsigned short * Ad_Census_Leftrow = Ad_Census_LeftImg + y*_Width;
			unsigned short * Ad_Census_Rightrow = Ad_Census_RightImg + y*_Width;
			for (int x = 0; x < _Width; x++)
			{
				//һ����� ע�� û�б�Ҫר�Ÿ�����߽����� �����߽�Ĳ���ֱ����Ϊ�Ǳ���С ����Ϊ0�ͺ���
				
						int census_value_left, census_value_right;
						for (int window_y = -_CensusWindowRadius; window_y <= _CensusWindowRadius; window_y++)
						{
							unsigned short *Current_Ad_Census_leftrow = Ad_Census_Leftrow + window_y*_Width;//Census����ƫ����
							unsigned short *Current_Ad_Census_rightrow = Ad_Census_Rightrow + window_y*_Width;
							for (int window_x = -_CensusWindowRadius; window_x <= _CensusWindowRadius; window_x++)
							{
								if (y+window_y<_Width&&y+window_y>=0&&
									x+window_x<_Width&&x+window_x>=0)//�����߽����� ֱ����Ϊ����С ����Ϊ0�ͺ���
								{
									unsigned short tempL = Current_Ad_Census_leftrow[x + window_x] - Ad_Census_Leftrow[x];
									unsigned short tempR = Current_Ad_Census_rightrow[x + window_x] - Ad_Census_Rightrow[x];
									census_value_left += tempL > 0 ? 1 : 0;
									census_value_right += tempL > 0 ? 1 : 0;
								}
								
							}
						}
					

				}
			
	}

}




Mat SGM::compute(const Mat leftimage,const  Mat rightimage)
{
	
	loaddata(leftimage, rightimage, _Originaldata_leftimg, _Originaldata_rightimg);
	sobelcap();
	
	return Mat();






}