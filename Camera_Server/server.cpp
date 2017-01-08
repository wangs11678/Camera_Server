#include <stdio.h>
#include <Winsock2.h>
#include <opencv/cv.h>  
#include <opencv/cxcore.h>  
#include <opencv/highgui.h>  
#pragma comment(lib, "ws2_32.lib")  
using namespace cv;

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	const int blocksize=28880;
	//������ṹ  
	struct recvbuf //����ʽ  
	{  
		char buf[blocksize]; //������ݵı���  
		int flag; //��־  
	};  
	struct recvbuf data;  
	wVersionRequested = MAKEWORD(2, 0);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0){
		return;
	}

	if(LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0){
		WSACleanup();
		return;
	}
	SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
	if(INVALID_SOCKET == sockSrv){
		printf("Socket ����ʧ�ܣ�Exit!");
		return;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);

	if(-1==bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR))){
		printf("Server bind error!\n");
		return;
	}

	//listen(sockSrv,5);

	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);

	//����IplImageָ��  
	IplImage* pFrame = NULL;  
	//pFrame->imageSize=
	//��ȡ����ͷ  
	//CvCapture* pCapture = cvCreateCameraCapture(0);  

	//��������  
	cvNamedWindow("video", 1);  

	//��ʾ����  
	/* while(1)  
	{  
		pFrame=cvQueryFrame( pCapture);  
		if(!pFrame)break;  
		cvShowImage("video",pFrame);  
		char c=cvWaitKey(33);  
		if(c==27)break;  
	}  
	*/
	//SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);

	int nRecvBuf=1024*1024*10;//���ջ���10M  
	setsockopt(sockSrv, SOL_SOCKET, SO_RCVBUF,(const char*)&nRecvBuf, sizeof(int));
	int COUNT=0;
	char img[blocksize*32]={0};
	int n;

	//Mat L();

	//long nRows = .rows * M.channels();  //channels()Ҳ��Mat��һ�����õĺ��������ڻ�ȡͨ������RGB=3���Ҷ�=1��
	//long nCols = M.cols;
	//uchar *p = M.data;  //����ָ��
	//printf("sadfsadf");
	while(1)  
	{     
		//memset(data.buf, 0, sizeof(char)*blocksize);
		for( int i=0;i<32;i++)           
		{  
			//Sleep(1000);
			n=recvfrom(sockSrv,(char *)(&data),blocksize+4,0,(SOCKADDR*)&addrClient,&len);  
			//Sleep(1000);
			printf("%d\n",n);
			COUNT=COUNT+data.flag;  
			for(int k=0;k<blocksize;k++)  
			{
				img[i*blocksize+k]=data.buf[k];  
			}  

			if(data.flag==2)  //data.flag==2��һ֡�е����һ�����ݿ�  
			{   
				if(COUNT==33)  
				{
					//Mat L(480,640, CV_8UC3, img);

					//if(M.isContinuous())
					//{
					//	  nCols *= nRows;
					//    for (long i=0; i < nCols; i++) {
					//		  *p++ = ; // ���ظ�ֵ���ȡ����
					//    }   
					//}

					//namedWindow("Lena.jpg", CV_WINDOW_AUTOSIZE); 
					//imshow("Lena.jpg", L);
					//waitKey(33);


					pFrame = cvCreateImageHeader(cvSize(640,480),IPL_DEPTH_8U,3);  

					cvSetData(pFrame,img,640*3);//���յ������ݽ���һ֡ͼ��  
					cvShowImage("video",pFrame);
					char c=cvWaitKey(1000/30);  
					if(c==27)break; 
					printf("frame!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}  
				else   
				{  
					COUNT=0;  
					i=-1;  
				}  
				// j++;  
			}  

		}      
	}  

	/*
	while(1)
	{
	recv(sockConn,pFrame->imageData,pFrame->imageSize,0);
	if(!pFrame)break;  
	cvShowImage("video",pFrame);
	}*/
	//closesocket(sockConn);
	cvDestroyWindow("video");  
}
