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


void getFiles(string path, vector<string>& files);	//�õ������õ��ļ�Ŀ¼�� ������files��
void Getdata(char * PosTrainFilePath, char * NegTrainFilePath, Mat& trainingImages, Mat&trainingLabels);	//����ѵ���õ��ļ����� ʹ��Ĭ�ϵ�·��
void myget(Mat& trainingImages, vector<int>& trainingLabels, char * filePath, int flag);	//�õ�ѵ���õ��ļ� ���Ҵ��ϱ�ǩ ��Getdata()����һ��ʹ��
void mytrain(Ptr<SVM> & SVM_params, int traintype, Mat& trainingImages, Mat&trainingLabels, string save_name = "svm.xml");	//	ѵ������ svm���������save_name�ļ���
void mytest(Ptr<ml::SVM>& Classify_SVM_params, char * filePath, int filetype, int & saveNumber = tmp1, int &saveResult = tmp2);	//�������� ��flag��׼ȥ�����趨·���µ�����ͼƬ
void myset(Ptr<SVM> & SVM_params, double P = 0.1, double G = 3, double eps = 5e-3, double C = 0.1, double R = 2, int max_time = 2000, int Kernel = cv::ml::SVM::LINEAR);	//�趨ѵ������
void print_time();	//��ӡʱ��
void totaltest(Ptr<SVM>& Classify_SVM_params, char * PosFilePath, char *NegFilePath, int & TotalTest = tmp1, int & TotalResult = tmp2);	//�ܲ��� �������������� ���������β��ԵĽ�� ������
void init();	//��ʼ��
void print_Params(Ptr<SVM> & SVM_params);	//��ӡ����


void getFiles(string path, vector<string>& files)	//�õ������õ��ļ�Ŀ¼�� ������files��
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

void Getdata(char * PosTrainFilePath,char * NegTrainFilePath , Mat& trainingImages,Mat&trainingLabels)	//����ѵ���õ��ļ����� ʹ��Ĭ�ϵ�·��
{
	//Mat classes;
	//Mat trainingData;
	
	vector<int> classes;
	myget(trainingImages, classes, PosTrainFilePath, POS);
	myget(trainingImages, classes, NegTrainFilePath, NEG);
	//Mat(trainingImages).copyTo(trainingData);
	trainingImages.convertTo(trainingImages, CV_32FC1);
	Mat(classes).copyTo(trainingLabels);

	//Ptr<TrainData>td = TrainData::create(trainingImages, SampleTypes::ROW_SAMPLE, classes);//��ȡѵ������
	return ;
}

void myget(Mat& trainingImages, vector<int>& trainingLabels, char * filePath, int flag)	//�õ�ѵ���õ��ļ� ���Ҵ��ϱ�ǩ ��Getdata()����һ��ʹ��
{
	//char * filePath = "D:\\data\\1";

	vector<string> files;
	getFiles(filePath, files);
	int number = files.size();
	cout << "��ȡѵ��������Ϊ" << flag << " ��" << number << "��" << endl;
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

void mytrain(Ptr<SVM> & SVM_params,  int traintype,  Mat& trainingImages, Mat&trainingLabels, string save_name )//	ѵ������ svm���������save_name�ļ���
{
	
	cout << "��ʼѵ��";
	if (traintype == 0)
	{
		cout << " autotrain"<<endl;
		SVM_params->trainAuto(trainingImages, SampleTypes::ROW_SAMPLE, trainingLabels);//ʱ��̫��
		print_Params(SVM_params);
	}
	else
	{
		cout << " train" << endl;
		SVM_params->train(trainingImages, SampleTypes::ROW_SAMPLE, trainingLabels);
	}
	//����ģ��
	//print_Params(SVM_params);

	SVM_params->save(save_name);
	
	cout << "���ѵ��" << endl;
	
	return;
}
void myset(cv::Ptr<cv::ml::SVM> & SVM_params, double P, double G, double eps, double C, double R, int max_time, int Kernel)	//�趨ѵ������
{
	//ml::SVM::Params SVM_params;
	out << "����SVM_params��������" << endl;
	out << "P:" << P << " C: " << C;
	out << " G:" << G << " R: " << R << endl;
	out << "��ֹ���� CV_TERMCRIT_ITER ����������:" << max_time << " eps:" << eps << endl;

	SVM_params->setType(cv::ml::SVM::Types::C_SVC); //	SVC��SVM��������SVR��SVM�ع�
	SVM_params->setKernel(Kernel);	//���ĺ���

	if (Kernel == cv::ml::SVM::RBF)
		out << "���ĺ���:" << "RBF" << endl;
	else if (Kernel == cv::ml::SVM::LINEAR)
		out << "���ĺ���:" << "LINEAR" << endl;
	else
		out << "���ĺ���������" << endl;
	out << endl;
	SVM_params->setTermCriteria(cv::TermCriteria(CV_TERMCRIT_ITER, max_time, eps));	//��ֹ���� type��������������eps
	SVM_params->setP(P);	//P��ʧ����
	SVM_params->setGamma(G);	//Gamma��������(��Զ���ʽ/rbf/sigmoid�˺���)
	SVM_params->setC(C);	//����C-SVC, e -SVR��v-SVR�Ĳ���(��ʧ����)(Ĭ��1);
	SVM_params->setCoef0(R);	//�˺����е�coef0����(��Զ���ʽ/sigmoid�˺���)((Ĭ��0);
	print_Params(SVM_params);
	cout << 1 << endl;
	return;
}
void print_time()	//��ӡʱ��
{
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	out << "time:";
	out << t.tm_mon + 1 << "��" << t.tm_mday << "��" << t.tm_hour << "ʱ" << t.tm_min << "��" << t.tm_sec << "��" << endl;
	cout << t.tm_mon + 1 << "��" << t.tm_mday << "��" << t.tm_hour << "ʱ" << t.tm_min << "��" << t.tm_sec << "��" << endl;
	return;
}
void totaltest(Ptr<ml::SVM>& Classify_SVM_params, char * PosFilePath, char *NegFilePath, int & TotalTest, int & TotalResult)	//�ܲ��� �������������� ���������β��ԵĽ�� ������
{
	cout << "�����ܲ��ԣ�" << endl;
	vector<string>files;
	TotalTest = TotalResult = 0;
	getFiles(PosFilePath, files);	//3 ����1 ����
	int result = 0;
	int number;
	mytest(Classify_SVM_params, PosFilePath, POS, number, result);
	TotalTest += number;
	TotalResult += result;

	mytest(Classify_SVM_params, NegFilePath, NEG, number, result);
	TotalTest += number;
	TotalResult += result;

	cout << "�ܲ����� correct rate : " << 1.0*TotalResult / TotalTest << "\t correct " << TotalResult << " / " << TotalTest << endl;
	out << "�ܲ����� correct rate : " << 1.0*TotalResult / TotalTest << "\t correct " << TotalResult << " / " << TotalTest << endl;
	out << endl;

}
void mytest(Ptr<ml::SVM>& Classify_SVM_params, char * filePath, int filetype, int & saveNumber, int &saveResult)	//�������� ��flag��׼ȥ�����趨·���µ�����ͼƬ
{
	cout << "���в���" << " �����ļ�λ��Ϊ��" << filePath << endl;
	out << "���в���" << " �����ļ�λ��Ϊ��" << filePath << endl;
	cout << "����Ŀ����";
	out << "����Ŀ����";
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
	vector<string> files;//����ͼƬ���Ƽ�
	getFiles(filePath, files);	//�õ�����ͼƬ���Ƽ�
	int number = files.size();;//��������
	int result = 0;//���Խ��
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
void init()	//��ʼ��
{
	out.open(savedata, ios::app);//����¼
	if (!out.is_open())
	{
		cout << "open savedata fail" << endl;
		getchar();
	}
	else
	{
		cout << "�����������ݽ���¼��" << savedata << "�ļ���" << endl;
	}
	print_time();//��ӡʱ��
	return;
}
void print_Params(Ptr<SVM> & SVM_params)	//��ӡ����
{
	float tC = SVM_params->getC();
	float tP = SVM_params->getP();
	float gamma = SVM_params->getGamma();
	cout << "svm_params����Ϊ C: " << tC << " P��" << tP << " gamma��" << gamma << endl;
	out << "svm_params����Ϊ C: " << tC << " P��" << tP << " gamma��" << gamma << endl;
	return;
}
/*
void get_0(Mat& trainingImages, vector<int>& trainingLabels)//������
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