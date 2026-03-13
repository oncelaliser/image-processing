#pragma once
// PpmIO.h
// Header for reading and writing PPM (Portable PixMap) RGB image files.
// Supports ASCII (P3) and Binary (P6) PPM formats.
// NOTE: This file is included in main.cpp but its functions are not
// currently called — included here for completeness.

#ifndef PPMIO_H
#define PPMIO_H

// Reads a PPM file (RGB color image).
// fileName  : path to the .ppm file
// ppData    : will be allocated and filled (R,G,B interleaved, 3 bytes per pixel)
// width     : will be set to image width
// height    : will be set to image height
// returns true on success, false on failure
bool readPPM(const char* fileName, unsigned char** ppData, int& width, int& height);

// Writes RGB pixel data to a PPM file.
// fileName  : output path
// pData     : R,G,B interleaved pixel data (3 bytes per pixel)
// width     : image width
// height    : image height
// returns true on success, false on failure
bool writePPM(const char* fileName, const unsigned char* pData, int width, int height);

#endif // PPMIO_H
