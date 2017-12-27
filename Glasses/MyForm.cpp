#include "MyForm.h"

using namespace Glasses;
int main(int argc, char *argv[])
{
	//OpenCL Setting
	ocl::setUseOpenCL(true);
	cv::ocl::Context context;
	context.device(0);
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm());
	return 0;
}
