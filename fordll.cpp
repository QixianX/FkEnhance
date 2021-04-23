
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/imgproc/imgproc_c.h>
//#include <opencv2/highgui/highgui.hpp>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"

//#include <ctime>
#include "fordll.h"

using namespace std;
using namespace cv;
Mat CalculateEntropyBlue(Mat aa, string flag);
Mat CalculateEntropyGreen(Mat aa,Mat blu, string flag);
Mat CalculateEntropyRed(Mat aa,Mat blu,string flag);
float calH(float a,float b,float c, Mat GrayImg);
bool check(float a,float b,float c);


Mat CalculateEntropyBlue(Mat aa, string flag)
{
	const int rows = aa.rows;
	const int cols = aa.cols;
	int c_x = cols / 2, c_y = rows / 2;
	const float d = sqrt((float)c_x*c_x + c_y * c_y + 1);
	float g;
	for (int row = 0; row < rows; ++row)
	{
		uchar *data = aa.ptr<uchar>(row);
		for (int col = 0; col < cols; ++col)
		{
			data[col] = 0;
			//float r = (float)((row - c_y)*(row - c_y) + (col - c_x)*(col - c_x)) / (d*d);
			//float a_min = 1.5;
			//g = 1 + a_min * r;
			//float res = (data[col] * g);
			//////if (col == 1695 && row == 1127)
			//if (res > 255.0f)
			//	res = 255.0f;
			//data[col] = res;
			//if (data[col] < (55.0f + 25.0f*r))
			//	data[col] = 0;
		}
	}
	return aa;
}

Mat CalculateEntropyGreen(Mat aa,Mat blu, string flag)
{
	const int rows = aa.rows;
	const int cols = aa.cols;
	int c_x = cols / 2, c_y = rows / 2;
	const float d = sqrt((float)c_x*c_x + c_y * c_y + 1);
	float g;
	for (int row = 0; row < rows; ++row)
	{
		uchar *data = aa.ptr<uchar>(row);
		uchar *bd = blu.ptr<uchar>(row);
		for (int col = 0; col < cols; ++col)
		{
			//if (bd[col] >= 60 || bd[col] <= 15)
			//	data[col] = 0;
			//else
			//{
				float r = (float)((row - c_y)*(row - c_y) + (col - c_x)*(col - c_x)) / (d*d);
				float a_min = 1.2;
				g = 1 + a_min * r;
				float res = (data[col] * g);
				////if (col == 1695 && row == 1127)
				if (res > 255.0f)
					res = 255.0f;
				data[col] = res;
				if (data[col] < (60.0f + 40.0f*r))
					data[col] = 0;
			/*}*/
		}
	}
	return aa;
}


Mat CalculateEntropyRed(Mat aa,Mat blu,string flag)
{
	//cout << "into cal" << endl;
    //Mat aa = img.clone(); 
	//Mat aa = img;
	//imwrite(name + ".jpg", aa);
	//Mat aa = img.clone();
	const int rows = aa.rows;
	const int cols = aa.cols;

	
	
 
	//Mat num = aa.clone();
    int c_x = cols/2,c_y = rows/2;
    const float d = sqrt((float)c_x*c_x+c_y*c_y+1);  
	//const float a_min = 1;
	//float a = 0, b = 0, c = 0;
	//float a_min = 0, b_min = 0, c_min = 0;
	//float delta = 2;
	float g;
	if (flag == "re")
	{	
		Mat resize_img;
		resize(aa, resize_img, Size(cols / 40, rows / 40), 0, 0, INTER_LINEAR);
		//cout << "intp re" << endl;
		float a = 0, b = 0, c = 0;
		float a_min = 0, b_min = 0, c_min = 0;
		float delta = 2;
		float hmin = calH(a, b, c, resize_img);
		cout << "finish cal h 1" << endl;

		while (delta > 1 / 256)
		{
			float a_temp = a + delta;
			if (check(a_temp, b, c))
			{
				float h = calH(a_temp, b, c, resize_img);
				if (hmin > h)
				{
					a_min = a_temp;
					b_min = b;
					c_min = c;
					hmin = h;
				}
			}
			a_temp = a - delta;
			if (check(a_temp, b, c))
			{
				float h = calH(a_temp, b, c, resize_img);
				if (hmin > h)
				{
					a_min = a_temp;
					b_min = b;
					c_min = c;
					hmin = h;
				}
			}
			float b_temp = b + delta;
			if (check(a, b_temp, c))
			{
				float h = calH(a, b_temp, c, resize_img);
				if (hmin > h)
				{
					a_min = a;
					b_min = b_temp;
					c_min = c;
					hmin = h;
				}
			}
			b_temp = b - delta;
			if (check(a, b_temp, c))
			{
				float h = calH(a, b_temp, c, resize_img);
				if (hmin > h)
				{
					a_min = a;
					b_min = b_temp;
					c_min = c;
					hmin = h;
				}
			}
			float c_temp = c + delta;
			if (check(a, b, c_temp))
			{
				float h = calH(a, b, c_temp, resize_img);
				if (hmin > h)
				{
					a_min = a;
					b_min = b;
					c_min = c_temp;
					hmin = h;
				}
			}
			c_temp = c - delta;
			if (check(a, b, c_temp))
			{
				float h = calH(a, b, c_temp, resize_img);
				if (hmin > h)
				{
					a_min = a;
					b_min = b;
					c_min = c_temp;
					hmin = h;
				}
			}
			delta = delta / 2.0f;
		}
	}
	//if (flag == "re")
	//	if (a_min > 0.7)
	//		a_min = 0.7;
    for(int row=0;row<rows;++row)
    {
        uchar *data = aa.ptr<uchar>(row);
		uchar *bd = blu.ptr<uchar>(row);
		//uchar *val = num.ptr<uchar>(row);
        //uchar *value = result.ptr<uchar>(row);
        for(int col=0;col<cols;++col)
        {
			/*if (bd[col] >= 60||bd[col]<=15)
				data[col] = 0;
			else
			{*/
				//clock_t rst = clock();
				//float r = sqrt((float)((row-c_y)*(row-c_y)+(col-c_x)*(col-c_x)))/d;
				float r = (float)((row - c_y)*(row - c_y) + (col - c_x)*(col - c_x)) / (d*d);
				//float r2 = r*r;

				//float r4 = r2*r2;
				//float r6 = r2*r2*r2;
				//float g = 1+a_min*r2+b_min*r4+c_min*r6;
				//if (flag == "re")
				//{
				//	//cout << a_min << "***" << b_min << "***" << c_min << "***" << endl;
					/*g = 1 + a_min * r + b_min * r*r + c_min * r*r*r;*/
				//	//g = 1;
				//}
				//else
				//{
				float a_min = 1.5;
				g = 1 + a_min * r;
				/*}*/

				//int g = (int)1 + 2 * r2;
				////clock_t rend = clock();
				////cout << "rjisuan:" << rend - rst << "ms\n";
				//// this will cause overflow 
				//// ToDo: The image should be normalized to the original brightness
				//int res = (int)(data[col] * g);
				////if (col == 1695 && row == 1127)
				//if (res > 255)
				//	res = 255;
				//data[col] = res;



				//val[col] = g;
				//clock_t rend = clock();
				//cout << "rjisuan:" << rend - rst << "ms\n";
				// this will cause overflow 
				// ToDo: The image should be normalized to the original brightness
				float res = (data[col] * g);
				////if (col == 1695 && row == 1127)
				if (res > 255.0f)
					res = 255.0f;
				data[col] = res;
				//if (data[col]<44.0f)
				//	data[col] = 0;
				if (data[col] < (55.0f + 25.0f*r))
					data[col] = 0;
			/*}*/
        }
    }
	//FileStorage fs("1.5.xml", FileStorage::WRITE);
	//fs << "vocabulary" << num;
	//fs.release();

	return aa;
}
 

//void get_maping_graph(char *path, int length)
//{
//	//std::ifstream fp(path);//只读文件 ，也可由下一句替代
//	std::fstream fp(path, std::ios::in);//只读文件
//	int temp;
//	if (!fp.is_open())
//	{
//		printf("could not load file: %s\n", path);
//		return;
//	}
//
//	for (int i = 0; i < length; i++)
//	{
//		fp >> temp;
//		g_maping_graph[i] = temp;
//	}
//	printf("get maping_graph done .\n ");
//	fp.close();


void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}


//int main(int argc, char* argv[])
extern "C"  __declspec(dllexport) unsigned char *StartEn(unsigned char *ImageBuffer, int imageWedth, int imageHeight, unsigned char *returnData)
{
	// int bitSize = imageWedth*imageHeight*3;
	Mat img = Mat(imageHeight, imageWedth, CV_8UC3, ImageBuffer);
	imwrite("D:/imIntodll.jpg", img);
	//cvtColor(img, img, CV_BGR2HSV);
	vector<Mat> channels;
	split(img, channels);
	Mat blue, green, red;
	blue = channels.at(0);
	green = channels.at(1);
	red = channels.at(2);
	string bl, gr, re;

	//clock_t end3 = clock();
	Mat car = CalculateEntropyRed(red,blue,re);
	Mat cag = CalculateEntropyGreen(green,blue,gr);
	Mat cab = CalculateEntropyBlue(blue, bl);
	Mat dstImage;
	vector<Mat> channels2;
	//channels2.push_back(channels.at(0));
	channels2.push_back(cab);
	channels2.push_back(cag);
	channels2.push_back(car);
	//channels2.push_back(channels.at(1));
	// 将图像以数据流的形式放入内存中
	merge(channels2, dstImage);	
	//vector<unsigned char> img_encode;
 //   imencode(".jpg", dstImage, img_encode);
	////long bufsize = imageWedth * imageHeight * 3;
	////unsigned char *encode_data = new unsigned char[bufsize];
	////for (int i = 0; i < bufsize; i++)
	////{
	////	encode_data[i] = img_encode[i];
	////}
	////unsigned char *encode_data = img_encode;
	// //size = img_encode.size();
	// for each(uchar var in img_encode)    //将buf拷贝到C#的byte[]中
	// {
	//	 *returnData = var;
	//	 returnData++;
	// }
	 returnData = dstImage.data;
	 imwrite("D:/afterEnIndll.jpg", dstImage);
	// imwrite("G:/cell/hxdtest/build/re-r/"+ v[1], dstImage);
	//return(unsigned char*)img_encode.data;
	return returnData;
}
 
//检查参数的合法性
bool check(float a,float b,float c)
{
   if ((a>0) && (b==0) && (c==0))
       return true;
   if (a>=0 && b>0 && c==0)
       return true;
   if (c==0 && b<0 && -a<=2*b)
       return true;
   if (c>0 && b*b<3*a*c)
       return true;
   if (c>0 && b*b == 3*a*c && b>=0)
       return true;
   if (c>0 && b*b == 3*a*c && -b>=3*c)
       return true;
   float q_p = (-2*b+sqrt(4*b*b-12*a*c))/(6*c);
   if (c>0 && b*b > 3*a*c && q_p<=0)
       return true;
   float q_d = (-2*b-sqrt(4*b*b-12*a*c))/(6*c);
   if (c>0 && b*b > 3*a*c && q_d>=1)
       return true;
   if (c<0 && b*b > 3*a*c && q_p >=1 && q_d<=0)
       return true;
   return false;
}
 
 
//计算图像熵
float calH(float a,float b,float c,Mat GrayImg)
{
   //Mat resize_img;
   //resize(GrayImg, resize_img, Size(cols / 10, rows / 10), 0, 0, INTER_LINEAR);
   Mat GrayFloatImg(GrayImg.size(),CV_32FC1);
   float histogram[256];
   memset(histogram,0,sizeof(float)*256);
   const int rows = GrayImg.rows;
   const int cols = GrayImg.cols;    

   float c_x = cols/2.0,c_y = rows/2.0;
   const float d = sqrt(c_x*c_x+c_y*c_y+1);

   for(int row=0;row<rows;++row)
   {
       uchar *data = GrayImg.ptr<uchar>(row);
       float *value = GrayFloatImg.ptr<float>(row);
       for(int col=0;col<cols;++col)
       {
           float r=sqrt((row-c_y)*(row-c_y)+(col-c_x)*(col-c_x))/d;
           float r2 = r*r;
           float r4 = r2*r2;
           float r6 = r2*r2*r2;
           float g = 1+a*r2+b*r4+c*r6;
           value[col] = data[col]*g;


	if(value[col] >= 255)
	{
	    value[col] = 255.0;
	    histogram[255]++;
	}
	else 
	{
	    value[col] = (255.0f)*log(1+value[col])/log(256.0f);
	    int k_d = (int)(floor(value[col]));
	    int k_u = (int)(ceil(value[col]));
	    histogram[k_d] += (1+k_d-value[col]);
	    histogram[k_u] += (k_u-value[col]);
	}

       }
   }
  
   float TempHist[256 + 2 * 4];            //    SmoothRadius = 4
   TempHist[0] = histogram[4];                TempHist[1] = histogram[3];    
   TempHist[2] = histogram[2];                TempHist[3] = histogram[1];
   TempHist[260] = histogram[254];            TempHist[261] = histogram[253];
   TempHist[262] = histogram[252];            TempHist[263] = histogram[251];
   memcpy(TempHist + 4, histogram, 256 * sizeof(float));
   //  smooth
   for (int X = 0; X < 256; X++)
       histogram[X] = (TempHist[X] + 2 * TempHist[X + 1] + 3 * TempHist[X + 2] + 4 * TempHist[X + 3] + 5 * TempHist[X + 4] + 4 * TempHist[X + 5] + 3 * TempHist[X + 6] + 2 * TempHist[X + 7]) + TempHist[X + 8] / 25.0f;

   float sum =0;
   for(int i=0;i<256;++i)
   {
       sum += histogram[i];
   }
   float H=0,pk;
   for(int i=0;i<256;++i)
   {
       pk = histogram[i]/sum;
       if(pk!=0)
           H += pk * log(pk); 
   }
   return -H;   
}
