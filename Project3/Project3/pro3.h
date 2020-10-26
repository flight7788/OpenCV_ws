#ifndef PRO3_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ximgproc.hpp>
#include <iostream>
#include<fstream>
#include<string>
#include <io.h>
using namespace std;
using namespace cv;

class pro3 {
public:
	pro3(){}
	~pro3(){}
	void run();
	void catchLaneLines(Mat src, Mat& dst);
	void catchAllImgName(string path, vector<string>& dst);
	void myHough(Mat src, Mat& dst);
	void avgThreshold(Mat input, Mat output, int means_size, uint8_t threshold_val);
private:

};

#endif // !PRO3_H_
