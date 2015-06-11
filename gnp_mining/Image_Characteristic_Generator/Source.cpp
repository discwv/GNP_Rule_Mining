#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include"GLCM.h"
using namespace std;
using namespace cv;

Mat Color8to4(Mat image);
void GenerateStats();
void G(string name, ofstream out);

int main()
{
	// C++ (OpenCV 2.0)
	Mat im_gray = imread("image.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
	im_gray = Color8to4(im_gray);
	//Mat small;
	Size si = Size(500, 333);
	resize(im_gray, im_gray, si);
	//im_gray = Color8to4(small);
	//imwrite("postshrink.jpeg", small);

	ofstream myout;
	myout.open("sav.csv");

	double** myGlcm = MakeGLCM(im_gray, 3);
	double count = 0;
	for (int i = 0; i < OUTSIZE; i++)
	{
		for (int j = 0; j < OUTSIZE; j++)
		{
			cout << myGlcm[i][j] << " ";
			count += myGlcm[i][j];
			myout << myGlcm[i][j] << ",";
		}
		cout << endl;
		myout << endl;
	}
	cout << count << endl << endl;

	cout << Contrast(myGlcm) << endl;
	cout << Dissimilarity(myGlcm) << endl;
	cout << Homogeneity(myGlcm) << endl;
	cout << ASM(myGlcm) << endl;
	cout << Maximum(myGlcm) << endl;
	cout << Entropy(myGlcm) << endl;

	int what;
	cin >> what;
}

Mat Color8to4(Mat image)
{
	for (int row = 0; row < image.rows; row++)
	{
		for (int col = 0; col < image.cols; col++)
		{
			int val = image.at<uchar>(row, col);
			val /= 16;
			//val *= 16;
			image.at<uchar>(row, col) = val;
		}
	}
	return image;
}

void GenerateStats()
{
	int start = 0;
	int end = 5;
	string filename = "stats_" + to_string(start) + "_" + to_string(end) + ".csv";
	ofstream myout;
	myout.open(filename);
	myout << "image,step,varience,entropy,energy,homogeneity,3moment" << endl;
	for (int i = start; i <= end; i++)
	{
		string leftname = to_string(i) + "_left.jpeg";
		string rightname = to_string(i) + "_right.jpeg";
		G(leftname, &myout);
		G(rightname, &myout);
	}
	myout.close();
}

void G(string name, ofstream* out)
{
	Mat im_gray = imread(name, CV_LOAD_IMAGE_GRAYSCALE);
	im_gray = Color8to4(im_gray);
	Size si = Size(500, 333);
	resize(im_gray, im_gray, si);
	double** myGlcm = MakeGLCM(im_gray, 3);
	*out << name << "," << Sav_Step(myGlcm) << "," << Sav_Varience(myGlcm) << "," << Sav_Entropy(myGlcm) << "," << Sav_Energy(myGlcm) << "," << Sav_Homogeneity(myGlcm) << "," << Sav_3Moment(myGlcm) << endl;
}