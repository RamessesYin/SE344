#ifndef READ_H
#define READ_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include<stdlib.h>
#include <time.h>
#include<OpenGL/glu.h>
typedef struct
{
    unsigned char idLength;
    unsigned char colorMapType;
    unsigned char imageTypeCode;
    unsigned char colorMapSpec[5];
    unsigned short xOrigin;
    unsigned short yOrigin;
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char imageDesc;
} tgaheader;

typedef struct
{
    GLubyte header[6];          // Holds The First 6 Useful Bytes Of The File
    GLuint bytesPerPixel;           // Number Of BYTES Per Pixel (3 Or 4)
    GLuint imageSize;           // Amount Of Memory Needed To Hold The Image
    GLuint type;                // The Type Of Image, GL_RGB Or GL_RGBA
    GLuint height;              // Height Of Image
    GLuint width;               // Width Of Image
    GLuint bits;             // Number Of BITS Per Pixel (24 Or 32)
} TGAHEADER;

#pragma pack(1)
struct RGB {
    GLbyte blue;
    GLbyte green;
    GLbyte red;
    GLbyte alpha;
};

struct BMPInfoHeader {
    GLuint	size;
    GLuint	width;
    GLuint	height;
    GLushort  planes;
    GLushort  bits;
    GLuint	compression;
    GLuint	imageSize;
    GLuint	xScale;
    GLuint	yScale;
    GLuint	colors;
    GLuint	importantColors;
};

struct BMPHeader {
    GLushort	type;
    GLuint	size;
    GLushort	unused;
    GLushort	unused2;
    GLuint	offset;
};

struct BMPInfo {
    BMPInfoHeader		header;
    RGB				colors[1];
};
#pragma pack(8)

GLubyte* gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);
GLubyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

#endif // READ_H
