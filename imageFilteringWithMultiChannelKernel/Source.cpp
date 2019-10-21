#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int mirror(int a, int b)
{
	if (b < 0)
	{
		return -b - 1;
	}
	if (b >= a)
	{
		return 2 * a - b - 1;
	}
	return b;
}

int circular(int a, int b)
{
	if (b<0)
		return b + a;
	if (b >= a)
		return b - a;
	return b;
}


void borderProc(Mat mathProc, Mat dist, float Kernel[][3])
{

	float sum;
	for (int a = 1; a < mathProc.rows - 1; a++) {
		for (int b = 1; b < mathProc.cols - 1; b++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					sum = sum + Kernel[j + 1][k + 1] * mathProc.at<uchar>(a - j, b - k);
				}
			}
			dist.at<uchar>(a, b) = sum;
		}
	}
}

void reflectProcessing(Mat mathProc, Mat dist, float Kernel[][3])
{
	float sum, b1, y1;
	for (int a = 0; a < mathProc.rows; a++) {
		for (int b = 0; b < mathProc.cols; b++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					b1 = mirror(mathProc.cols, b - j);
					y1 = mirror(mathProc.rows, a - k);
					sum = sum + Kernel[j + 1][k + 1] * mathProc.at<uchar>(y1, b1);
				}
			}
			dist.at<uchar>(a, b) = sum;
		}
	}
}

void circularProcessing(Mat mathProc, Mat dist, float Kernel[][3])
{
	float sum, x1, y1;
	for (int a = 0; a < mathProc.rows; a++) {
		for (int b = 0; b < mathProc.cols; b++) {
			sum = 0.0;
			for (int k = -1; k <= 1; k++) {
				for (int j = -1; j <= 1; j++) {
					x1 = circular(mathProc.cols, b - j);
					y1 = circular(mathProc.rows, a - k);
					sum = sum + Kernel[j + 1][k + 1] * mathProc.at<uchar>(y1, x1);
				}
			}
			dist.at<uchar>(a, b) = sum;
		}
	}
}

int main()
{

	Mat mathProc, dist;


	/// Load an image
	mathProc = imread("image_for_part_2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	if (!mathProc.data)
	{
		return -1;
	}


	float Kernel[3][3] = {
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 },
		{ 1 / 9.0, 1 / 9.0, 1 / 9.0 }
	};

	dist = mathProc.clone();
	for (int a = 0; a < mathProc.rows; a++)
		for (int b = 0; b < mathProc.cols; b++)
			dist.at<uchar>(a, b) = 0.0;

	circularProcessing(mathProc, dist, Kernel);



	namedWindow("final");
	imshow("final", dist);

	namedWindow("initial");
	imshow("initial", mathProc);

	waitKey();


	return 0;
}