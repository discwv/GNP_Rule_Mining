#ifndef GLCM_H_
#define GLCM_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

const int OUTSIZE = 16;
double** MakeGLCM(cv::Mat image, int radius);
double Contrast(double** glcm);
double Dissimilarity(double** glcm);
double Homogeneity(double** glcm);
double ASM(double** glcm);
double Maximum(double** glcm);
double Entropy(double** glcm);

double Sav_Step(double** glcm);
double Sav_Varience(double** glcm);
double Sav_Entropy(double** glcm);
double Sav_Energy(double** glcm);
double Sav_Homogeneity(double** glcm);
double Sav_3Moment(double** glcm);

#endif