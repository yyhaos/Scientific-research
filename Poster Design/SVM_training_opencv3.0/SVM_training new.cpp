#include "SVM_traing_pic.h"
#define trains 1
//#define pic 1
#define DoTrichotomy 1

#ifdef pic 
char * PosTrainFilePath ="C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\POS_TRAIN";//������
char * NegTrainFilePath ="C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\NEG_TRAIN";//������
char * PosTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\POS_TEST";//���������
char * NegTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\2.26\\NEG_TEST";//���������
#else
char * PosTrainFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\1";	//������
char * NegTrainFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\0";//������
char * PosTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\1_test";
char * NegTestFilePath = "C:\\Users\\HASEE\\Desktop\\research\\Dataset\\number\\0_test";
#endif
void trichotomy(Ptr<SVM> & SVM_params, Mat & trainingImages, Mat &trainingLabels, double left1=0, double right1=10, double left2=0, double right2=10, double tr = 0.2, double eps = 0.01);
int main()
{
	init();
	Mat trainingImages,trainingLabels;
	Getdata(PosTrainFilePath,NegTrainFilePath,trainingImages,trainingLabels);	//��ȡѵ������
#ifdef trains
	cv::Ptr<cv::ml::SVM>  SVM_params = cv::ml::SVM::create();
	double P = 0.1,G = 0.0769231, eps = 5e-3, C = 0.001, R = 2; int max_time = 2000;
	myset(SVM_params, P, G, eps, C, R);
	mytrain(SVM_params,1 , trainingImages, trainingLabels);	//traintype:1-autotrain , 0-train
#endif

#ifdef DoTrichotomy
	trichotomy(SVM_params, trainingImages, trainingLabels);	//���ֵ���
#endif
	cout << "���ڴ�"<<"svm.xml" << endl;
	Ptr<SVM> Classify_SVM_params = Algorithm::load<ml::SVM>("svm.xml");//ѵ����������´� ����SVM_params->predict ����

	print_Params(Classify_SVM_params);

	mytest(Classify_SVM_params, PosTrainFilePath, POS);	//��ѵ������в��� ����
 	mytest(Classify_SVM_params, NegTrainFilePath, NEG);	
		
	totaltest(Classify_SVM_params,PosTestFilePath,NegTestFilePath);//�Բ�������в���
		
	cout << "�������еĽ���Ѿ�������" << savedata << "�ļ���" << endl;
	getchar();
	return 0;
}

void trichotomy(Ptr<SVM> & SVM_params, Mat & trainingImages, Mat &trainingLabels, double left1,double right1,double left2,double right2, double tr , double eps )
{
	cout << "��ʼ��������" << endl;
	out << "��������------------------------------" << endl;
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
				Ptr<SVM> Classify_SVM_params = Algorithm::load<ml::SVM>("svm.xml");//ѵ����������´� ����SVM_params->predict ����
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
	cout << "���ֵõ��Ľ���ǣ�" << left[0] << " - " << right[0] << "\t" << left[1] << " - " << right[1] << endl;
	out << "���ֵõ��Ľ���ǣ�" << left[0] << " - " << right[0]<<"\t" << left[1] << " - " << right[1] << endl;
	out << "���ֽ���------------------------------" << endl;
}