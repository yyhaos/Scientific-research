#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <opencv/cv.h>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h>

using namespace std;
using namespace cv;

void getFiles(string path, vector<string>& files);

int main()
{
	int result = 0;
	char * filePath = "D:\\data\\test_image";
	vector<string> files;
	
	getFiles(filePath, files);
	int number = files.size();
	//cout << number << endl;
	Ptr<ml::SVM> SVM_params = Algorithm::load<ml::SVM>("svm.xml");
	//SVM_params->clear();
	/*
	string modelpath = "svm.xml";
	FileStorage svm_fs(modelpath, FileStorage::READ);
	if (svm_fs.isOpened())
	{
		cout << "svm.xml loaded" << endl;
		SVM_params->load(modelpath.c_str());
	}
	*/
	for (int i = 0; i < number; i++)
	{
		Mat inMat = imread(files[i].c_str());
		Mat p = inMat.reshape(1, 1);
		p.convertTo(p, CV_32FC1);
		int response = (int)SVM_params->predict(p);
		if (response == 0)
		{
			cout << i+1 << "\tYES" << endl;
			result++;
		}
		else
			cout << i+1 << "\tNO" << endl;
	}
	cout << result << endl;
	getchar();
	return  0;
}
void getFiles(string path, vector<string>& files)
{
	long long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
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
				cout << fileinfo.name << endl;
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
