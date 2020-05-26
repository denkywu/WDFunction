/**********************************************************************//**
		(1) write cv::Mat To Binary File
		(2)	read Binary File To cv::Mat

@file		cvMatToBinaryFile.cpp
@author		WD
@date		2019/11/14
@brief		write/read cv::Mat format to/from Binary File
**************************************************************************/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>


/**********************************************************************************************************************//**
@brief		д�ļ����� Mat ���������ļ�
@param		const cv::Mat&			[IN] matData�������cv::Mat�е����ݣ���ʽ����
@param		const std::string&		[INOUT] filename��Ҫ������ļ������������򴴽�������Ĩ����ͷд��

@return		bool					[OUT]  1��true���ɹ���0��false��ʧ��

@author		WD
@date		2019/11/14
**************************************************************************************************************************/
bool matToBinaryFile(const cv::Mat& matData, const std::string& filename)
{
	if (matData.empty())
	{
		std::cout << "Error: empty cv::Mat." << std::endl;
		return false;
	}
	const char *filenamechar = filename.c_str();
	FILE *fpw = fopen(filenamechar, "wb");// ���û���򴴽�������������ͷ��ʼд
	if (fpw == NULL)
	{
		std::cout << "Error in opening file." << std::endl;
		fclose(fpw);
		return false;
	}

	int channl = matData.channels();// ͨ��
	int rows = matData.rows;// ����
	int cols = matData.cols;// ����
#if DEBUG_matToBinaryFile == 1
	std::cout << "channl:" << channl << ", rows:" << rows << ", cols:" << cols << std::endl;
	// �� CV_32FC3 Ϊ������Ӧ����ʾ�� channl:3, rows:424, cols:512
#endif// DEBUG

	int typeMat = matData.type();// Mat��������
	int elemSize_Mat = matData.elemSize();// Mat�е���Ԫ����ռ���ֽ�������ͨ�����йأ�
	int elemSize1_Mat = matData.elemSize1();// ��ռ���ֽ�������ͨ�����޹أ��������Ϊ����Ԫ�ص���ͨ����������ռ�ֽ�����
#if DEBUG_matToBinaryFile == 1
	std::cout << "typeMat:" << typeMat << ", elemSize:" << elemSize_Mat << ", elemSize1:" << elemSize1_Mat << std::endl;
	// �� CV_32FC3 Ϊ������Ӧ����ʾ�� typeMat:21, elemSize:12, elemSize1:4
#endif// DEBUG

	fwrite(&channl, sizeof(char), 4, fpw);// �ĸ��ֽڴ�
	fwrite(&rows, sizeof(char), 4, fpw);// �ĸ��ֽڴ�
	fwrite(&cols, sizeof(char), 4, fpw);// �ĸ��ֽڴ�
	fwrite(&typeMat, sizeof(char), 4, fpw);// �ĸ��ֽڴ�
	fwrite(&elemSize_Mat, sizeof(char), 4, fpw);// �ĸ��ֽڴ�
	fwrite(&elemSize1_Mat, sizeof(char), 4, fpw);// �ĸ��ֽڴ�

	char* dp = (char*)matData.data;
	fwrite(dp, sizeof(char)*elemSize_Mat, cols*rows, fpw);// ÿ��Ԫ����ռ�ֽ�Ϊ elemSize_Mat��Ӧ�� = channl*elemSize1_Mat.

	fclose(fpw);
	return true;
}


/**********************************************************************************************************************//**
@brief		���ļ����Ӷ������ļ��� Mat
@param		const std::string&		[IN] filename����Ҫ������ļ�����Ҫ�Ѵ��ڣ�
@param		cv::Mat&				[INOUT] matData�������ݴ����cv::Mat�У���ʽ����

@return		bool					[OUT]  1��true���ɹ���0��false��ʧ��

@author		WD
@date		2019/11/14
**************************************************************************************************************************/
bool binaryFileToMat(const std::string& filename, cv::Mat& matData)
{
	const char *filenamechar = filename.c_str();
	FILE *fpr = fopen(filenamechar, "rb");
	if (fpr == NULL)
	{
		std::cout << "Error in opening file." << std::endl;
		fclose(fpr);
		return false;
	}

	int channl = 0;// ͨ��
	int rows = 0;// ����
	int cols = 0;// ����
	int typeMat = 0;// Mat��������
	int elemSize_Mat = 0;// Mat�е���Ԫ����ռ���ֽ�������ͨ�����йأ�
	int elemSize1_Mat = 0;// ��ռ���ֽ�������ͨ�����޹أ��������Ϊ����Ԫ�ص���ͨ����������ռ�ֽ�����
	int resultCount;// ���fread()����ֵ
	resultCount = fread(&channl, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
	resultCount = fread(&rows, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
	resultCount = fread(&cols, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
	resultCount = fread(&typeMat, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
	resultCount = fread(&elemSize_Mat, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
	resultCount = fread(&elemSize1_Mat, sizeof(char), 4, fpr);
	if(4 != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
#if DEBUG_binaryFileToMat == 1
	std::cout << "channl:"<< channl << ", rows:" << rows << ", cols:" << cols << std::endl;
	std::cout << "typeMat:" << typeMat << ", elemSize:" << elemSize_Mat << ", elemSize1:" << elemSize1_Mat << std::endl;
#endif// DEBUG

	// ��������
	matData = cv::Mat::zeros(rows, cols, typeMat);
	char* pData = (char*)matData.data;
	resultCount = fread(pData, sizeof(char)*elemSize_Mat, cols*rows, fpr);
	if(cols*rows != resultCount)
	{
		std::cout << "Error on fread()!!!" << std::endl;
		fclose(fpr);
		return false;
	}
		
	fclose(fpr);
	return true;
}