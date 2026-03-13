#pragma warning(disable : 4996)
#include "PgmIO.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

static void skipwhitespaceandcomments(FILE* f) {
	int c;
	while ((c = fgetc(f)) != EOF) {
		if (c == '#') {
			while ((c = fgetc(f)) != EOF && c != '\n');
		}
		else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
			ungetc(c, f);
			break;
		}
	}
}

bool readPGM(const char* fileName, unsigned char** ppData, int& width, int& height) {
	FILE* f = nullptr;
	fopen_s(&f, fileName, "rb");
	if (!f) {
		cerr << "readPGM: Cannot open file" << fileName << endl;
		return false;
	}
	// Read magic number (P2 = ASCII, P5 = Binary)
	char magic[3] = {};
	if (fscanf(f, "%2s", magic) != 1) {
		cerr << "readPGM: Failed to read magic number " << endl;
		fclose(f);
		return false;
	}
	bool isAscii;
	if (strcmp(magic, "P2") == 0)  isAscii = true;
	else if (strcmp(magic, "P5") == 0) isAscii = false;
	else {
		cerr << "readPGM: Unsupported format" << magic << endl;
		fclose(f);
		return false;
	}
	// Read width, height, max gray value (skip comments between values)
	int maxval;
	skipwhitespaceandcomments(f);
	fscanf(f, "%d", &width);
	skipwhitespaceandcomments(f);
	fscanf(f, "%d", &height);
	skipwhitespaceandcomments(f);
	fscanf(f, "%d", &maxval);
	if (width <= 0 || height <= 0 || maxval <= 0 || maxval > 255) {
		cerr << "readPGM: Invalid header values" << endl;
		fclose(f);
		return false;
	}
	// Allocate output buffer
	const int numpixels = width * height;
	*ppData = new unsigned char[numpixels];

	if (isAscii) {
		for (int i = 0; i < numpixels; i++) {
			int val;
			if (fscanf(f, "%d", &val) != 1) {
				cerr << "readPGM: Unexpected end of ASCII pixel" << i << endl;
				delete[] * ppData;
				*ppData = nullptr;
				fclose(f);
				return false;
			}
			(*ppData)[i] = (unsigned char)val;
		}
	}
	else {
		fgetc(f);
		size_t bytesRead = fread(*ppData, 1, numpixels, f);
		if ((int)bytesRead != numpixels) {
			cerr << "readPGM: Expected " << numpixels << "bytes got " << bytesRead << endl;
			delete[] * ppData;
			*ppData = nullptr;
			fclose(f);
			return false;
		}
	}
	fclose(f);
	return true;
}
	// writePGM: always writes P5 (binary) format for efficiency
	bool writePGM(const char* fileName, const unsigned char* pData, int width, int height){
		FILE* f = nullptr;
		fopen_s(&f, fileName, "wb");
		if (!f) {
			cerr << "writePGM: Cannot open file for writing: " << fileName << endl;
			return false;
		}
		//writePGM binary header
		fprintf(f, "P5\n%d %d\n255\n", width, height);

		//write raw pixel data
		size_t bytesWritten = fwrite(pData, 1, width * height, f);
		fclose(f);

		if ((int)bytesWritten != width * height) {
			cerr << "writePGM: Write failed" << endl;
			return false;
		}
		return true;
	}