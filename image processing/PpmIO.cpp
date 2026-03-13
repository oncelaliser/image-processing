#pragma warning(disable : 4996)
#include "PpmIO.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

static void skipWhitespaceAndComments(FILE* f) {
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

// readPPM: supports P3 (ASCII) and P6 (Binary)
bool readPPM(const char* fileName, unsigned char** ppData, int& width, int& height) {
    FILE* f = nullptr;
    fopen_s(&f, fileName, "rb");
    if (!f) {
        cerr << "readPPM: Cannot open file: " << fileName << endl;
        return false;
    }

    char magic[3] = {};
    if (fscanf(f, "%2s", magic) != 1) {
        fclose(f); return false;
    }

    bool isAscii;
    if (strcmp(magic, "P3") == 0) isAscii = true;
    else if (strcmp(magic, "P6") == 0) isAscii = false;
    else {
        cerr << "readPPM: Unsupported format: " << magic << endl;
        fclose(f); return false;
    }

    int maxVal;
    skipWhitespaceAndComments(f); 
    fscanf(f, "%d", &width);
    skipWhitespaceAndComments(f); 
    fscanf(f, "%d", &height);
    skipWhitespaceAndComments(f); 
    fscanf(f, "%d", &maxVal);

    // 3 channels (R, G, B) per pixel
    const int numBytes = width * height * 3;
    *ppData = new unsigned char[numBytes];

    if (isAscii) {
        for (int i = 0; i < numBytes; ++i) {
            int val;
            fscanf(f, "%d", &val);
            (*ppData)[i] = (unsigned char)val;
        }
    }
    else {
        fgetc(f); // consume whitespace separator
        fread(*ppData, 1, numBytes, f);
    }

    fclose(f);
    return true;
}


// writePPM: writes P6 (binary) format
bool writePPM(const char* fileName, const unsigned char* pData, int width, int height) {
    FILE* f = nullptr;
    fopen_s(&f, fileName, "wb");
    if (!f) {
        cerr << "writePPM: Cannot open file: " << fileName << endl;
        return false;
    }

    fprintf(f, "P6\n%d %d\n255\n", width, height);
    fwrite(pData, 1, width * height * 3, f);
    fclose(f);
    return true;
}
