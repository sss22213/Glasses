#include <opencv2/core/core.hpp>
#include <string>
#include <iostream>
#include <windows.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <vector>
#include "C:\\Webcam_AR\\opencv\\include\\opencv2\\core\\cuda.hpp"
#include <array>
using namespace std;
using namespace cv;
using namespace System;
using namespace System::IO;


namespace Mywebcam
{
	/** @defgroup The starting position of the picture
	*
	* @{*/
	/** @briefPuts The Maximum number of pictures */
	#define MAX_TASK 100
	/** @brief The Maximum number of trig region */
	#define Trig_num 100 
	/** @brief Trig percent */
	float Trig_Percent = 0.5;
	/** @brief Trig threshold */
	int threshold_num=1000;
	/** @} */

/** @brief Provide webcam function
*/
	//webcam class
	class webcam
	{
	private:
		/** @defgroup The starting position of the picture
		*
		* @{*/
		/** @brief The coordinate of the X-axis at the trig point */
		int Trig_X[Trig_num];
		/** @brief The coordinate of the Y-axis at the trig point */
		int Trig_Y[Trig_num];
		/** @brief The coordinate of the X-axis at the trig range */
		int Trig_regX[Trig_num];
		/** @brief The coordinate of the Y-axis at the trig range */
		int Trig_regY[Trig_num];
		/** @} */
		/** @brief Trig effective flag*/
		int Effective[Trig_num];

		/** @brief Contained trigger container */
		int(*Trig_callfunc[Trig_num])(void);

		/** @brief Webcam body*/
		VideoCapture capture;

		/** @brief Background picture*/
		UMat Parent_frame;

	public:
		//webcam(){};
		~webcam() {};
		/** @brief webcam Constructor 
		*	@param	Ocapture: Webcam body
		*	@param	WIDTH: Webcam width
		*	@param	HEIGHT: Webcam height
		*	@param	FPS:	Webcam FPS
		*/
		webcam(VideoCapture Ocapture, int WIDTH, int HEIGHT, int FPS)
		{
			if (WIDTH < 200 || HEIGHT < 200 || FPS < 10)
			{
				WIDTH = 1280;
				HEIGHT = 720;
				FPS = 30;
			}
			capture = Ocapture;
			//Setting WebCam Format
			capture.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
			capture.set(CV_CAP_PROP_FPS, FPS);
			//Setting WebCam zero lag
			capture.set(CV_CAP_PROP_BUFFERSIZE, 0);
		};
		/** @brief	Catch background picture into Parent_frame
		*	@return	0: successfully
		*/
		int webcam_Trig_init()
		{
			//Origin frame
			capture >> this->Parent_frame;
			//Init Trig Array
			for (int i = 0; i < Trig_num; i++)
			{
				this->Trig_X[i] = -1;
				this->Trig_Y[i] = -1;
				this->Trig_regX[i] = -1;
				this->Trig_regY[i] = -1;
				this->Effective[i] = -1;
			}
			return 0;
		}
		/** @brief	Get image from webcam
		*	@return	image
		*/
		UMat Catch_image()
		{
			UMat frame;
			capture >> frame;		
			return frame;
		}

#define  Trig_Create Trig_Pos_Change //Lazy to Write
		/** @brief	Create Trig point
		*	@param	OTrig_X: The coordinate of the X-axis at the start point
		*	@param	OTrig_Y: The coordinate of the Y-axis at the start point
		*	@param	OTrig_regX: The coordinate of the X-axis at the trig range 
		*	@param	OTrig_regX: The coordinate of the Y-axis at the trig range
		*	@param	prior1: Trigger priority
		*	@return	0: successfully
		*/
		int Trig_Create(int OTrig_X, int OTrig_Y, int OTrig_regX, int OTrig_regY, int prior1)
		{
			if (prior1 > Trig_num - 1)
			{
				return -1;
			}

			this->Trig_X[prior1] = OTrig_X;
			this->Trig_Y[prior1] = OTrig_Y;
			this->Trig_regX[prior1] = OTrig_regX;
			this->Trig_regY[prior1] = OTrig_regY;
			this->Effective[prior1] = 1;
			return 0;
		}
		/** @brief	Trig param try
		*	@return	0: successfully
		*/
		int Trig_param(int oTrig_Percent)
		{
			cv::Point left_top, right_down;
			left_top.x = 100; left_top.y = 100;
			right_down.x = 100; right_down.y = 100;
			oTrig_Percent = Trig_Percent;
			//update webcam picture
			UMat update_frame;
			UMat imageROI0;
			UMat imageROI1;
			//init threshold_num
			int threshold_tweak = threshold_num;
			int move_flag = 1;
			capture >> update_frame;
			//Parent frame
			imageROI0 = Parent_frame(Rect(left_top.x, left_top.y, right_down.x, right_down.y));
			//Sub frame
			imageROI1 = update_frame(Rect(left_top.x, left_top.y, right_down.x, right_down.y));
			while (move_flag)
			{
				//threshold
				threshold(imageROI1, imageROI0, threshold_tweak, 255, THRESH_BINARY_INV);
				double Sum_pixel = 0;
				for (int i = 0; i < 3; i++)Sum_pixel += sum(imageROI0)[i];
				//NON black Percentage
				if (Sum_pixel / ((float)right_down.x * (float)right_down.y * 255.0 * 3.0) < Trig_Percent)move_flag = 0;
				threshold_tweak -=5;
				cout << threshold_tweak << endl;
				if (threshold_tweak < 50)return -1;
				cv::waitKey(30);
			}
			threshold_num = threshold_tweak-10;
			return threshold_tweak - 10;
		}
		/** @brief	Delete Trig point
		*	@param	prior1: Trigger priority
		*	@return	0: successfully
		*/
		int Trig_Del(int prior1)
		{
			/*this->Trig_X[prior1] = -1;
			this->Trig_Y[prior1] = -1;
			this->Trig_regX[prior1] = -1;
			this->Trig_regY[prior1] = -1;*/
			this->Effective[prior1] - 1;
			return 0;

		}
		/** @brief	Restore Trig point
		*	@param	prior1: Trigger priority
		*	@return	0: successfully
		*/
		int Trig_Restore(int prior1)
		{
			this->Effective[prior1] = 1;
			return 0;
		}
		/** @brief	Hide Trig point
		*	@param	prior1: Trigger priority
		*	@return	0: successfully
		*/
		int Trig_hide(int prior1)
		{

			this->Effective[prior1] = -1;
			return 0;
		}
		/** @brief	Trig function using background subtraction 
		*	@param	left_top: Point at upper left
		*	@param	right_down: Trig range
		*	@return	1: Trig
		*	@return 0: Not Trig
		*/
		int Trig_func(cv::Point left_top, cv::Point right_down)
		{
			//update webcam picture
			UMat update_frame;
			UMat imageROI0;
			UMat imageROI1;
			capture >> update_frame;
			//Parent frame
			imageROI0= Parent_frame(Rect(left_top.x, left_top.y, right_down.x, right_down.y));
			//Sub frame
			imageROI1 = update_frame(Rect(left_top.x, left_top.y, right_down.x, right_down.y));
			//threshold
			threshold(imageROI1, imageROI0, threshold_num, 255, THRESH_BINARY_INV);
			double Sum_pixel = 0;
			for (int i = 0; i < 3; i++)Sum_pixel += sum(imageROI0)[i];
			//NON black Percentage
			if (Sum_pixel / ((float)right_down.x * (float)right_down.y * 255.0 * 3.0) > Trig_Percent)return 1;
			return 0;
		}
		/** @brief	Trig function using background subtraction
		*	@return	i: The i th trigger area is triggered
		*	@return 0: Not Trig
		*/
		int Trig_func()
		{
			//update webcam picture
			UMat update_frame;
			UMat imageROI0;
			UMat imageROI1;
			for (int i = 0; i < Trig_num - 1; i++)
			{
				/*if (this->Trig_X[i] == -1 || this->Trig_Y[i] == -1 || this->Trig_regX[i] == -1 || this->Trig_regY[i] == -1)
				{
				continue;
				}*/
				if (this->Effective[i] == -1)
				{
					continue;
				}
				capture >> update_frame;
				//waitKey(30);
				//Parent frame
				imageROI0 = Parent_frame(Rect(Trig_X[i], Trig_Y[i], Trig_regX[i], Trig_regY[i]));
				//Sub frame
				imageROI1 = update_frame(Rect(Trig_X[i], Trig_Y[i], Trig_regX[i], Trig_regY[i]));
				//threshold
				threshold(imageROI1, imageROI0, threshold_num, 255, THRESH_BINARY_INV);
				double Sum_pixel = 0;
				for (int i = 0; i < 3; i++)
				{
					//Scalar summary
					Sum_pixel += sum(imageROI0)[i];
				}

				//NON black Percentage
				if (Sum_pixel / ((float)Trig_regX[i] * (float)Trig_regY[i] * 255.0 * 3.0) > Trig_Percent)
				{
					//Call Action Function
					return i;
				}

			}
			return 0;
		}
		/** @brief	Trig function using color condition
		*	@return	i: The i th trigger area is triggered
		*	@return 0: Not Trig
		*/
		int Trig_func1()
		{
			//update webcam picture
			UMat update_frame;
			UMat imageROI0;
			UMat imageROI1;
			UMat hsv;
			UMat b; //各顏色的閥值
			for (int i = 0; i < Trig_num - 1; i++)
			{
				if (this->Effective[i] == -1)
				{
					continue;
				}
				capture.read(update_frame);

				imageROI1 = update_frame(Rect(Trig_X[i], Trig_Y[i], Trig_regX[i], Trig_regY[i]));
				UMat mask = UMat::zeros(imageROI1.rows, imageROI1.cols, CV_8U); //為了濾掉其他顏色
				cvtColor(imageROI1, hsv, CV_BGR2HSV);
				inRange(hsv, Scalar(90, 100, 0), Scalar(130, 255, 255), b);

				double Sum_pixel = 0;
				for (unsigned char i = 0; i < 3; i++)
				{
					//Scalar summary
					Sum_pixel += sum(b)[i];
				}
				//return Sum_pixel;

				if (Sum_pixel / ((float)Trig_regX[i] * (float)Trig_regY[i] * 255.0 * 3.0) > 0.015)
				{
					//Call Action Function
					return i;
				}
			}
			return 0;
		}
		/** @brief	Trig function using color condition
		*	@param	left_top: Point at upper left
		*	@param	right_down: Trig range
		*	@return	i: The i th trigger area is triggered
		*	@return 0: Not Trig
		*/
		int Trig_Color(cv::Point left_top, cv::Point right_down)
		{
			//update webcam picture
			UMat update_frame;
			UMat imageROI0;
			UMat imageROI1;
			UMat hsv;
			UMat b; //各顏色的閥值
			capture >> update_frame;
			imageROI1 = update_frame(Rect(left_top.x, left_top.y, right_down.x, right_down.y));
			UMat mask = UMat::zeros(imageROI1.rows, imageROI1.cols, CV_8U); //為了濾掉其他顏色
			cvtColor(imageROI1, hsv, CV_BGR2HSV);
			inRange(hsv, Scalar(90, 100, 0), Scalar(130, 255, 255), b);
			double Sum_pixel = 0;
			for (unsigned char i = 0; i < 3; i++)Sum_pixel += sum(b)[i];
			//return Sum_pixel;
			if (Sum_pixel / ((float)(right_down.x- left_top.x) * (float)(right_down.y- left_top.y) * 255.0 * 3.0) > 0.015)return 1;
			return 0;
		}
	};
}