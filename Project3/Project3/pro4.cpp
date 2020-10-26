#include"pro4.h"




void pro4::catchLaneLines(Mat src, Mat& dst) {
	Mat gray, hsv, white, morpho, clo;
	Mat mask, rgb, hist, binary;
	Mat copy = src.clone();

	for (int i = 0; i < copy.size().width; i++) {
		for (int j = 0; j < copy.size().height/2; j++) {
			copy.at<Vec3b>(j, i)[0] = 0;
			copy.at<Vec3b>(j, i)[1] = 0;
			copy.at<Vec3b>(j, i)[2] = 0;
		}
	}
	cvtColor(copy, hsv, COLOR_RGB2HSV);
	
	inRange(hsv, Scalar(0, 0, 200), Scalar(200, 255, 255), white);
	bitwise_and(hsv, hsv, mask, white);
	cvtColor(mask, rgb, COLOR_HSV2RGB);
	cvtColor(rgb, gray, COLOR_RGB2GRAY);


	equalizeHist(gray, hist);
	threshold(hist, binary, 20, 255, THRESH_BINARY);
	
	morphologyEx(binary, morpho, MORPH_OPEN, Mat(), Point(-1, -1), 1);
	morphologyEx(morpho, clo, MORPH_CLOSE, Mat(5, 5, CV_8UC1), Point(-1, -1), 5);

	dst = clo;
	return;
}

void pro4::find_corner(Mat src, vector<Point>& dst) {
	bool flag = false;
	int p0_x, p0_y;
	for (int y = 0; y < src.rows; y++) {
		for (int x = 0; x < src.cols / 2; x++) {
			if (src.at<uchar>(y, x) >= 255) {
				p0_x = x;
				p0_y = y;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	flag = false;
	int p1_x, p1_y;
	for (int y = 0; y < src.rows; y++) {
		for (int x = src.cols - 1; x > src.cols / 2; x--) {
			if (src.at<uchar>(y, x) >= 255) {
				p1_x = x;
				p1_y = y;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	flag = false;
	int p2_x, p2_y;
	for (int y = src.rows - 1; y > 0; y--) {
		for (int x = 0; x < src.cols / 2; x++) {
			if (src.at<uchar>(y, x) >= 255) {
				p2_x = x;
				p2_y = y;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	flag = false;
	int p3_x, p3_y;
	for (int y = src.rows - 1; y > 0; y--) {
		for (int x = src.cols - 1; x > src.cols / 2; x--) {
			if (src.at<uchar>(y, x) >= 255) {
				p3_x = x;
				p3_y = y;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}

	
	dst.push_back(Point(p0_x, p0_y));
	dst.push_back(Point(p1_x, p1_y));
	dst.push_back(Point(p3_x, p3_y));
	dst.push_back(Point(p2_x, p2_y));
}

void pro4::run() {
	VideoCapture video("D:/OpenCV_ws/output.avi");
	if (!video.isOpened()) {
		cout << "Movie open Error" << endl;
		return;
	}

	int frame_count = video.get(CAP_PROP_FRAME_COUNT);
	Mat frame = Mat::zeros(240, 320, CV_8UC3);
	Mat lane_line = Mat::zeros(240, 320, CV_8UC3);
	Mat rect;
	Mat dst = Mat::zeros(240, 320, CV_8UC3);
	namedWindow("frame");
	moveWindow("frame", 250, 300);

	namedWindow("lane_line");
	moveWindow("lane_line", 600, 300);

	namedWindow("dst");
	moveWindow("dst", 950, 300);

	for (int i = 0; i < frame_count; i++) {
		video.read(frame);
		catchLaneLines(frame, lane_line);
		vector<Point> corner;
		find_corner(lane_line, corner);
		frame.copyTo(rect);
		
		fillPoly(rect, corner, Scalar(0, 255, 0));
		
		addWeighted(frame, 0.7, rect, 0.3, 0, dst);
		line(dst, Point(corner[0].x, corner[0].y), Point(corner[3].x, corner[3].y), Scalar(255, 0, 0), 2);
		line(dst, Point(corner[1].x, corner[1].y), Point(corner[2].x, corner[2].y), Scalar(0, 0, 255), 2);
		for (int i = 0; i < corner.size(); i++) {
			circle(dst, Point(corner[i].x, corner[i].y), 5, Scalar(255, 0, 0), FILLED);
		}
		double x = frame.size().width, y = frame.size().height;
		resize(frame, frame, Size(x / 2, y / 2));
		resize(lane_line, lane_line, Size(x / 2, y / 2));
		resize(dst, dst, Size(x / 2, y / 2));
		imshow("frame", frame);
		imshow("lane_line", lane_line);
		imshow("dst", dst);
		if (waitKey(1) > 0)
			break;
	}

}