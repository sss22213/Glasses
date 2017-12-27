#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <vector>
#include <windows.h>
#include <iostream>
#include "C:\\Webcam_AR\\opencv\\include\\opencv2\\core\\cuda.hpp"
#include <array>
using namespace cv;
using namespace std;
using namespace System;

namespace MyPicture
{
	/** @brief Loading, setting and get picture information
	*/
	class Picture
	{
	private:
		/** ID of picture*/
		int ID;
		/** Path of picture*/
		string Path;
		/** Image*/
		UMat image;
	public:
		/** Image using GPU*/
		cv::cuda::GpuMat image_GPU;
		/** @defgroup The starting position of the picture
		* 
		* @{*/
		/** @brief The coordinate of the X-axis at the start point of the picture */
		int position_x;
		/** @brief The coordinate of the Y-axis at the start point of the picture */
		int position_y;
		/** @} */

		/** @defgroup picture border
		* 
		* @{*/
		/** @brief The coordinate of the X-axis at the picture border 
		*
		*	- pic_boarder_x[0][0] is X coordinate of the upper left
		*	- pic_boarder_x[0][1] is X coordinate of the upper right
		*	- pic_boarder_x[1][0] is X coordinate of the down left
		*	- pic_boarder_x[1][1] is X coordinate of the down right
		*/
		int pic_boarder_x[2][2];
		/** @brief The coordinate of the Y-axis at the picture border 
		*
		*	- pic_boarder_y[0][0] is Y coordinate of the upper left
		*	- pic_boarder_y[0][1] is Y coordinate of the upper right
		*	- pic_boarder_y[1][0] is Y coordinate of the down left
		*	- pic_boarder_y[1][1] is Y coordinate of the down right
		*/
		int pic_boarder_y[2][2];
		/** @} */

		~Picture() {};
		/** @brief Picture Constructor 
		*@param InputMat: Input of picture
		*@param	ID: ID of picture
		*/
		Picture(UMat InputMat, int ID)
		{
			this->ID = ID;
			this->position_x = -1;
			this->position_y = -1;
			this->image = InputMat;
			this->Picture_Load();
		}
		/** @brief Picture Constructor
		*@param InputMat: Input of picture
		*@param	ID: ID of picture
		*@param position_x: The coordinate of the X-axis at the start of the picture
		*@param position_y: The coordinate of the Y-axis at the start of the picture
		*/
		Picture(UMat InputMat, int ID, int position_x, int position_y)
		{
			this->ID = ID;
			this->position_x = position_x;
			this->position_y = position_y;
			this->image = InputMat;
			this->Picture_Load();
		}
		/** @brief Picture Constructor
		*@param Path: Path of picture
		*@param	ID: ID of picture
		*@param position_x: The coordinate of the X-axis at the start of the picture
		*@param position_y: The coordinate of the Y-axis at the start of the picture
		*/
		Picture(string Path, int ID, int position_x, int position_y)
		{
			this->ID = ID;
			this->Path = Path;
			this->position_x = position_x;
			this->position_y = position_y;
			//Check exist of picture
			FILE *f1;
			f1 = fopen((this->Path).c_str(), "r");
			if (!f1)
			{
				//File too Large
				std::cerr << E_NOTIMPL;
			}
			fclose(f1);
			//Init Picture
			Mat pic = imread(this->Path, CV_LOAD_IMAGE_COLOR);
			pic.copyTo(this->image);
			//this->image_GPU.upload(this->image);
			this->Picture_Load();
		}
		/** @brief Picture Load
		*@return S_OK: Loaded successfully
		*@return E_NOTIMP: ID is not illegal
		*/
		HRESULT Picture_Load()
		{
			if (this->ID < 0)
			{
				return E_NOTIMPL;
			}
			//Calculate picture boarder
			pic_boarder_x[0][0] = this->position_x;
			pic_boarder_y[0][0] = this->position_y;

			pic_boarder_x[0][1] = this->position_x + this->image.cols;
			pic_boarder_y[0][1] = this->position_y;

			pic_boarder_x[1][0] = this->position_x;
			pic_boarder_y[1][0] = this->position_y + this->image.rows;

			pic_boarder_x[1][1] = this->position_x + this->image.cols;
			pic_boarder_y[1][1] = this->position_y + this->image.rows;

			//bondary check 
			return S_OK;
		}
		/** @brief Change position of picture
		*@param position_x: The coordinate of the X-axis
		*@param position_y: The coordinate of the Y-axis
		*@return S_OK: successfully
		*/
		HRESULT Add_Position(int position_x, int position_y)
		{
			this->position_x = position_x;
			this->position_y = position_y;
			//Calculate picture boarder
			pic_boarder_x[0][0] = this->position_x;
			pic_boarder_y[0][0] = this->position_y;

			pic_boarder_x[0][1] = this->position_x + this->image.cols;
			pic_boarder_y[0][1] = this->position_y;

			pic_boarder_x[1][0] = this->position_x;
			pic_boarder_y[1][0] = this->position_y + this->image.rows;

			pic_boarder_x[1][1] = this->position_x + this->image.cols;
			pic_boarder_y[1][1] = this->position_y + this->image.rows;
			return S_OK;
		}
		/** @brief Change image of picture
		*@param InputMat: image
		*@return S_OK: successfully
		*/
		HRESULT Change_Mat(UMat InputMat)
		{

			this->image = InputMat;
			return S_OK;
		}
		/** @brief Display image using opencv window
		*@param window_name: Window name
		*@return S_OK: successfully
		*/
		HRESULT Show_Picture(const string& window_name)
		{
			//namedWindow(window_name, CV_WINDOW_OPENGL);
			cv::imshow(window_name, this->image);
			return S_OK;
		}

		/** @brief Resize image
		*@param x: The horizontal size of the output
		*@param y: The vertical size of the output
		*@param fx: Output of the horizontal magnification
		*@param fy: Output of the vertical magnification
		*@return S_OK: successfully
		*/
		HRESULT Picture_Matresize(int x = 0, int y = 0, float fx = 1.5, float fy = 1.5)
		{
			cv::Size desize(x, y);
			cv::resize(image, image, desize, fx, fy);
			return S_OK;
		}
		/** @brief Resize image
		*@param x: The horizontal size of the output
		*@param y: The vertical size of the output
		*@param fx: Output of the horizontal magnification
		*@param fy: Output of the vertical magnification
		*@return S_OK: successfully
		*/
		HRESULT Picture_resize(int x = 0, int y = 0, float fx = 1.5, float fy = 1.5)
		{

			cv::Size desize(x, y);
			//Image from GPU to CPU
			Mat temp;
			image.copyTo(temp);
			cv::resize(temp, temp, desize, fx, fy);
			//temp from CPU to GPU
			temp.copyTo(image);
			/*
			temp=image.getMat(1);
			cv::resize(temp, temp, desize, fx, fy);*/
			return S_OK;
		}
		/** @brief Get position of image
		*@return X coordinate of the upper left
		*/
		int Get_Pos_X()
		{
			return position_x;
		}
		/** @brief @brief Get position of image
		*@return Y coordinate of the upper left
		*/
		int Get_Pos_Y()
		{
			return position_y;
		}
		/** @brief Get image
		*@return image
		*/
		UMat Get_image()
		{
			return image;
		}
		/** @brief Get ID of picture
		*@return ID of picture
		*/
		int Get_ID()
		{
			return this->ID;
		}
	};

	/** @brief Save picture
	*
	*/
	class MyPictureBox
	{
	private:
		/** @brief Point to head of picture box
		*
		*/
		vector<Picture*> Picture_Box;
	public:
		/** @brief MyPictureBox Constructor
		*
		*/
		MyPictureBox()
		{
			this->MyPictureBox_clear();
		}
		~MyPictureBox() {}
		/** @brief MyPictureBox Constructor
		*@return picture box
		*/
		vector<Picture*> *MyPictureBox_Get()
		{
			return &Picture_Box;
		}
		/** @brief Clear picture box
		*@return S_OK: successfully
		*/
		HRESULT MyPictureBox_clear()
		{
			this->Picture_Box.clear();
			return S_OK;
		}
		/** @brief Put picture into picture box
		*@return S_OK: successfully
		*/
		HRESULT MyPictureBox_put(Picture *Input_pic)
		{
			Picture_Box.push_back(Input_pic);
			return S_OK;
		}
		/** @brief Delete picture from picture box
		*@param index: index of picture
		*@return S_OK: successfully
		*/
		HRESULT MyPictureBox_Del(int index)
		{
			//if vector size is 1,than clear, else erase index
			if (Picture_Box.size())MyPictureBox_clear();
			else Picture_Box.erase(Picture_Box.begin() + index);
			return S_OK;
		}
		/** @brief Put many picture into picture box
		*@param Input_pic: Point to point picture object
		*@param count: Count of Picture Box
		*@return S_OK: successfully
		*/
		HRESULT MyPictureBox_Multiple_Put(Picture **Input_pic, int count)
		{
			for (int i = 0; i<count; i++)Picture_Box.push_back(Input_pic[i]);
			return S_OK;
		}
		/** @brief Put picture into picture box using index
		*@param Input_pic: picture object
		*@param index: picture index
		*@return S_OK: successfully
		*/
		HRESULT MyPictureBox_Put_Index(Picture *Input_pic, int index)
		{
			Picture_Box.at(index) = Input_pic;
			return S_OK;
		}
		/** @brief Get picture box length
		*@return picture box length
		*/
		int MyPictureBox_get_size()
		{
			return Picture_Box.size();
		}
	};
	/** @brief Display, plot into window
	*
	*/
	class view
	{
		MyPictureBox *PicBox;
		Picture* background;
	public:
		/** @brief MyPictureBox Constructor
		*@param Inputbackground: Canvas background
		*@param InputPicBox: Picture box of put
		*/
		view(Picture* Inputbackground, MyPictureBox *InputPicBox)
		{
			this->background = Inputbackground;
			this->PicBox = InputPicBox;
		}
		/** @brief Put picture into Canvas
		*@return S_OK: successfully
		*/
		HRESULT Image_puts()
		{
			vector <Picture*>::iterator it = (PicBox->MyPictureBox_Get())->begin();
			for (int i = 0; it != (PicBox->MyPictureBox_Get())->end(); it++, i++)
			{
				UMat back = (background->Get_image());
				//kinect's Color frame is 8UC4 , but Mat is 8UC3.Therefore convert it.
				cvtColor(back, back, CV_BGRA2BGR);
				UMat logo = (PicBox->MyPictureBox_Get())->at(i)->Get_image();
				//if (!logo.data) { cerr << "Error" << endl; }
				//Mat Temp
				UMat ImageROT;
				//Mask
				UMat mask = logo;
				//Setting area
				ImageROT = back(Rect((PicBox->MyPictureBox_Get()->at(i))->position_x, (PicBox->MyPictureBox_Get()->at(i))->position_y, logo.cols, logo.rows));
				//Tweak color(UMat is not implenment)
				// ImageROT from GPU to CPU
				Mat temp,temp1;
				ImageROT.copyTo(temp);
				logo.copyTo(temp1);
				addWeighted(temp, 1, logo, 0, 0, temp);
				// ImageROT from GPU to CPU
				temp.copyTo(ImageROT);
				temp1.copyTo(logo);
				//Picture
				logo.copyTo(ImageROT, mask);
				//save return background
				background->Change_Mat(back);
			}
			return S_OK;
		}
		/** @brief Move picture position from Canvas
		*@param InputPic: picture object
		*@param offset: Move distance
		*@param direction: 1(Move horizontally),0(Move vertically)
		*@return S_OK: successfully
		*/
		HRESULT Image_Move(Picture *InputPic, int offset, int direction = 1)
		{
			if (direction == 1)
			{
				InputPic->Add_Position(InputPic->Get_Pos_X() + offset, InputPic->Get_Pos_Y());
			}
			else if (direction == 0)
			{
				InputPic->Add_Position(InputPic->Get_Pos_X(), InputPic->Get_Pos_Y() + offset);
			}
			else if (direction == -1)
			{
				InputPic->Add_Position(InputPic->Get_Pos_X() - offset, InputPic->Get_Pos_Y());
			}
			return S_OK;
		}
		/** @brief Plot Line into Canvas
		*@param x1: The coordinate of the X-axis at the start point
		*@param x2: The coordinate of the Y-axis at the start point
		*@param y1: The coordinate of the X-axis at the end point
		*@param y2: The coordinate of the Y-axis at the end point
		*@param color: BACKGROUND_BLUE(Blue)
		*@param thickness: Line width
		*@param direction: 1(Move horizontally),0(Move vertically)
		*@return S_OK: successfully
		*/
		HRESULT Plot_Line(int x1, int x2, int y1, int y2, const Scalar& color = (BACKGROUND_BLUE | BACKGROUND_INTENSITY), int thickness = 12)
		{
			cv::Point P1(x1, y1);
			cv::Point P2(x2, y2);
			cv::line(background->Get_image(), P1, P2, color, thickness);
			return S_OK;
		}
		/** @brief Plot circle into Canvas
		*@param x: The coordinate of the X-axis at the start point
		*@param y: The coordinate of the Y-axis at the start point
		*@param radius: Circule radius
		*@param color: BACKGROUND_BLUE(Blue)
		*@param thickness: Line width
		*@return S_OK: successfully
		*/
		HRESULT Plot_Circle(int x, int y, int radius, const Scalar& color = (BACKGROUND_BLUE | BACKGROUND_INTENSITY), int thickness = 10)
		{
			cv::Point P1(x, y);
			cv::circle(background->Get_image(), P1, radius, color, thickness);
			return S_OK;
		}
		/** @brief Plot Text into Canvas
		*@param x: The coordinate of the X-axis at the start point
		*@param y: The coordinate of the Y-axis at the start point
		*@param textint: Text body
		*@param fontScale: Font style
		*@param fontFace: font size
		*@param color: BACKGROUND_BLUE(Blue)
		*@param thickness: Text width
		*@return S_OK: successfully
		*/
		HRESULT Plot_Text(int x, int y, const string& textint, double fontScale = 4, int fontFace = FONT_HERSHEY_COMPLEX, const Scalar& color = (BACKGROUND_BLUE | BACKGROUND_INTENSITY), int thickness = 10)
		{
			cv::Point P1(x, y);
			cv::putText(background->Get_image(), textint, P1, fontFace, fontScale, color, thickness);
			return S_OK;
		}
		/** @brief Plot track into Canvas
		*@param track: Point of track
		*@param Line_weight: Line width
		*@return S_OK: successfully
		*/
		HRESULT plot_Two_Joint(vector<cv::Point> track, int Line_weight)
		{
			vector <cv::Point>::iterator it = track.begin();
			for (int i = 0; (it + 1) != track.end(); it++, i++)
			{
				Plot_Line(track[i].x, track[i + 1].x, track[i].y, track[i + 1].y, (BACKGROUND_BLUE | BACKGROUND_INTENSITY), Line_weight);
			}
			return S_OK;
		}
		//
		HRESULT plot_Rectangle(cv::Point *pt1, int Line_weight = 20)
		{
			cv::rectangle(background->Get_image(), pt1[0], pt1[1], (BACKGROUND_BLUE | BACKGROUND_INTENSITY), Line_weight);
			return S_OK;
		}
		//
		HRESULT plot_Rectangle(cv::Point pt1, cv::Point pt2, int Line_weight = 20)
		{
			cv::rectangle(background->Get_image(), pt1, pt2, (BACKGROUND_BLUE | BACKGROUND_INTENSITY), Line_weight);
			return S_OK;
		}
		//
		HRESULT plot_Rectangle(int x1, int x2, int y1, int y2, int Line_weight = 20)
		{
			cv::Point pt1(x1, y1);
			cv::Point pt2(x2, y2);
			cv::rectangle(background->Get_image(), pt1, pt2, (BACKGROUND_BLUE | BACKGROUND_INTENSITY), Line_weight);
			return S_OK;
		}
		/** @brief Get background from Canvas
		*@return Background
		*/
		Picture* Get_background()
		{
			return background;
		}
	};
}
