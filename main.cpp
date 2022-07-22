#include  "opencv2/opencv.hpp"
#include <iostream>
#include <Windows.h>
using namespace cv;
using namespace std;

void main()
{
	Mat src, dst1, dst2, dst3, src_copy, im_out2;
	vector<Point2f> corners, _arr, arr;
	int posx, posy;
	float iScreenCx = GetSystemMetrics(SM_CXSCREEN);								// ȭ�� ũ�� ���ϱ�
	float iScreenCy = GetSystemMetrics(SM_CYSCREEN);


	src = imread("C:/Users/KimHaYeon/source/repos/opencv2/image.png", IMREAD_GRAYSCALE);
	if (src.empty())
	{
		cerr << "Image Load Failed!" << endl;
		return;
	}


	if (src.cols > iScreenCx || src.rows > iScreenCy)
	{
		float ratio = float(src.cols) / float(src.rows);
		if (src.cols - iScreenCx > src.rows - iScreenCy)							// �ʺ� �� ũ�� �ʰ��� ���
			resize(src, src, Size(iScreenCx, src.rows - ((src.cols - iScreenCx) * 1/ratio)));
		else																		// ���̰� �� ũ�� �ʰ��� ���
			resize(src, src, Size(src.cols - ((src.rows - iScreenCy) * ratio), iScreenCy));
	}
	imshow("src", src);
	src_copy = src.clone();


	GaussianBlur(src, src, Size(), 1);												// ����þ� ���� ����
	imshow("GaussianBlur", src);


	adaptiveThreshold(src, dst1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 3);	// �̹����� ����ȭ
	dst1 = ~dst1;
	imshow("adaptiveThreshold", dst1);
	waitKey();


	medianBlur(dst1, dst1, 5);		
	imshow("medianBlur", dst1);
	morphologyEx(dst1, dst1, MORPH_CLOSE, Mat());
	imshow("morphologyEx", dst1);
	waitKey();

	
	Mat im_floodfill = dst1.clone();												// flood-fillng(�ݱ� ����) ����
	Mat im_floodfill_inv;
	floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));
	im_floodfill_inv = ~im_floodfill;
	imshow("im_floodfill1", im_floodfill);
	waitKey();


	floodFill(im_floodfill_inv, cv::Point(0, 0), Scalar(255));						// flood-fillng���� inv��  ���� ������ ����
	imshow("im_floodfill2", im_floodfill_inv);
	im_floodfill_inv = ~im_floodfill_inv;
	im_floodfill -= im_floodfill_inv;
	imshow("im_floodfill3", im_floodfill);
	waitKey();


	goodFeaturesToTrack(im_floodfill, corners, 4, 0.05, 90, noArray(), 9);			// goodFeaturesToTrack�� ���� ������ �ڳ� 4�� ����
	for (int i = 0; i < corners.size(); i++) 
		circle(im_floodfill, corners[i], 5, Scalar(100, 0, 0));
	imshow("Corner", im_floodfill);
	waitKey();
	 

	for (int c = 0; c < 2; c++)														// �ڳ� ��ǥ ����
	{
		int num, max_index = -1, min_index = -1;
		float min = 99999.0f, max = -99999.0f;
		for (int a = 0; a < corners.size(); a++)
		{
			if (c == 0) num = sqrt(pow(corners[a].x, 2) + pow(corners[a].y,2));
			else num = sqrt(pow((src.cols - corners[a].x),2) + pow(corners[a].y,2));
			if (max <= num)
			{
				max = num;
				max_index = a;
			}
			if (min >= num)
			{
				min = num;
				min_index = a;
			}
			if (max == min)	cerr << "error!" << endl;
		}
		arr.push_back(corners[min_index]);
		arr.push_back(corners[max_index]);
	}
	_arr.push_back(Point2f(0, 0));
	_arr.push_back(Point2f(src.cols - 1, src.rows - 1));
	_arr.push_back(Point2f(src.cols - 1, 0));
	_arr.push_back(Point2f(0, src.rows - 1));
	

	Mat pers = getPerspectiveTransform(arr, _arr);									// getPerspectiveTransform()�� 3x3 ���� ��ȯ ��� ��ȯ
	warpPerspective(src_copy, src_copy, pers, Size(src.cols, src.rows));			// warpPerspective()�� ���� ��ȯ�� ��� ���� ����
	imshow("dst", src_copy);
	waitKey();


	return;
}