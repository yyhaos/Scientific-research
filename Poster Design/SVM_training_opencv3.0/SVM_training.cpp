#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <opencv/cv.h>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h>
#include <ml.hpp>
#include <ml.h>

using namespace std;
using namespace cv;

//using namespace ml;
void getFiles(string path, vector<string>& files);
void get_1(Mat& trainingImages, vector<int>& trainingLabels);
void get_0(Mat& trainingImages, vector<int>& trainingLabels);

int main()
{
	//获取训练数据
	Mat classes;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;
	get_1(trainingImages, trainingLabels);
	get_0(trainingImages, trainingLabels);
	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);
	//配置SVM训练器参数
	
	//ml::SVM::Params SVM_params;
	cv::Ptr<cv::ml::SVM>  SVM_params = cv::ml::SVM::create();
	SVM_params->setType(cv::ml::SVM::Types::C_SVC);
	SVM_params->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
	SVM_params->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

	//SVM_params.svm_type = ml::SVM::C_SVC;
	//SVM_params.kernelType = ml::SVM::POLY;
	/*SVM_params.degree = 0;
	SVM_params.gamma = 1;
	SVM_params.coef0 = 0;
	SVM_params.C = 1;
	SVM_params.nu = 0;
	SVM_params.p = 0;
	SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
	*/
	//训练
	//CvSVM svm;
	SVM_params->train(trainingData, cv::ml::SampleTypes::ROW_SAMPLE, classes);
	//SVM_params->train();
	//保存模型
	SVM_params->save("svm.xml");
	cout << "训练好了！！！" << endl;
	getchar();
	return 0;
}
void getFiles(string path, vector<string>& files)
{
	long long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	cout << p.assign(path).append("\\*").c_str() << endl;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}
void get_1(Mat& trainingImages, vector<int>& trainingLabels)
{
	char * filePath = "D:\\data\\1";
	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	cout << "1  "<<number << endl;
	for (int i = 0; i < number; i++)
	{
		Mat  SrcImage = imread(files[i].c_str());
		SrcImage = SrcImage.reshape(1, 1);
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(1);
	}
}
void get_0(Mat& trainingImages, vector<int>& trainingLabels)
{
	char * filePath = "D:\\data\\0";
	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	
	for (int i = 0; i < number; i++)
	{
		Mat  SrcImage = imread(files[i].c_str());
		SrcImage = SrcImage.reshape(1, 1);
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(0);
	}
}
