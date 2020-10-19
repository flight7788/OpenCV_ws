#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


void AvgThreshold(Mat input, Mat output, int means_size, uint8_t threshold_val) {
	
	for (int y = 0; y < input.rows; y += means_size) {
		for (int x = 0; x < input.cols; x += means_size) {
			double AvgColor = 0.0;
			int avg_cnt = 0;
			int max_y = ((y + means_size) >= input.rows) ? (input.rows) : (y + means_size);
			int max_x = ((x + means_size) >= input.cols) ? (input.cols) : (x + means_size);

			for (int means_y = y; means_y < max_y; means_y++) {
				for (int means_x = x; means_x < max_x; means_x++) {
					AvgColor += input.at<uchar>(means_y, means_x);
					avg_cnt++;
				}
			}
			AvgColor = AvgColor / avg_cnt;

			for (int means_y = y; means_y < max_y; means_y++) {
				for (int means_x = x; means_x < max_x; means_x++) {
					if (AvgColor >= threshold_val && (means_y>=100)) {
						output.at<uchar>(means_y, means_x) = 255;
					}
					else {
						output.at<uchar>(means_y, means_x) = 0;
					}
				}
			}
		}
	}
}

int main(void) {
	VideoCapture stream1("../../road_following_dataset_xy_blackground/dataset_xy/test.mp4");
	Mat frame, GrayImage, FilterImage, Final;
	Mat ThresholdImage(224, 224, CV_8UC1, Scalar(0));
	namedWindow("OriginImage", 0);
	namedWindow("GrayImage", 0);
	namedWindow("ThresholdImage", 0);
	namedWindow("FilterImage", 0);
	namedWindow("final", 0);
	
	while (true) {
		if (!(stream1.read(frame)))   
			break;
		//resize(frame, frame, Size(224, 224));
		cvtColor(frame, GrayImage, COLOR_BGR2GRAY);
		//threshold(GrayImage, ThresholdImage, 175, 255, THRESH_BINARY | THRESH_OTSU);
		//adaptiveThreshold(GrayImage, ThresholdImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 25, 10);
		
		Mat mask,result;
		inRange(GrayImage, Scalar(165), Scalar(255), mask);
		bitwise_and(GrayImage, mask, result);
		AvgThreshold(result, ThresholdImage, 3, 100);
		
		bilateralFilter(ThresholdImage, FilterImage, 10, 150, 20);
		morphologyEx(FilterImage, Final, MORPH_OPEN, Mat(), Point(-1, -1), 2);

		imshow("OriginImage", frame);
		imshow("GrayImage", GrayImage);
		imshow("ThresholdImage", ThresholdImage);
		imshow("FilterImage", FilterImage);
		imshow("Final", Final);

		if (waitKey(30) > 0)
			break;
	}
    waitKey(0);
    return 0;
}






/*
	string path = "../../road_following_dataset_xy_blackground/dataset_xy/xy_048_068_a584f9a4-0180-11eb-b9f5-16f63a1aa8c9.jpg";
	Mat image = imread(path, IMREAD_COLOR);
	if (!image.data) {
		cout << "Image not found or unable to open" << endl;
		return -1;
	}
	//imshow("picture1", image);

	Mat GrayImage;
	cvtColor(image, GrayImage, COLOR_BGR2GRAY);

	Mat ThresholdImage;
	threshold(GrayImage, ThresholdImage, 200, 255, THRESH_BINARY);// | THRESH_OTSU);
	//imshow("picture2", ThresholdImage);

	Mat FilterImage;
	bilateralFilter(ThresholdImage, FilterImage, 10, 100, 20);
	//imshow("picture3", FilterImage);

	Mat result;
	morphologyEx(FilterImage, result, MORPH_OPEN, Mat(), Point(1, 1), 2);
	imshow("picture4", result);
	*/
