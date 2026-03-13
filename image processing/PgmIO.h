#pragma once
#ifndef PGMIO_H
#define PGMIO_H

// Reads a PGM file from disk.
// fileName  : path to the .pgm file
// ppData    : pointer-to-pointer, will be allocated and filled with pixel data
// width     : will be set to the image width
// height    : will be set to the image height
// returns true on success, false on failure

bool readPGM(const char* fileName, unsigned char** ppData, int& width, int& height);

// Writes grayscale pixel data to a PGM file on disk.
// fileName  : path to output .pgm file
// pData     : pointer to pixel data (row-major, 1 byte per pixel)
// width     : image width in pixels
// height    : image height in pixels
// returns true on success, false on failure

bool writePGM(const char* fileName, const unsigned char* pData, int width, int height);

#endif //PGMIO_H

