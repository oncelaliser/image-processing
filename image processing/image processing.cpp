#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "PpmIO.h"
#include "PgmIO.h"

using namespace std;

static void filterGauss3x1(unsigned char* pImgDst, const unsigned char* pImgSrc, const int width, const int height) {
	for (int y = 0; y < height; ++y) {
		unsigned char* pDst = pImgDst + y * width + 1;
		const unsigned char* pSrc = pImgSrc + y * width;
		for (int x = 1; x < width - 1; ++x) {
			const unsigned int sum = (int)pSrc[0] + 2 * (int)pSrc[1] + (int)pSrc[2];
			*pDst = sum / 4;
			++pDst;
			++pSrc;
		}
	}
};

static void filterGauss1x3(unsigned char* pImgDst, const unsigned char* pImgSrc, const int width,
	const int height) {
	for (int y = 1; y < height - 1; y++) {
		unsigned char* pDst = pImgDst + y * width;
		const unsigned char* pSrc = pImgSrc + y * width;

		for (int x = 0; x < width; ++x) {
			const unsigned int sum = (int)pSrc[-width] + 2 * (int)pSrc[0] + (int)pSrc[width];
			*pDst = sum / 4;
			++pDst;
			++pSrc;
		}
	}
};
//Filter image with 3*3 Gaussian Kernel (seperated in 3*1 and 1*3)
static void filterGaussian3x3(unsigned char* pImg, const int width, const int height) {
	unsigned char* pFlty = new unsigned char[width * height];
	memcpy(pFlty, pImg, width * height);

	filterGauss3x1(pFlty, pImg, width, height); //horizontal pass
	filterGauss1x3(pImg, pFlty, width, height); //vertical pass

	// copying the border
	memcpy(pImg, pFlty, width); // top row
	memcpy(pImg + width * (height - 1), pFlty + width * (height - 1), width); //bottom row
	delete[] pFlty;
};



int main() {
	//READING THE IMAGE
	//the position of the image in memory it is stored in a pointer that refers to the first pixel
	unsigned char* pImage = 0;
	int height;
	int width;
	bool readOk = readPGM("saturn.ascii.pgm", &pImage, width, height);

	if (false == readOk) {
		cout << "Reading image failed \n";
		cout << "Press a random key to exit \n";
		return -1;
	}
	//SCALE IMAGE
	const int widthscl = width / 2;
	const int heightscl = height / 2;

	unsigned char* pScaledImage = new unsigned char[widthscl * heightscl];

	for (int y = 0; y < heightscl; y++) {
		for (int x = 0; x < widthscl; x++) {
			pScaledImage[x + y * widthscl] = pImage[2 * x + 2 * y * width];
		}
	}
	writePGM("half.pgm", pScaledImage, widthscl, heightscl);

	//FILTER IMAGE WITH 3*3 GAUSSIAN KERNEL
	filterGaussian3x3(pScaledImage, widthscl, heightscl);

	writePGM("halfFiltered.pgm", pScaledImage, widthscl, heightscl);


	unsigned int histogram[256];
	memset(histogram, 0, 256 * sizeof(unsigned int));

	for (int i = 0; i < widthscl * heightscl; i++) {
		++histogram[pScaledImage[i]];
	}
	const float cutOffPercentage = 0.05;
	unsigned char lowerBound, upperBound;
	unsigned int histAccu = 0;
	const unsigned int lowerPercentile = cutOffPercentage * heightscl * widthscl;
	const unsigned int upperPercentile = (1 - cutOffPercentage) * widthscl * heightscl;

	for (int h = 0; h < 256; h++) {
		histAccu += histogram[h];
		if (histAccu <= lowerPercentile) {
			lowerBound = h;
			continue;
		}
		if (histAccu >= upperPercentile) {
			upperBound = h;
			break;
		}
	}
	const float histScale = 255 / (upperBound - lowerBound);
	for (int i = 0; i < widthscl * heightscl; i++) {
		const int newVal = histScale * ((int)pScaledImage[i] - lowerBound);
		pScaledImage[i] = min<int>(255, max<int>(0, newVal));
	}
	writePGM("histogram.pgm", pScaledImage, widthscl, heightscl);
	// CALCULATE IMAGE ENERGY BY GRADIENTS
	unsigned char* energy = new unsigned char [widthscl * heightscl];
	memset(energy, 0, widthscl * heightscl);

	for (int y = 1; y < heightscl -1; ++y) {
		const int rowOffset = y * widthscl;
		for (int x = 1; x < widthscl - 1; ++x) {
			const int gradx = pScaledImage[rowOffset + x + 1] - pScaledImage[rowOffset + x - 1];
			const int grady = pScaledImage[rowOffset + x + widthscl] - pScaledImage[rowOffset + x - widthscl];
			energy[rowOffset + x] = sqrt((float)gradx * gradx + grady * grady) / sqrt(2.f);
		}
	}
	writePGM("energy.pgm", energy, widthscl, heightscl);
	// SEGMENT HIGH ENERGY AREAS BY THRESHOLDING
	for (int i = 0; i < widthscl * heightscl; ++i) {
		if (energy[i] > 30) {
			energy[i] = 255;
		}
		else {
			energy[i] = 0;
		}
	}
	writePGM("energythresh.pgm", energy, widthscl, heightscl);

	delete[] energy;
	delete[] pScaledImage;
	delete[] pImage;

	return 0;
}
