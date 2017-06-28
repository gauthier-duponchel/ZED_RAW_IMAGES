//# include <stdio.h>

# include <conio.h>  
# include <sl/Camera.hpp>;
# include <opencv2/core/core.hpp>
# include <opencv2/highgui/highgui.hpp>
# include <opencv2/imgcodecs.hpp>
# include <opencv2/imgproc/imgproc.hpp>
 



//-----------------------------------------------------------------------------------------------//

//# define TEST_SL_CAMERA_CONTROL

//# define TEST_OPENCV

# define RECORD_RAW_IMAGES


//-----------------------------------------------------------------------------------------------//

#ifdef TEST_SL_CAMERA_CONTROL
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////


/********************************************************************************
** This sample demonstrates how to grab images and change the camera settings **
** with the ZED SDK                                                           **
********************************************************************************/


//// Standard includes
#include <stdio.h>
#include <string.h>

//// ZED include
#include <sl/Camera.hpp>


//// Using std and sl namespaces
using namespace std;
using namespace sl;


//// Sample functions
void updateCameraSettings(char key, sl::Camera &zed);
void switchCameraSettings();
void printHelp();
void printCameraInformation(sl::Camera &zed);

//// Sample variables (used everywhere)
CAMERA_SETTINGS camera_settings_ = CAMERA_SETTINGS_BRIGHTNESS; // create a camera settings handle
string str_camera_settings = "BRIGHTNESS";
int step_camera_setting = 1;

int main(int argc, char **argv) {

	///////// Create a ZED camera //////////////////////////
	Camera zed;

	///////// Initialize and open the camera ///////////////
	ERROR_CODE err; // error state for all ZED SDK functions

					// Open the camera
	err = zed.open();

	if (err != SUCCESS) {
		std::cout << errorCode2str(err) << std::endl;
		zed.close();
		return EXIT_FAILURE; // quit if an error occurred
	}

	// Print help in console
	printHelp();

	// Print camera information
	printCameraInformation(zed);

	// Create a Mat to store images
	Mat zed_image;

	// Loop until 'q' is pressed
	char key = ' ';
	while (key != 'q') {

		// Grab images and process them
		err = zed.grab();

		// Check that grab() is successful
		if (err == SUCCESS) {
			// Retrieve left image and display it with OpenCV
			zed.retrieveImage(zed_image, VIEW_LEFT);
			cv::imshow("VIEW", cv::Mat(zed_image.getHeight(), zed_image.getWidth(), CV_8UC4, zed_image.getPtr<sl::uchar1>(sl::MEM_CPU)));
			key = cv::waitKey(5);

			// Handle keyboard shortcuts
			updateCameraSettings(key, zed);
		}
		else
			key = cv::waitKey(5);
	}

	// Exit
	zed.close();
	return EXIT_SUCCESS;
}

/**
*  This function updates the ZED camera settings
**/
void updateCameraSettings(char key, sl::Camera &zed) {
	int current_value;

	// Keyboard shortcuts
	switch (key) {

		// Switch to next camera parameter
	case 's':
		switchCameraSettings();
		break;

		// Increase camera settings value ('+' key)
	case '+':
		current_value = zed.getCameraSettings(camera_settings_);
		zed.setCameraSettings(camera_settings_, current_value + step_camera_setting);
		std::cout << str_camera_settings << ": " << current_value + step_camera_setting << std::endl;
		break;

		// Decrease camera settings value ('-' key)
	case '-':
		current_value = zed.getCameraSettings(camera_settings_);
		if (current_value >= 1) {
			zed.setCameraSettings(camera_settings_, current_value - step_camera_setting);
			std::cout << str_camera_settings << ": " << current_value - step_camera_setting << std::endl;
		}
		break;

		// Reset default parameters
	case 'r':
		std::cout << "Reset all settings to default" << std::endl;
		zed.setCameraSettings(sl::CAMERA_SETTINGS_BRIGHTNESS, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_CONTRAST, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_HUE, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_SATURATION, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_GAIN, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE, -1, true);
		zed.setCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE, -1, true);
		break;
	}
}

/**
*  This function switches between camera settings
**/
void switchCameraSettings() {
	step_camera_setting = 1;
	switch (camera_settings_) {
	case CAMERA_SETTINGS_BRIGHTNESS:
		camera_settings_ = CAMERA_SETTINGS_CONTRAST;
		str_camera_settings = "Contrast";
		std::cout << "Camera Settings: CONTRAST" << std::endl;
		break;

	case CAMERA_SETTINGS_CONTRAST:
		camera_settings_ = CAMERA_SETTINGS_HUE;
		str_camera_settings = "Hue";
		std::cout << "Camera Settings: HUE" << std::endl;
		break;

	case CAMERA_SETTINGS_HUE:
		camera_settings_ = CAMERA_SETTINGS_SATURATION;
		str_camera_settings = "Saturation";
		std::cout << "Camera Settings: SATURATION" << std::endl;
		break;
	case CAMERA_SETTINGS_SATURATION:
		camera_settings_ = CAMERA_SETTINGS_GAIN;
		str_camera_settings = "Gain";
		std::cout << "Camera Settings: GAIN" << std::endl;
		break;

	case CAMERA_SETTINGS_GAIN:
		camera_settings_ = CAMERA_SETTINGS_EXPOSURE;
		str_camera_settings = "Exposure";
		std::cout << "Camera Settings: EXPOSURE" << std::endl;
		break;
	case CAMERA_SETTINGS_EXPOSURE:
		camera_settings_ = CAMERA_SETTINGS_WHITEBALANCE;
		str_camera_settings = "White Balance";
		step_camera_setting = 100;
		std::cout << "Camera Settings: WHITE BALANCE" << std::endl;
		break;

	case CAMERA_SETTINGS_WHITEBALANCE:
		camera_settings_ = CAMERA_SETTINGS_BRIGHTNESS;
		str_camera_settings = "Brightness";
		std::cout << "Camera Settings: BRIGHTNESS" << std::endl;
		break;
	}
}

/**
*  This function displays ZED camera information
**/
void printCameraInformation(sl::Camera &zed) {
	printf("ZED Serial Number         : %d\n", zed.getCameraInformation().serial_number);
	printf("ZED Firmware              : %d\n", zed.getCameraInformation().firmware_version);
	printf("ZED Camera Resolution     : %dx%d\n", zed.getResolution().width, zed.getResolution().height);
	printf("ZED Camera FPS            : %d\n", (int)zed.getCameraFPS());
}

/**
*  This function displays help
**/
void printHelp() {
	cout << endl;
	cout << endl;
	cout << "Camera controls hotkeys: " << endl;
	cout << "  Increase camera settings value:            '+'" << endl;
	cout << "  Decrease camera settings value:            '-'" << endl;
	cout << "  Switch camera settings:                    's'" << endl;
	cout << "  Reset all parameters:                      'r'" << endl;
	cout << endl;
	cout << "Exit : 'q'" << endl;
	cout << endl;
	cout << endl;
	cout << endl;
}

#endif 

//-----------------------------------------------------------------------------------------------//

#ifdef TEST_OPENCV

int main(int argc, char* argv[])
{
	// Will store the code of the keyboard key pressed to terminate the program.
	int c = 0;

	// Load an image in bitmap format (modify the path to the image if needed...).
	IplImage* image = cvLoadImage("Lenna.png");

	// Creation of a window entitled "Window".
	cvNamedWindow("Window");

	// Display the image in the window.
	cvShowImage("Window", image);

	// Wait for the user to press a key, and return the key code.
	// This function is also necessary to refresh the window content.
	c = cvWaitKey(0);

	// Destroy the window previously created.
	cvDestroyWindow("Window");

	// Free the memory used by the image.
	cvReleaseImage(&image);

	//------------------------------------------------------------------------------------------//

	cv::Mat imgdata = cv::imread("Lenna.png");			// Load image.
	cv::namedWindow("Lenna", 1);						// Create a window for display.
	cv::imshow("Lenna", imgdata);						// Display the image in the window.
	c = cvWaitKey(0);
	//imgdata.release();									// Free the memory used by the image.
	//std::cout << "imgdata = " << imgdata << std::endl;	// Control Free the memory used by the image.

	cv::Mat bgr[3];
	cv::split(imgdata, bgr);
	cv::namedWindow("blue", 1);						// Create a window for display.
	cv::imshow("blue", bgr[0]);

	c = cvWaitKey(0);

	std::cout << "Fini !!" << std::endl;

	getchar();   //  = system("PAUSE"); !!

	//------------------------------------------------------------------------------------------//
}

#endif

//-----------------------------------------------------------------------------------------------//

#ifdef RECORD_RAW_IMAGES

enum resolution { HD2K, r1080p, r720p, WVGA };

std::tuple<int, int> set_Resolution()
{
	int r;

	std::cout << "\n-----------------------------------------------------------------" << std::endl;
	std::cout << "\t\tWelcome in : ZED RAW Images Recording" << std::endl;
	std::cout << "-----------------------------------------------------------------" << std::endl;
	std::cout << "\nSelect Resolution:\n" << std::endl;

	std::cout << "\t1  FOR  HD2K  = 4416 x 1242" << std::endl;
	std::cout << "\t2  FOR  1080p = 3840 x 1080" << std::endl;
	std::cout << "\t3  FOR  HD2K  = 2560 x  720" << std::endl;
	std::cout << "\t4  FOR  HD2K  = 1344 x  376" << std::endl;
	std::cout << "\nEnter the selected value: ";
	std::cin  >> r;

	resolution selected_resolution;

	switch (r)
	{
	case 1:
		selected_resolution = HD2K;
		break;
	case 2:
		selected_resolution = r1080p;
		break;
	case 3:
		selected_resolution = r720p;
		break;
	case 4:
		selected_resolution = WVGA;
		break;
	}

	int width, height;

	switch (selected_resolution)
	{
	case HD2K:
		width = 4416;
		height = 1242;
		break;
	case r1080p:
		width = 3840;
		height = 1080;
		break;
	case r720p:
		width = 2560;
		height = 720;
		break;
	case WVGA:
		width = 1344;
		height = 376;
		break;
	default:
		std::cout << " ???\n";
	}

	return std::make_tuple(width, height);
}

std::string setRecordingFolder()
{
	// Select Output Folder:
	std::string folder;

	std::cout << "\n-----------------------------------------------------------------" << std::endl;
	std::cout << "\nSelect Recording Folder:" << std::endl;
	std::cout << "\nImages are recorded in C:\\Users\\duponcga\\Documents\\ ....selected Folder:" << std::endl;
	std::cout << "\nEnter the selected Folder: ";
	std::cin >> folder;

	// Create Recorded Images Folder:
	std::string directoryName = "C:\\Users\\duponcga\\Documents\\" + folder;
	if (!_mkdir(directoryName.c_str())) std::cout << "\nFichier: " << directoryName << "\tOK!\n";
	else
	{
		if (!fopen(directoryName.c_str(), "rb")) std::cout << "\nFichier: " << directoryName << "\tOK!\n";
		else std::cout << "\nFichier: " << directoryName << "tPROBLEM!n";
	}

	return directoryName;
}

int setAquisitionMode()
{
	std::cout << "-----------------------------------------------------------------" << std::endl;
	std::cout << "\nSelect Aquisition Mode:\n" << std::endl;

	std::cout << "\t1  FOR  Continue Recording" << std::endl;
	std::cout << "\t2  FOR      Snap Recording" << std::endl;

	int mode;
	std::cout << "\nEnter the selected value: ";
	std::cin >> mode;

	if (mode != 1 && mode != 2) setAquisitionMode();

	return mode;
}

void launchZedRawContinueRecord(std::string directoryName, cv::VideoCapture zed_cap)
{
	std::cout << "\n-----------------------------------------------------------------" << std::endl;
	std::cout << "\nPress a key to start the record\n" << std::endl;

	system("PAUSE");

	std::cout << "\n-----------------------------------------------------------------" << std::endl;
	std::cout << "\nImages Created: (Press a key to stop the record)\n" << std::endl;
	
	int i = 1;

	while( zed_cap.grab() && !kbhit() )
	{
		
		cv::Mat sbs;
		zed_cap.retrieve(sbs);

		cv::imshow("SBS ZED", sbs);

		std::string double_image_path = directoryName + "\\image_" + std::to_string(i) + ".png";
		if (cv::imwrite(double_image_path, sbs)) std::cout << double_image_path << std::endl;

		cv::Mat left = sbs(cv::Range(0, sbs.rows) , cv::Range(0, sbs.cols / 2));
		cv::Mat right = sbs(cv::Range(0, sbs.rows), cv::Range(sbs.cols / 2, sbs.cols));

		std::string left_image_path  = directoryName + "\\image_" + std::to_string(i) + "_left.png" ;
		std::string right_image_path = directoryName + "\\image_" + std::to_string(i) + "_right.png";

		if (cv::imwrite(left_image_path , left )) std::cout << left_image_path << std::endl;
		if (cv::imwrite(right_image_path, right)) std::cout << right_image_path + "\n" << std::endl;

		i++;
		cv::waitKey(20);
	}
}

void launchZedRawSnapRecord(std::string directoryName, cv::VideoCapture zed_cap)
{
	std::cout << "\n-----------------------------------------------------------------" << std::endl;
	std::cout << "\nPress each time SPACE BAR to take a SNAP or ESC to Quit\n" << std::endl;

	system("PAUSE");

	int i = 1;
	
	while (zed_cap.grab())
	{
		zed_cap.grab();
		cv::Mat sbs;
		zed_cap.retrieve(sbs);

		cv::imshow("SBS ZED", sbs);
		
		if (GetAsyncKeyState(' '))
		{
			std::string double_image_path = directoryName + "\\image_" + std::to_string(i) + ".png";
			if (cv::imwrite(double_image_path, sbs))	std::cout << double_image_path << std::endl;

			cv::Mat left  = sbs(cv::Range(0, sbs.rows), cv::Range(0, sbs.cols / 2));
			cv::Mat right = sbs(cv::Range(0, sbs.rows), cv::Range(sbs.cols / 2, sbs.cols));

			std::string left_image_path  = directoryName + "\\image_" + std::to_string(i) + "_left.png";
			std::string right_image_path = directoryName + "\\image_" + std::to_string(i) + "_right.png";

			if (cv::imwrite(left_image_path , left )) std::cout << left_image_path << std::endl;
			if (cv::imwrite(right_image_path, right)) std::cout << right_image_path + "\n" << std::endl;

			i++;
		}

		if (GetAsyncKeyState(27)) break;
		
		cv::waitKey(20);
	} 
}


int main(int argc, char* argv[])
{
	
	//--------------------------------------------------------------//
	// Select Resolution:                                           //
	//--------------------------------------------------------------//

	int width, height;
	std::tie(width, height) = set_Resolution();


	//--------------------------------------------------------------//
	// Init Camera and Set Resolution:                              //
	//--------------------------------------------------------------//

	cv::VideoCapture zed_cap(0);
		
	zed_cap.set(cv::CAP_PROP_FRAME_WIDTH , width );
	zed_cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

	std::cout << "\nSelected Resolution: " << zed_cap.get(cv::CAP_PROP_FRAME_WIDTH) << "x" << zed_cap.get(cv::CAP_PROP_FRAME_HEIGHT) << "\n"<< std::endl;
	
	
	//--------------------------------------------------------------//
	// Select Mode:                                                 //
	//--------------------------------------------------------------//

	int mode = setAquisitionMode();


	//--------------------------------------------------------------//
	// Select and Create Recording Folder:                          //
	//--------------------------------------------------------------//

	std::string directoryName = setRecordingFolder();


	//--------------------------------------------------------------//
	// Record data from ZED Camera:                                 //
	//--------------------------------------------------------------//

	if(mode == 1) launchZedRawContinueRecord(directoryName, zed_cap);
	if(mode == 2) launchZedRawSnapRecord(directoryName, zed_cap)    ;

	system("PAUSE");


	//--------------------------------------------------------------//

	return 0;
}

#endif

