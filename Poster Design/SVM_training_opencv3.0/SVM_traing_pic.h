#pragma once
#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <opencv/cv.h>  
#include <iostream> 
#include <ostream>
#include <fstream>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h>
#include <ml.hpp>
#include <ml.h>
#include<time.h> 
#include<atltime.h>
#include <string>
#include<ctime>

using namespace std;
using namespace cv;
using namespace cv::ml;

#define POS 1
#define NEG 0
int tmp1, tmp2;
char * savedata = "C:\\Users\\HASEE\\Desktop\\research\\out.txt";
ofstream out;


void getFiles(string path, vector<string>& files);	//得到测试用的文件目录集 保存在files里
void Getdata(char * PosTrainFilePath, char * NegTrainFilePath, Mat& trainingImages, Mat&trainingLabels);	//返回训练用的文件向量 使用默认的路径
void myget(Mat& trainingImages, vector<int>& trainingLabels, char * filePath, int flag);	//得到训练用的文件 并且打上标签 和Getdata()函数一起使用
void mytrain(Ptr<SVM> & SVM_params, int traintype, Mat& trainingImages, Mat&trainingLabels, string save_name = "svm.xml");	//	训练函数 svm结果保存在save_name文件中
void mytest(Ptr<ml::SVM>& Classify_SVM_params, char * filePath, int filetype, int & saveNumber = tmp1, int &saveResult = tmp2);	//单独测试 以flag标准去测试设定路径下的所有图片
void myset(Ptr<SVM> & SVM_params, double P = 0.1, double G = 3, double eps = 5e-3, double C = 0.1, double R = 2, int max_time = 2000, int Kernel = cv::ml::SVM::LINEAR);	//设定训练参数
void print_time();	//打印时间
void totaltest(Ptr<SVM>& Classify_SVM_params, char * PosFilePath, char *NegFilePath, int & TotalTest = tmp1, int & TotalResult = tmp2);	//总测试 将两类测试项测试 并整合两次测试的结果 并保存
void init();	//初始化
void print_Params(Ptr<SVM> & SVM_params);	//打印参数


void getFiles(string path, vector<string>& files)	//得到测试用的文件目录集 保存在files里
{
	files.clear();
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

void Getdata(char * PosTrainFilePath,char * NegTrainFilePath , Mat& trainingImages,Mat&trainingLabels)	//返回训练用的文件向量 使用默认的路径
{
	//Mat classes;
	//Mat trainingData;
	
	vector<int> classes;
	myget(trainingImages, classes, PosTrainFilePath, POS);
	myget(trainingImages, classes, NegTrainFilePath, NEG);
	//Mat(trainingImages).copyTo(trainingData);
	trainingImages.convertTo(trainingImages, CV_32FC1);
	Mat(classes).copyTo(trainingLabels);

	//Ptr<TrainData>td = TrainData::create(trainingImages, SampleTypes::ROW_SAMPLE, classes);//读取训练样本
	return ;
}

void myget(Mat& trainingImages, vector<int>& trainingLabels, char * filePath, int flag)	//得到训练用的文件 并且打上标签 和Getdata()函数一起使用
{
	//char * filePath = "D:\\data\\1";

	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	cout << "读取训练项类型为" << flag << " 共" << number << "个" << endl;
	for (int i = 0; i < number; i++)
	{
		//cout << i << " " << files[i].c_str() << endl;
		Mat  SrcImage = imread(files[i].c_str());
		//cout << SrcImage;
		//cout << endl <<endl;
		SrcImage = SrcImage.reshape(1,1);// SrcImage.cols*SrcImage.channels()*SrcImage.rows
										 
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(flag);
	}
}

void mytrain(Ptr<SVM> & SVM_params,  int traintype,  Mat& trainingImages, Mat&trainingLabels, string save_name )//	训练函数 svm结果保存在save_name文件中
{
	
	cout << "开始训练";
	if (traintype == 0)
	{
		cout << " autotrain"<<endl;
		SVM_params->trainAuto(trainingImages, SampleTypes::ROW_SAMPLE, trainingLabels);//时间太长
		print_Params(SVM_params);
	}
	else
	{
		cout << " train" << endl;
		SVM_params->train(trainingImages, SampleTypes::ROW_SAMPLE, trainingLabels);
	}
	//保存模型
	//print_Params(SVM_params);

	SVM_params->save(save_name);
	
	cout << "完成训练" << endl;
	
	return;
}
void myset(cv::Ptr<cv::ml::SVM> & SVM_params, double P, double G, double eps, double C, double R, int max_time, int Kernel)	//设定训练参数
{
	//ml::SVM::Params SVM_params;
	out << "调整SVM_params参数如下" << endl;
	out << "P:" << P << " C: " << C;
	out << " G:" << G << " R: " << R << endl;
	out << "终止条件 CV_TERMCRIT_ITER 最大迭代次数:" << max_time << " eps:" << eps << endl;

	SVM_params->setType(cv::ml::SVM::Types::C_SVC); //	SVC是SVM分类器，SVR是SVM回归
	SVM_params->setKernel(Kernel);	//核心函数

	if (Kernel == cv::ml::SVM::RBF)
		out << "核心函数:" << "RBF" << endl;
	else if (Kernel == cv::ml::SVM::LINEAR)
		out << "核心函数:" << "LINEAR" << endl;
	else
		out << "核心函数：其他" << endl;
	out << endl;
	SVM_params->setTermCriteria(cv::TermCriteria(CV_TERMCRIT_ITER, max_time, eps));	//终止条件 type，最大迭代次数，eps
	SVM_params->setP(P);	//P损失函数
	SVM_params->setGamma(G);	//Gamma函数设置(针对多项式/rbf/sigmoid核函数)
	SVM_params->setC(C);	//设置C-SVC, e -SVR和v-SVR的参数(损失函数)(默认1);
	SVM_params->setCoef0(R);	//核函数中的coef0设置(针对多项式/sigmoid核函数)((默认0);
	print_Params(SVM_params);
	cout << 1 << endl;
	return;
}
void print_time()	//打印时间
{
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	out << "time:";
	out << t.tm_mon + 1 << "月" << t.tm_mday << "日" << t.tm_hour << "时" << t.tm_min << "分" << t.tm_sec << "秒" << endl;
	cout << t.tm_mon + 1 << "月" << t.tm_mday << "日" << t.tm_hour << "时" << t.tm_min << "分" << t.tm_sec << "秒" << endl;
	return;
}
void totaltest(Ptr<ml::SVM>& Classify_SVM_params, char * PosFilePath, char *NegFilePath, int & TotalTest, int & TotalResult)	//总测试 将两类测试项测试 并整合两次测试的结果 并保存
{
	cout << "进行总测试：" << endl;
	vector<string>files;
	TotalTest = TotalResult = 0;
	getFiles(PosFilePath, files);	//3 测试1 正面
	int result = 0;
	int number;
	mytest(Classify_SVM_params, PosFilePath, POS, number, result);
	TotalTest += number;
	TotalResult += result;

	mytest(Classify_SVM_params, NegFilePath, NEG, number, result);
	TotalTest += number;
	TotalResult += result;

	cout << "总测试项 correct rate : " << 1.0*TotalResult / TotalTest << "\t correct " << TotalResult << " / " << TotalTest << endl;
	out << "总测试项 correct rate : " << 1.0*TotalResult / TotalTest << "\t correct " << TotalResult << " / " << TotalTest << endl;
	out << endl;

}
void mytest(Ptr<ml::SVM>& Classify_SVM_params, char * filePath, int filetype, int & saveNumber, int &saveResult)	//单独测试 以flag标准去测试设定路径下的所有图片
{
	cout << "进行测试" << " 测试文件位置为：" << filePath << endl;
	out << "进行测试" << " 测试文件位置为：" << filePath << endl;
	cout << "测试目标是";
	out << "测试目标是";
	if (filetype == POS)
	{
		cout << "POS" << endl;
		out << "POS" << endl;
	}
	else
	{
		cout << "NEG" << endl;
		out << "NEG" << endl;
	}
	vector<string> files;//测试图片名称集
	getFiles(filePath, files);	//得到测试图片名称集
	int number = files.size();;//测试总数
	int result = 0;//测试结果
	for (int i = 0; i < number; i++)
	{
		Mat inMat = imread(files[i].c_str());
		//Mat p = inMat;
		Mat p = inMat.reshape(1, 1);
		p.convertTo(p, CV_32FC1);
		int response = (int)Classify_SVM_params->predict(p);
		cout << files[i].c_str() << " ";
		if (response == filetype)
		{
			cout << i + 1 << "\tYES" << endl;
			//myos << i + 1 << "\t" << files[i].c_str() << "\tYes" << endl;
			result++;
		}
		else
		{
			cout << i + 1 << "\tNO" << endl;
			//myos << i + 1 << "\t" << files[i].c_str() << "\tNo" << endl;
		}
	}
	cout << "correct rate :" << 1.0*result / number << " \tcorrect " << result << " / " << number << endl;
	out << "correct rate :" << 1.0*result / number << " \tcorrect " << result << " / " << number << endl;
	saveNumber = number;
	saveResult = result;
	return;
}
void init()	//初始化
{
	out.open(savedata, ios::app);//做记录
	if (!out.is_open())
	{
		cout << "open savedata fail" << endl;
		getchar();
	}
	else
	{
		cout << "本次运行数据将记录在" << savedata << "文件中" << endl;
	}
	print_time();//打印时间
	return;
}
void print_Params(Ptr<SVM> & SVM_params)	//打印参数
{
	float tC = SVM_params->getC();
	float tP = SVM_params->getP();
	float gamma = SVM_params->getGamma();
	cout << "svm_params参数为 C: " << tC << " P：" << tP << " gamma：" << gamma << endl;
	out << "svm_params参数为 C: " << tC << " P：" << tP << " gamma：" << gamma << endl;
	return;
}
/*
void get_0(Mat& trainingImages, vector<int>& trainingLabels)//正面项
{
//char * filePath2 = "D:\\data\\0";

vector<string> files;
getFiles(filePath, files);
int number = files.size();
cout << "1  " << number << endl;
for (int i = 0; i < number; i++)
{
cout << i << " " << files[i].c_str() << endl;
Mat  SrcImage = imread(files[i].c_str());
SrcImage = SrcImage.reshape(1, 1);
trainingImages.push_back(SrcImage);
trainingLabels.push_back(0);
}
}
*/