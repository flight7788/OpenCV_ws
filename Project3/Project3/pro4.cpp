#include"pro4.h"

void pro4::perspective_transform(Mat src, Mat& dst) {
	Point2f pre[] = {
		Point2f(23,365),
		Point2f(639,365),
		Point2f(190,206),
		Point2f(467,211)
	};

	Point2f after[] = {
		Point2f(28,365),
		Point2f(600,365),
		Point2f(28,100),
		Point2f(600,100)
	};

	for (int i = 0; i < 4; i++) {
		src.at<Vec3b>(pre[i].y, pre[i].x)[0] = 255;
		src.at<Vec3b>(pre[i].y, pre[i].x)[1] = 0;
		src.at<Vec3b>(pre[i].y, pre[i].x)[2] = 0;
	}
	Mat M = getPerspectiveTransform(pre, after);
	warpPerspective(src, dst, M, Size(560, 370), INTER_LINEAR);
}

void pro4::catchLaneLines(Mat src, Mat& dst) {
	Mat perspective, gray, hsv, white, morpho, clo, thin;
	perspective_transform(src, perspective);
	cvtColor(perspective, gray, COLOR_BGR2GRAY);
	cvtColor(perspective, hsv, COLOR_RGB2HSV);
	inRange(hsv, Scalar(0, 0, 120), Scalar(120, 80, 255), white);
	morphologyEx(white, morpho, MORPH_OPEN, Mat(), Point(-1, -1), 2);
	morphologyEx(morpho, clo, MORPH_CLOSE, Mat(5, 5, CV_8UC1), Point(-1, -1), 4);
	//ximgproc::thinning(clo, thin, ximgproc::THINNING_ZHANGSUEN);
	dst = clo;
	return;
}

void pro4::run() {
	VideoCapture video("D:/OpenCV_ws/output.avi");
	if (!video.isOpened()) {
		cout << "Movie open Error" << endl;
		return;
	}

	int frame_count = video.get(CAP_PROP_FRAME_COUNT);
	Mat frame;
	Mat lane_line;

	for (int i = 0; i < frame_count; i++) {
		video.read(frame);
		catchLaneLines(frame, lane_line);

		imshow("f", frame);
		imshow("line", lane_line);
		if (waitKey(100) > 0)
			break;
	}

}