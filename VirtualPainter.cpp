#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img;
vector<vector<int>> newPoints;

vector<vector<int>> myColors{ {67, 111, 177, 112, 231, 255}, // Blue
                              {117, 17, 158, 138, 50, 255}}; // Purple

vector<Scalar> colors{ {255, 0, 0},  // Blue
						{255, 0, 255}};  // Purple


Point getContours(Mat imgD) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgD, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	string objectType;
	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++) {

		int area = contourArea(contours[i]);
		//cout << area << endl;

		if (area > 500) {

			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			//drawContours(img, conPoly, i, Scalar(255, 255, 0), 2);
			
			boundRect[i] = boundingRect(conPoly[i]);

			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			cout << myPoint << endl;
			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);

		}
	}

	return myPoint;
}


void findColor(Mat image)
{
    Mat imgHSV;
    cvtColor(image, imgHSV, COLOR_BGR2HSV);

    for (int i = 0; i < myColors.size(); i++) {
        Mat mask;
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
        inRange(imgHSV, lower, upper, mask);

		Point myPoint = getContours(mask);

		if (myPoint.x != 0 || myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y, i});
		}

    }
}

void draw() {
	for (int i = 0; i < newPoints.size(); i++) {
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, colors[newPoints[i][2]], FILLED);
	}

}

int main() {

    VideoCapture cap(0);
    while (true) {
        cap.read(img);
		findColor(img);
		draw();

        imshow("Image", img);
		waitKey(1);
    }

    waitKey(0);
    return 0;
}


