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

//调参部分
#define trains 1	//def train / not def - not train only predict
#define num 1		//def 使用数字图作为训练样本 / not def 使用海报作为训练样本
//#define loop 1	//def 某参数持续改变 改变一次 出一次结果 / not def 只进行一次结果
#define RBFm 1		//def 使用RBF作为核心函数 / not def 使用LINEAR作为核心函数		(如果其他类型 在101 131 行改)
#define Auto 1	//def 使用 trainAuto() 训练 速度较慢 能自动调参 有交叉验证 / not def 使用train() 训练 速度快，参数固定（自己给的）

//部分参数列表
double P = 0.1;	//P损失函数
double G = 3;	// Gamma函数设置(针对多项式/rbf/sigmoid核函数)
double eps = 5e-3;	//精度 用于结束条件
double C = 0.1;	//设置C-SVC, e -SVR和v-SVR的参数(损失函数)(默认1)
double R = 2;	//核函数中的coef0设置(针对多项式 / sigmoid核函数)((默认0) 


#ifdef num
#else
	#define pic 1
#endif

#ifdef pic 
char * filePath = "C:\\Users\\admin\\Desktop\\research\\Dataset\\new\\traned_1";//正面项
char * filePath2 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\new\\traned_2";//反面项
char * filePath3 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\new\\traned_1_test";//正面测试项
char * filePath4 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\new\\traned_2_test";//反面测试项
#endif
char * savedata = "C:\\Users\\admin\\Desktop\\research\\tmp_out.txt";	//记录每一次调参的结果

#ifdef num

char * filePath = "C:\\Users\\admin\\Desktop\\research\\Dataset\\number\\1";	//正面项
char * filePath2 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\number\\0";	//反面项
char * filePath3 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\number\\1_test";
char * filePath4 = "C:\\Users\\admin\\Desktop\\research\\Dataset\\number\\0_test";
#endif

using namespace std;
using namespace cv;
using namespace cv::ml;
//using namespace ml;
void getFiles(string path, vector<string>& files);
void get_1(Mat& trainingImages, vector<int>& trainingLabels);
void get_0(Mat& trainingImages, vector<int>& trainingLabels);

int main()
{
	struct tm t;
	time_t now;
	ofstream out;
	out.open(savedata, ios::app);
	if (!out.is_open())
	{
		cout << "open savedata fail" << endl;
		getchar();
	}
	
	//getchar();
	//return 0;
#ifdef loop
	for (C = 0.0001; C <= 1; C *= 10)
	{
#endif
		time(&now);
		localtime_s(&t, &now);
		out << "time:";
		out << t.tm_mon + 1 << "月" << t.tm_mday << "日" << t.tm_hour << "时" << t.tm_min << "分" << t.tm_sec << "秒" << endl;
		cout << t.tm_mon + 1 << "月" << t.tm_mday << "日" << t.tm_hour << "时" << t.tm_min << "分" << t.tm_sec << "秒" << endl;
#ifdef trains
		//获取训练数据

		int max_time = 2000;
#ifdef Auto
		out << "Auto\t";
#endif
#ifdef num
		out << "数字";
#endif
#ifdef pic
		out << "图片";
#endif
		out << "训练" << endl;
#ifdef RBFm
		out << "核心函数:" << "RBF" << endl;
#else
		out << "核心函数:" << "LINEAR" << endl;
#endif
		out << "P:" << P << " C: " << C;
		out << " G:" << G << " R: " << R << endl;
		out << "终止条件 CV_TERMCRIT_ITER 最大迭代次数:" << max_time << " eps:" << eps << endl;
		out << endl;

		//ml::SVM::Params SVM_params;
		cv::Ptr<cv::ml::SVM>  SVM_params = cv::ml::SVM::create();
		SVM_params->setType(cv::ml::SVM::Types::C_SVC); //	SVC是SVM分类器，SVR是SVM回归
		
#ifdef RBFm
		SVM_params->setKernel(cv::ml::SVM::RBF);	//核心函数
#else
		SVM_params->setKernel(cv::ml::SVM::LINEAR);	//核心函数
#endif
		SVM_params->setTermCriteria(cv::TermCriteria(CV_TERMCRIT_ITER, max_time, eps));	//终止条件 type，最大迭代次数，eps
		SVM_params->setP(P);	//P损失函数
		SVM_params->setGamma(G);	//Gamma函数设置(针对多项式/rbf/sigmoid核函数)
		SVM_params->setC(C);	//设置C-SVC, e -SVR和v-SVR的参数(损失函数)(默认1);
		SVM_params->setCoef0(R);	//核函数中的coef0设置(针对多项式/sigmoid核函数)((默认0);

		Mat classes;
		Mat trainingData;
		Mat trainingImages;
		vector<int> trainingLabels;
		get_1(trainingImages, trainingLabels);
		get_0(trainingImages, trainingLabels);
		Mat(trainingImages).copyTo(trainingData);
		trainingData.convertTo(trainingData, CV_32FC1);
		Mat(trainingLabels).copyTo(classes);
		
		Ptr<TrainData>td = TrainData::create(trainingData, SampleTypes::ROW_SAMPLE, classes);//读取训练样本
																							 //配置SVM训练器参数

		cout << "开始训练" << endl;
#ifdef Auto
		SVM_params->trainAuto(td);//时间太长
#else
		SVM_params->train(td);
#endif

		//保存模型
		SVM_params->save("svm.xml");
		cout << "完成训练" << endl;

#endif

		int result = 0;
		vector<string> files;

		cout << "正在打开svm" << endl;
		Ptr<ml::SVM> Classify_SVM_params = Algorithm::load<ml::SVM>("svm.xml");
		cout << "打开完成" << endl;
		//必须重新打开 否则SVM_params->predict 报错

		int number;
		result = 0;
		getFiles(filePath2, files);	//1 反面
		number = files.size();
		string outpath = "out1.txt";
		for (int i = 0; i < number; i++)
		{
			Mat inMat = imread(files[i].c_str());
			//Mat p = inMat;
			Mat p = inMat.reshape(1, 1);
			p.convertTo(p, CV_32FC1);
			int response = (int)Classify_SVM_params->predict(p);
			cout << files[i].c_str() << " ";
			if (response == 0)
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
		cout << "反面训练项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		out << "反面训练项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;

		result = 0;
		getFiles(filePath, files);	//2 正面
		number = files.size();
		for (int i = 0; i < number; i++)
		{
			Mat inMat = imread(files[i].c_str());
			//Mat p = inMat;
			Mat p = inMat.reshape(1, 1);
			p.convertTo(p, CV_32FC1);
			int response = (int)Classify_SVM_params->predict(p);
			cout << files[i].c_str() << " ";
			if (response == 0)
			{
				cout << i + 1 << "\tYES" << endl;
				result++;
			}
			else
			{
				cout << i + 1 << "\tNO" << endl;
			}
		}
		cout << "正面训练项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		out << "正面训练项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;

		int totaltest = 0,testcorrct=0;
		getFiles(filePath3, files);	//3 测试1 正面
		result = 0;
		number = files.size();
		totaltest += number;
		for (int i = 0; i < number; i++)
		{
			Mat inMat = imread(files[i].c_str());
			//Mat p = inMat;
			Mat p = inMat.reshape(1, 1);
			p.convertTo(p, CV_32FC1);
			int response = (int)Classify_SVM_params->predict(p);
			cout << files[i].c_str() << " ";
			if (response == 0)
			{
				cout << i + 1 << "\tYES" << endl;
				result++;
			}
			else
			{
				cout << i + 1 << "\tNO" << endl;
			}
		}

		cout << "正面测试项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		out << "正面测试项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		testcorrct += result;
		getFiles(filePath4, files);	//4 测试2 反面
		result = 0;
		number = files.size();
		for (int i = 0; i < number; i++)
		{
			Mat inMat = imread(files[i].c_str());
			//Mat p = inMat;
			Mat p = inMat.reshape(1, 1);
			p.convertTo(p, CV_32FC1);
			int response = (int)Classify_SVM_params->predict(p);
			cout << files[i].c_str() << " ";
			if (response == 0)
			{
				cout << i + 1 << "\tYES" << endl;
				result++;
			}
			else
			{
				cout << i + 1 << "\tNO" << endl;
			}
		}
		cout << "反面测试项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		out << "反面测试项 success rate :" << 1.0*result / number << " \tsuccess " << result << " / " << number << endl;
		testcorrct += number - result;
		totaltest += number;
		cout << "总测试项 correct rate : " << 1.0*testcorrct / totaltest << "\t correct " << testcorrct << " / " << totaltest << endl;
		out << "总测试项 correct rate : " << 1.0*testcorrct / totaltest << "\t correct " << testcorrct << " / " << totaltest << endl;
		out << endl;
		cout << result << endl;
#ifdef loop
	}
#endif
	getchar();
	return 0;
}

void getFiles(string path, vector<string>& files)
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

void get_1(Mat& trainingImages, vector<int>& trainingLabels)//反面项
{
	//char * filePath = "D:\\data\\1";

	vector<string> files;
	getFiles(filePath2, files);
	int number = files.size();
	cout << "2  读取反面数据" << number << endl;
	for (int i = 0; i < number; i++)
	{
		//cout << i << " " << files[i].c_str() << endl;
		Mat  SrcImage = imread(files[i].c_str());
		//cout << SrcImage;
		//cout << endl <<endl;
		SrcImage = SrcImage.reshape(1,1);// SrcImage.cols*SrcImage.channels()*SrcImage.rows
		//Mat.reshape(通道数,行数) 转换矩阵的行列
		//cout << SrcImage;
		//getchar();
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(1);
	}
}
void get_0(Mat& trainingImages, vector<int>& trainingLabels)//正面项
{
	//char * filePath2 = "D:\\data\\0";

	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	cout << "1  读取正面数据" << number << endl;
	for (int i = 0; i < number; i++)
	{
		//cout << i << " " << files[i].c_str() << endl;
		Mat  SrcImage = imread(files[i].c_str());
		SrcImage = SrcImage.reshape(1, 1);
		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(0);
	}
}
