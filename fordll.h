
//方案一：传入图片RGB数据byte[],返回图片清晰度 float clarity

#pragma once
#ifndef FORDLL_H_
#define FORDLL_H_
extern "C"  __declspec(dllexport) unsigned char *StartEn(unsigned char *ImageBuffer, int imageWedth, int imageHeight, unsigned char *returnData);
#endif