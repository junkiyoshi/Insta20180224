#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(1280, 720);
	ofSetFrameRate(this->cap.get(CV_CAP_PROP_FPS));

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame, src, blend_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		return;
	}
	cv::resize(cap_frame, src, this->cap_size);
	cv::flip(src, src, 1);
	cv::cvtColor(src, src, CV_RGB2BGR);

	for (int x = 0; x < src.cols; x++) {

		for (int y = 0; y < src.rows; y++) {

			ofVec2f center = ofVec2f(src.cols / 2, src.rows / 2);
			ofVec2f point = ofVec2f(x, y);
			float distance = center.distance(point);

			if (distance < 300) {

				int angle = distance / 20;

				point = point - center;
				point = point.getRotated(angle * ofGetFrameNum());
				point = point + center;
			}
			
			float tmp_x = (int)point.x;
			float tmp_y = (int)point.y;
			
			if (tmp_x >= 0 && tmp_x < src.cols && tmp_y >= 0 && tmp_y < src.rows) {

				this->frame.at<cv::Vec3b>(tmp_y, tmp_x) = src.at<cv::Vec3b>(y, x);
			}
		}
	}

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}