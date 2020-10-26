#include"pro3.h"

void pro3::myHough(Mat src, Mat& dst) {
	vector<Vec4f> lines;
	HoughLinesP(src, lines, 1, CV_PI / 180, 1, 1.0, 1.0);
	for (size_t i = 0; i < lines.size(); i++) {
		double x1 = lines[i][0], y1 = lines[i][1];
		double x2 = lines[i][2], y2 = lines[i][3];
		Point pt1(x1, y1);
		Point pt2(x2, y2);
		line(dst, pt1, pt2, Scalar(255, 0, 0), 1, LineTypes::LINE_AA);
	}
}

void pro3::avgThreshold(Mat input, Mat output, int means_size, uint8_t threshold_val) {

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
					if (AvgColor >= threshold_val && (means_y >= 100)) {
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

void pro3::catchAllImgName(string path, vector<string>& dst) {
	intptr_t hFile = 0;
	_finddata64i32_t fileinfo;
	hFile = _findfirst64i32((path + "*.jpg").c_str(), &fileinfo);
	if (hFile != -1) {
		do {

			if ((fileinfo.attrib & _A_SUBDIR)) {
				continue;
			}
			else {

				dst.push_back(fileinfo.name);
			}
		} while (!_findnext64i32(hFile, &fileinfo));
		_findclose(hFile);
	}
}

void pro3::catchLaneLines(Mat src, Mat& dst) {
	Mat adjust_color = src.clone();
	Mat gray, hist, binary, mask, morpho;

	cvtColor(adjust_color, gray, COLOR_BGR2GRAY);
	//imwrite("D:/OpenCV_ws/process/gray.jpg", gray);

	equalizeHist(gray, hist);
	//imwrite("D:/OpenCV_ws/process/hist.jpg", hist);

	threshold(hist, binary, 220, 255, THRESH_BINARY);
	//imwrite("D:/OpenCV_ws/process/binary.jpg", binary);

	mask = binary.clone();
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols / 3; j++) {
			mask.at<char>(Point(i, j)) = 0;
		}
	}
	//imwrite("D:/OpenCV_ws/process/mask.jpg", mask);

	morphologyEx(mask, morpho, MORPH_OPEN, Mat(), Point(-1, -1), 2);
	//imwrite("D:/OpenCV_ws/process/morpho.jpg", morpho);

	dst = morpho;
	return;
}

void pro3::run() {
	bool sw = false;
	string path[] = { "D:/OpenCV_ws/test_img/road_following_dataset_xy_blackground/dataset_xy/",\
					  "D:/OpenCV_ws/test_img/road_following_dataset_xy_puzzle/dataset_xy/",\
					  "D:/OpenCV_ws/test_img/road_following_dataset_xy_twolane/dataset_xy/" };
	string selectPath[] = { "D:/OpenCV_ws/others/road_following_dataset_xy_blackground/",\
							"D:/OpenCV_ws/others/road_following_dataset_xy_puzzle/",\
							"D:/OpenCV_ws/others/road_following_dataset_xy_twolane/", };

	for (int i = 0; i < 3; i++) {
		vector<string> imgList;
		catchAllImgName(path[i], imgList);
		int count = 3;
		for (string img_name : imgList) {
			if (!count) {
				break;
			}
			Mat src, dst;
			src = imread(path[i] + img_name);
			catchLaneLines(src, dst);

			imshow("original", src);
			imshow(img_name, dst);
			if (waitKey(300) > 0)
				break;
			//destroyWindow(img_name);
			if (sw) {
				char select;
				cout << "select:";
				cin >> select;
				if (select == '1') {
					count--;
					img_name = img_name.substr(0, img_name.find('.'));
					imwrite(selectPath[i] + img_name + "_original.jpg", src);
					imwrite(selectPath[i] + img_name + "_catch.jpg", dst);
				}
			}


		}
	}
}