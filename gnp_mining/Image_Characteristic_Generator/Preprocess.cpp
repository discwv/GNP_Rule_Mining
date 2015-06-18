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

	Mat out(input.size(), input.type());
	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{
			if (input.at<uchar>(row, col) <= min)
				out.at<uchar>(row, col) = 0;
			else
				out.at<uchar>(row, col) = 255;
		}
	}

	//find center of mass
	int thresh = 100;
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(out, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	for (int i = 0; i < mc.size(); i++)
	{
		int x = mc[i].x;
		int y = mc[i].y;
		for (i = 0; i < 50; i++)
		{
			out.at<uchar>(y, x) = 0;
			x++;
			y++;
		}
	}




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