#include "Preprocess.h"

using namespace std;
using namespace cv;

Mat gnp_m::ExtractChannel(Mat input, int channel)
{
	Mat newmat(input.rows, input.cols, CV_8UC1);
	Mat out[] = { newmat };
	int from_to[] = { channel, 0 };
	mixChannels(&input, 1, out, 1, from_to, 1);
	return out[0];
}

void gnp_m::ErodeMap(Mat input, int radius)
{
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * radius + 1, 2 * radius + 1),
		Point(radius, radius));
	erode(input, input, element);
}

Mat gnp_m::MakeGradientImage(Mat input)
{
	Mat out(input.size(), input.type());
	for (int i = 0; i < input.cols; i++)
	{
		out.at<uchar>(0, i) = 0;
		out.at<uchar>(out.rows - 1, i) = 0;
	}
	for (int i = 0; i < input.rows; i++)
	{
		out.at<uchar>(i, 0) = 0;
		out.at<uchar>(i, out.cols -1) = 0;
	}
	for (int row = 1; row < out.rows - 1; row++)
	{
		for (int col = 1; col < out.cols - 1; col++)
		{
			out.at<uchar>(row, col) = //odd almost sobel filter, its what they said in the paper im working from so whatevs
				abs(
				input.at<uchar>(row + 1, col - 1) + input.at<uchar>(row + 1, col) + input.at<uchar>(row + 1, col + 1)
				- input.at<uchar>(row - 1, col - 1) - input.at<uchar>(row - 1, col) - input.at<uchar>(row - 1, col + 1)
				) + abs(
				input.at<uchar>(row - 1, col - 1) + input.at<uchar>(row, col - 1) + input.at<uchar>(row + 1, col - 1)
				- input.at<uchar>(row - 1, col + 1) - input.at<uchar>(row, col + 1) - input.at<uchar>(row + 1, col + 1)
				);
		}
	}
	return out;
}

Mat gnp_m::FindHueMask(Mat input, Mat mask)
{
	Mat huemat(input.size(), input.type());
	cvtColor(input, huemat, CV_RGB2HSV);
	Mat justhue = gnp_m::ExtractChannel(huemat, 0);
	Mat meanarr, stddevarr;
	meanStdDev(justhue, meanarr, stddevarr, mask);
	double thresh = meanarr.at<double>(0) + stddevarr.at<double>(0);
	Mat out(justhue.size(), justhue.type());
	for (int row = 0; row < justhue.rows; row++)
	{
		for (int col = 0; col < justhue.cols; col++)
		{
			if (justhue.at<uchar>(row, col) >= thresh)
			{
				out.at<uchar>(row, col) = 0;
			}
			else
			{
				out.at<uchar>(row, col) = 255;
			}
		}
	}
	return out;
}

Mat gnp_m::FindFOVMask(Mat input)
{
	int hist[256] = { };
	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{
			hist[input.at<uchar>(row,col)]++;
		}
	}

	int min = 0;
	for (int i = 10; i < 51; i++)
	{
		if (hist[min] > hist[i]) min = i;
	}
	
	double area = 0;
	Mat out(input.size(), input.type());
	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{
			if (input.at<uchar>(row, col) <= min)
			{
				out.at<uchar>(row, col) = 0;
			}
			else
			{
				area += 1;
				out.at<uchar>(row, col) = 255;
			}
		}
	}
	//             FOR LATER USE
	//double diam = 2 * sqrt(area / 3.141592);

	////find center of mass
	//int thresh = 100;
	//Mat canny_output;
	//vector<vector<Point> > contours;
	//vector<Vec4i> hierarchy;

	///// Detect edges using canny
	//Canny(out, canny_output, thresh, thresh * 2, 3);
	///// Find contours
	//findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	///// Get the moments
	//vector<Moments> mu(contours.size());
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	mu[i] = moments(contours[i], false);
	//}

	/////  Get the mass centers:
	//vector<Point2f> mc(contours.size());
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	//}

	//for (int i = 0; i < 1; i++)
	//{
	//	int x = mc[i].x;
	//	int y = mc[i].y;
	//	for (int k = 0; k < 150; k++)
	//	{
	//		out.at<uchar>(y, mc[i].x) = 0;
	//		out.at<uchar>(mc[i].y, x) = 0;
	//		x++;
	//		y++;
	//	}
	//}

	return out;
	/*int lefty = 0, righty = 0;
	int countleftmax = 0, countrightmax = 0;
	int leftmax = input.cols - 1, rightmax = 0;
	bool leftfound = false, rightfound = false;
	//for each row
	for (int row = 0; row < input.rows; row++)
	{
		//iterate from left and right
		for (int in = 0; in < input.cols / 2; in++)
		{
			//left
			if (!leftfound && input.at<uchar>(row, in) >= 10)
			{
				int val = input.at<uchar>(row, in);
				leftfound = true;
				if (in < leftmax)
				{
					leftmax = in;
					lefty = row;
				}
			}

			//right
			if (!rightfound && input.at<uchar>(row, input.cols - in - 1) >= 10)
			{
				rightfound = true;
				if (input.cols - in - 1 > rightmax)
				{
					rightmax = input.cols - in - 1;
					righty = row;
				}
			}

			if (rightfound && leftfound)
			{
				break;
			}
		}
		leftfound = rightfound = false;
	}
	double diam = rightmax - leftmax;
	double middlex = leftmax + (diam / 2);
	diam -= (input.cols / 40);// arbitrary number selected to help ensure mask covers entire black space
	double middley = (lefty + righty) / 2;
	middley += 10;// another arbitrary number
	Mat out(input.size(), input.type());
	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{
			if (sqrt(pow(abs(((double)row) - middley), 2) + pow(abs(((double)col) - middlex), 2)) < (diam / 2))
			{
				out.at<uchar>(row, col) = 50;
			}
			else
			{
				out.at<uchar>(row, col) = 0;
			}
		}
	}
	return out;*/
}

//make histogram
//find min between 10,50
//thats tmask