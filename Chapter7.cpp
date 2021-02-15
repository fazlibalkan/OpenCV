#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// <summary>
/// Shapes / Contour Detection
/// </summary>

Mat imgGray, imgBlur, imgCanny, imgDilate;


void getContours(Mat imgD, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgD, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 255, 0), 2);
	
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	string objectType;

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 1000) {

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			drawContours(img, conPoly, i, Scalar(255, 255, 0), 2);
			cout << conPoly[i].size() << endl;
			
			boundRect[i] = boundingRect(conPoly[i]);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			
			int objectCorner = (int) conPoly[i].size();
			
			if (objectCorner == 3) {
				objectType = "Triangle";
			}
			if (objectCorner == 4) { 
				float aspRatio = (float)boundRect[i].width / boundRect[i].height;
				if (aspRatio > 0.95 && aspRatio < 1.05) 
					objectType = "Square";
				else
					objectType = "Rectangle"; 
			}
			if (objectCorner > 4) { 
				objectType = "Circle"; 
			}

			putText(img, objectType, { boundRect[i].x, boundRect[i].y}, FONT_HERSHEY_DUPLEX, 0.7, Scalar(200,12,234));
		}

	}

}


void main() {
	string path = "Resources/shapes.png";
	Mat img = imread(path);

	//Preprocessing
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3,3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDilate, kernel);

	getContours(imgDilate, img);

	imshow("Image", img);

	waitKey(0);
}