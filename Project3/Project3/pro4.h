#ifndef PRO4_H_

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



class pro4 {
public:
	pro4(){}
	~pro4(){}
	void run();
	void catchLaneLines(Mat src, Mat& dst);
	void perspective_transform(Mat src, Mat& dst);
private:

};

#endif // !PRO4_H_

