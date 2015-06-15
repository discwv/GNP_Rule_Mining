#include "GLCM.h"
//#include <iostream>
#include <cmath>
using namespace cv;

double** MakeGLCM(Mat image, int radius)
{
	//	initialize my glcm
	double** myGlcm = new double*[OUTSIZE];
	for (int i = 0; i < OUTSIZE; i++)
	{
		myGlcm[i] = new double[OUTSIZE];
		for (int j = 0; j < OUTSIZE; j++) myGlcm[i][j] = 0;
	}
	long int count = 0;
	for (int row = radius; row < image.rows - radius; row++)
	{
		for (int col = radius; col < image.cols - radius; col++)
		{

			for (int r = -radius; r <= radius; r++)
			{
				for (int c = -radius; c <= radius; c++)
				{
					myGlcm[image.at<uchar>(row, col)][image.at<uchar>(row + r, col + c)]++;
				}
			}
			myGlcm[image.at<uchar>(row, col)][image.at<uchar>(row, col)]--;
			count += ((2 * radius + 1) * (2 * radius + 1)) - 1;
		}
		//std::cout << row << std::endl;
	}

	//	Remove 0 rows (boundary of cornea image)
	count -= myGlcm[0][0];
	myGlcm[0][0] = 0;
	for (int i = 1; i < OUTSIZE; i++)
	{
		count -= myGlcm[i][0];
		count -= myGlcm[0][i];
		myGlcm[i][0] = 0;
		myGlcm[0][i] = 0;
	}

	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
		myGlcm[i][j] /= count;

	return myGlcm;
}

double Contrast(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
    for (int j = 0; j < OUTSIZE; j++)
	{
		sum += glcm[i][j] * ((i - j)*(i - j));
	}
	return sum;
}

double Dissimilarity(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += glcm[i][j] * abs(i - j);
	}
	return sum;
}

double Homogeneity(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += (glcm[i][j]) / (1 + (i - j)*(i - j));
	}
	return sum;
}

double ASM(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += (glcm[i][j] * glcm[i][j]);
	}
	return sum;
}

double Maximum(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		if (glcm[i][j] > sum) sum = glcm[i][j];
	}
	return sum;
}

double Entropy(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	{
		for (int j = 0; j < OUTSIZE; j++){
			if (glcm[i][j] != 0){
				sum -= ((glcm[i][j]) * log(glcm[i][j]));
			}
		}
	}
	return sum;
}

double Sav_Step(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += glcm[i][j];
	}
	return sum;
}
double Sav_Varience(double** glcm)
{
	return Contrast(glcm);
}
double Sav_Entropy(double** glcm)
{
	return Entropy(glcm);
}
double Sav_Energy(double** glcm)
{
	return ASM(glcm);
}
double Sav_Homogeneity(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += glcm[i][j] / (1 + abs(i - j));
	}
	return sum;
}
double Sav_3Moment(double** glcm)
{
	double sum = 0;
	for (int i = 0; i < OUTSIZE; i++)
	for (int j = 0; j < OUTSIZE; j++)
	{
		sum += glcm[i][j] * ((i - j)*(i - j)*(i - j));
	}
	return sum;
}
