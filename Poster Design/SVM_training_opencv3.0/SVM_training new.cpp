#include "SVM_traing_pic.h"
#define trains 1
//#define pic 1
#define DoTrichotomy 1

#ifdef pic 
char * PosTrainFilePath ="C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\POS_TRAIN";//正面项
char * NegTrainFilePath ="C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\NEG_TRAIN";//反面项
char * PosTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\POS_TEST";//正面测试项
char * NegTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\NEG_TEST";//反面测试项
#else
char * PosTrainFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\1";	//正面项
char * NegTrainFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\0";//反面项
char * PosTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\1_test";
char * NegTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\0_test";
#endif
void trichotomy(Ptr<SVM> & SVM_params, Mat & trainingImages, Mat &trainingLabels, double left1=0, double right1=10, double left2=0, double right2=10, double tr = 0.2, double eps = 0.01);
int main()
{
	init();
	Mat trainingImages,trainingLabels;
	Getdata(PosTrainFilePath,NegTrainFilePath,trainingImages,trainingLabels);	//获取训练数据
#ifdef trains
	cv::Ptr<cv::ml::SVM>  SVM_params = cv::ml::SVM::create();
	double P = 0.1,G = 0.0769231, eps = 5e-3, C = 0.001, R = 2; int max_time = 2000;
	myset(SVM_params, P, G, eps, C, R);
	mytrain(SVM_params,1 , trainingImages, trainingLabels);	//traintype:1-autotrain , 0-train
#endif

#ifdef DoTrichotomy
	trichotomy(SVM_params, trainingImages, trainingLabels);	//三分调参
#endif
	cout << "正在打开"<<"svm.xml" << endl;
	Ptr<SVM> Classify_SVM_params = Algorithm::load<ml::SVM>("svm.xml");//训练后必须重新打开 否则SVM_params->predict 报错

	print_Params(Classify_SVM_params);

	mytest(Classify_SVM_params, PosTrainFilePath, POS);	//对训练项进行测试 检验
 	mytest(Classify_SVM_params, NegTrainFilePath, NEG);	
		
	totaltest(Classify_SVM_params,PosTestFilePath,NegTestFilePath);//对测试项进行测试
		
	cout << "本次运行的结果已经保存在" << savedata << "文件中" << endl;
	getchar();
	return 0;
}

void trichotomy(Ptr<SVM> & SVM_params, Mat & trainingImages, Mat &trainingLabels, double left1,double right1,double left2,double right2, double tr , double eps )
{
	cout << "开始进行三分" << endl;
	out << "进行三分------------------------------" << endl;
	double left[2] = { left1,left2 }, right[2] = { right1,right2 };
	double mid[2][2];
	while (right[0] - left[0] > eps || right[1] - left[1] > eps)
	{
		cout << left[0]<<" - "<<right[0]<<"\t"<<left[1]<<" - "<<right[1]<< endl;
		for (int i = 0; i < 2; i++)
		{
			mid[i][0] = left[i] * tr + right[i] * (1-tr);
			mid[i][1] = left[i] * (1-tr) + right[i] * tr;
		}
		int re, nu;
		double ss[4];
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				SVM_params->setC(mid[0][j]);
				SVM_params->setGamma(mid[1][i]);
				mytrain(SVM_params, 1, trainingImages, trainingLabels);
				Ptr<SVM> Classify_SVM_params = Algorithm::load<ml::SVM>("svm.xml");//训练后必须重新打开 否则SVM_params->predict 报错
				totaltest(Classify_SVM_params, PosTestFilePath, NegTestFilePath, nu, re);
				ss[i*2+j] = 1.0*re / nu;
			}
		}
		int maxx = 0,cnt=1;
		for (int i = 1; i < 4; i++)
		{
			if (ss[i] > ss[maxx]+0.00005)
			{
				maxx = i;
			}
			else if (ss[maxx] - ss[i] < 0.00005)
			{
				cnt++;
				if (rand() % cnt == 1)
				{
					maxx = i;
				}
			}
		}
		switch (maxx)
		{
		case 0:
			right[0] = mid[0][1];
			right[1] = mid[1][1];
			break;
		case 1:
			right[1] = mid[1][1];
			left[0] = mid[0][0];
			break;
		case 2:
			right[0] = mid[0][1];
			left[1] = mid[1][0];
			break;
		case 3:
			left[0] = mid[0][0];
			left[1] = mid[1][0];
		}
	}
	cout << "三分得到的结果是：" << left[0] << " - " << right[0] << "\t" << left[1] << " - " << right[1] << endl;
	out << "三分得到的结果是：" << left[0] << " - " << right[0]<<"\t" << left[1] << " - " << right[1] << endl;
	out << "三分结束------------------------------" << endl;
}