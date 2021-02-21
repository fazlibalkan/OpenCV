#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float width = 420, height = 596;

Mat preProcessing(Mat img) {
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);

	return imgDil;
}

vector<Point> getContours(Mat imgD) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgD, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	string objectType;

	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++) {

		int area = contourArea(contours[i]);

		if (area > 1000) {

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {
				biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
				maxArea = area;
				//drawContours(imgOriginal, conPoly, i, Scalar(255, 255, 0), 2);
			}

			
		}
	}

	return biggest;
}

void drawPoints(vector<Point> points, Scalar color) {
	cout << points.size();
	for (int i = 0; i < points.size(); i++) {
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_COMPLEX, 2, color, 2);
	}
}

vector<Point> reorder(vector<Point> points) {
	vector<Point> newPoints;
	vector<int> sumP, subP;

	for (int i = 0; i < points.size(); i++) {
		sumP.push_back(points[i].x + points[i].y);
		subP.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumP.begin(), sumP.end()) - sumP.begin()]);
	newPoints.push_back(points[max_element(subP.begin(), subP.end()) - subP.begin()]);
	newPoints.push_back(points[min_element(subP.begin(), subP.end()) - subP.begin()]);
	newPoints.push_back(points[max_element(sumP.begin(), sumP.end()) - sumP.begin()]);

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h) {
	Point2f src[4] = {points[0], points[1], points[2], points[3]};
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

void main() {
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	//resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	// Preprocessing
	imgThre = preProcessing(imgOriginal);

	// Get Contours
	initialPoints = getContours(imgThre);
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 255, 0));

	// Warp
	imgWarp = getWarp(imgOriginal, docPoints, width, height);

	//Crop
	int cropValue = 10;
	Rect roi(cropValue, cropValue, width - 2 * cropValue, height - 2 * cropValue);
	imgCrop = imgWarp(roi);


	imshow("Image", imgOriginal);
	//imshow("Image Dil", imgThre);
	//imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);
	waitKey(0);

}