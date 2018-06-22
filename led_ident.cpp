#define _CRT_SECURE_NO_WARNINGS

#include "led_ident.h"

/***�w��w���L�B�z�᪺7�q��ܾ��S�ʤ��R�Ʀr***/
char* ocr_num(int x, char *y_1, char *y_3) {
	switch (x)
	{
	case 0:
		return "1";
		break;
	case 1:
		if (strcmp(y_1, "11") == 0) return "4";
		else if (strcmp(y_1, "01") == 0) return "7";
		else return "-";
		break;
	case 2:
		if (strcmp(y_1, "11") == 0)return"0";
		else if (strcmp(y_1, "01") == 0)return "7";
		else if (strcmp(y_1, "10") == 0)return "1";
		break;
	case 3:
		if (strcmp(y_1, "01") == 0) {
			if (strcmp(y_3, "10") == 0)return "2";
			else if (strcmp(y_3, "01") == 0)return "3";
		}
		else if (strcmp(y_1, "10") == 0) {
			if (strcmp(y_3, "01") == 0) return "5";
			else if (strcmp(y_3, "11") == 0)return "6";
		}
		else if (strcmp(y_1, "11") == 0) {
			if (strcmp(y_3, "11") == 0)return "8";
			else if (strcmp(y_3, "01") == 0)return "9";
		}
		break;
	default:
		return "";
		break;
	}
	return "";
}

/*********��O���X7�q��ܾ��S��**********/
char* ident(Mat img,int warp) {

	//cvtColor(img, img, CV_BGR2GRAY);
	Mat binary;
	//threshold(img, binary, 10, 255, THRESH_BINARY | THRESH_OTSU);
	img.copyTo(binary);
	/******�z���ܴ�***/
	cv::Point2f pts2[] = { cv::Point2f(0,0),cv::Point2f(0 ,binary.rows),cv::Point2f(binary.cols,binary.rows),cv::Point2f(binary.cols,0) };
	cv::Point2f pts1[] = { cv::Point2f(warp ,0),cv::Point2f(-warp ,binary.rows),cv::Point2f(binary.cols - warp  ,binary.rows),cv::Point2f(binary.cols + warp,0) };
	// �z���ܴ���C�p��
	cv::Mat perspective_matrix = cv::getPerspectiveTransform(pts1, pts2);
	cv::Mat dst_img;
	// �ܴ�
	cv::warpPerspective(binary, binary, perspective_matrix, binary.size(), cv::INTER_LINEAR);

	
	if (img.rows / img.cols >= 2.0) {
		//printf("1");
		return "1";
	}

	int x = 0;							//�����q�L
	char y_1[3] = "", y_3[3] = "";		//1/4  3/4  �����q�L

	bool vertical = false;
	bool level_1 = false, level_3 = false, level_ck_1 = false, level_ck_3 = false;
	for (int row = 0; row < binary.rows; row++) {					//��������
		if (binary.at<uchar>(row, binary.cols / 2) == 255 && !vertical) {
			x++;
			vertical = true;
		}
		else if (binary.at<uchar>(row, binary.cols / 2) == 0 && vertical) vertical = false;
	}

	for (int col = 0; col < binary.cols; col++) {
		if (strlen(y_1) == 2 && strlen(y_3) == 2)break;
		if (col >= binary.cols / 2) {
			if (strcmp(y_1, "") == 0) strcat(y_1, "0");
			if (strcmp(y_3, "") == 0) strcat(y_3, "0");
			if (col == binary.cols - 1) {
				if (strlen(y_1) == 1) strcat(y_1, "0");
				if (strlen(y_3) == 1) strcat(y_3, "0");
			}
		}
		if (binary.at<uchar>(binary.rows / 4, col) == 255 && !level_1) {		//����1/4
			strcat(y_1, "1");
			level_1 = true;
		}
		else if (binary.at<uchar>(binary.rows / 4, col) == 0 && level_1) level_1 = false;

		if (binary.at<uchar>(binary.rows / 4 * 3, col) == 255 && !level_3) {	//����3/4
			strcat(y_3, "1");
			level_3 = true;
		}
		else if (binary.at<uchar>(binary.rows / 4 * 3, col) == 0 && level_3) level_3 = false;
	}

	/*printf("%d ", x);
	printf("%s ", y_1);
	printf("%s\n", y_3);*/
	char *ch = ocr_num(x, y_1, y_3);
	return ch;
}