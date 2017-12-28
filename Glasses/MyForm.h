#pragma once
//#include "MyKinect.h"
//#include "MyKinect.h"
#include "core.h"
#include <windows.h>
#include "opencv2/core/ocl.hpp"
#include <mmsystem.h>
#include "glut.h"
#include "Picture.h"
#include "C:\\Webcam_AR\\opencv\\include\\opencv2\\core\\opengl.hpp"
#pragma comment(lib,"Winmm.lib")
#include "opencv2/imgproc/imgproc.hpp"
namespace Glasses {
	using namespace MyPicture;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		//backgroundWorker1->RunWorkerAsync();
		float count;
		int Trig_count;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Timer^  timer2;
	public: System::Windows::Forms::Button^  button2;
	public:	cv::VideoCapture *Ocapture;
	public: Mywebcam::webcam *Webcam;
	public:	Picture *Color_background;
	public:	Picture *P2;
	public:	Picture *P3;
	public:	Picture *P4;
	public:	Picture *P5;
	public:	Picture *P6;
	public: view *View1;
	public:	MyPictureBox *Box1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker2;






	public:
	public: cv::Point *pps;
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:


	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->backgroundWorker2 = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &MyForm::timer1_Tick);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker1_DoWork);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(3, 375);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(142, 109);
			this->button1->TabIndex = 0;
			this->button1->Text = L"絤策家Α";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(377, 375);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(142, 109);
			this->button2->TabIndex = 1;
			this->button2->Text = L"代喷家Α";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"PMingLiU", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label1->Location = System::Drawing::Point(182, 209);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(156, 64);
			this->label1->TabIndex = 2;
			this->label1->Text = L"碞狐";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// timer2
			// 
			this->timer2->Tick += gcnew System::EventHandler(this, &MyForm::timer2_Tick);
			// 
			// backgroundWorker2
			// 
			this->backgroundWorker2->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MyForm::backgroundWorker2_DoWork);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(531, 496);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
		//OpenCV CPU ISA Optimized
		setUseOptimized(true);
		this->Ocapture = new cv::VideoCapture(0);
		this->Webcam = new Mywebcam::webcam(*this->Ocapture, 1280, 720, 30);
		//Access to Platform
		Webcam->webcam_Trig_init();
		/** @brief Object allocate space */
		Color_background = new Picture(Webcam->Catch_image(), 0);
		P2 = new Picture("C:\\Webcam_AR\\image\\base1.png", 2, 0, 0);
		P3 = new Picture("C:\\Webcam_AR\\image\\left_to_right.png", 3, 0, 0);
		P4 = new Picture("C:\\Webcam_AR\\image\\right_to_left.png", 3, 0, 0);
		P5 = new Picture("C:\\Webcam_AR\\image\\top_to_buttom.png", 3, 0, 0);
		P6 = new Picture("C:\\Webcam_AR\\image\\error.png", 3, 0, 0);
		P2->Picture_resize(0, 0, 0.5, 0.5);
		P3->Picture_resize(0, 0, 0.5, 0.5);
		P4->Picture_resize(0, 0, 0.5, 0.5);
		P5->Picture_resize(0, 0, 0.5, 0.5);
		P6->Picture_resize(0, 0, 0.5, 0.5);
		Box1 = new MyPictureBox;
		Box1->MyPictureBox_put(P2);
		View1= new view(Color_background, Box1);
		pps = new cv::Point(2);
		pps[0].x = View1->Get_background()->Get_image().cols / 2 - 500;
		pps[0].y = 30;
		pps[1].x = View1->Get_background()->Get_image().cols / 2 + 100;
		pps[1].y = (View1->Get_background()->Get_image()).rows / 2 + 300;
		P2->Add_Position(pps[0].x, 30);
		P3->Add_Position(P2->pic_boarder_x[0][1], (P2->pic_boarder_y[0][1] + P2->pic_boarder_y[1][1]) / 2);
		Box1->MyPictureBox_put(P3);
		//Update trig percent data 
		//textBox1->Text = Convert::ToString(Mywebcam::threshold_num);
		
		//Init System Time(10 ms)
		this->Trig_count = 0;
		this->count = 0;
		timer1->Interval = 1000;
		timer1->Stop();
		timer2->Interval = 50;
		timer2->Stop();
	}
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		count++;
	}
private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
	
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	

}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	
		while (backgroundWorker2->IsBusy);
		backgroundWorker2->RunWorkerAsync();
		while (1)
		{
			Color_background->Change_Mat(Webcam->Catch_image());
			View1->plot_Rectangle(pps, 15);
			View1->Image_puts();
			View1->Get_background()->Show_Picture("絤策家Α");
			cv::waitKey(30);
		}
}
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void timer2_Tick(System::Object^  sender, System::EventArgs^  e) {
	this->Trig_count++;
}
private: System::Void backgroundWorker2_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
		//Read trig param (Mywebcam::Trig_Percent,Mywebcam::threshold_num)
		//Mywebcam::threshold_num = Convert::ToInt32(textBox1->Text);
		//Mywebcam::Trig_Percent = Convert::ToInt32(textBox2->Text);
		//Mywebcam::threshold_num = 100;
		//Access to Platform
		Webcam->webcam_Trig_init();
		//Effective Range
		int Trig_x = pps[0].x;
		int Trig_y = pps[0].y;
		cv::Point pp1(Trig_x, Trig_y);
		cv::Point pp2(Trig_x + P2->Get_image().cols, Trig_y + P2->Get_image().rows);

		//Terminal Range
		const int horizontal_Terminal = 50;
		const int vertical_Terminal = 10;

		//Error Range
		cv::Point error_PP1(Trig_x, Trig_y);
		cv::Point error_pp2(Trig_x + P2->Get_image().cols, Trig_y + P2->Get_image().rows);

		//Picture flag
		int move_flag = 0;

		//Initial flag
		int init_flag = 0;

		//Effective argument
		const int Trig_region_decrease = 10;
		const int horizontal_move = 10;
		const int vertical_move = 10;

		//Error argument
		const int Trig_region_increase = 50;

		//Sound dataset
		LPCTSTR Error_Sound = L"F:\\Webcam_AR\\sound\\error.wav";

		//Start timer
		timer1->Start();
		while (1)
		{
			//Trig Effective
			if (Webcam->Trig_Color(pp1, pp2))
			{
				//unlock Initial flag
				int init_flag = 1;
				enum
				{
					left_to_right,
					top_to_down_one,
					right_to_left,
					top_to_down_two,
				};
				//Change Picture Position
				switch (move_flag)
				{
				case left_to_right:
					View1->Image_Move(P2, horizontal_move);
					View1->Image_Move(P3, horizontal_move);
					if (P2->pic_boarder_x[0][1] > pps[1].x)
					{
						//Change Arrow
						Box1->MyPictureBox_Put_Index(P5, 1);
						P5->Add_Position(P3->Get_Pos_X(), P3->Get_Pos_Y());
						move_flag = 1;
					}
					break;
				case top_to_down_one:
					View1->Image_Move(P2, vertical_move, 0);
					move_flag = 2;
					break;
				case right_to_left:
					if (P2->pic_boarder_x[0][0] > pps[0].x)
					{
						//Change Arrow
						Box1->MyPictureBox_Put_Index(P4, 1);
						P4->Add_Position(P2->pic_boarder_x[0][0] - P3->Get_image().cols, (P2->pic_boarder_y[0][0] + P2->pic_boarder_y[1][1]) / 2);
						View1->Image_Move(P2, horizontal_move, -1);
					}
					else
					{
						//Change Arrow
						Box1->MyPictureBox_Put_Index(P5, 1);
						P5->Add_Position(P4->Get_Pos_X(), P4->Get_Pos_Y());
						move_flag = 3;
					}
					break;
				case top_to_down_two:
					View1->Image_Move(P2, vertical_move, 0);
					Box1->MyPictureBox_Put_Index(P3, 1);
					P3->Add_Position(P2->pic_boarder_x[0][1], (P2->pic_boarder_y[0][1] + P2->pic_boarder_y[1][1]) / 2);
					move_flag = 0;
					break;
				default:
					break;
				}
				if (P2->pic_boarder_x[1][1] > pps[1].x && P2->pic_boarder_y[1][1] > Color_background->Get_image().rows - horizontal_Terminal)
				{
					cv::destroyWindow("絤策家Α");
					//Stop timer
					timer1->Stop();
					//Display
					label1->Text = Convert::ToString((int)count / 60) + "だ" + Convert::ToString((int)count % 60) + "";
					break;
				}
			}

			//Update Trig Region
			pp1.x = P2->pic_boarder_x[0][0];
			pp1.y = P2->pic_boarder_y[0][0];
			//Region
			pp2.x = P2->pic_boarder_x[1][1] - P2->pic_boarder_x[0][0];
			pp2.y = P2->pic_boarder_y[1][1] - P2->pic_boarder_y[0][0];
		}
		cv::waitKey(1000);
	}

private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void textBox1_TextChanged_1(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		label1->Text = "秸俱把计い";
		//textBox1->Text = Convert::ToString(Webcam->Trig_param(Convert::ToInt32(Mywebcam::Trig_Percent)));
		label1->Text = "碞狐";
}
};
}
