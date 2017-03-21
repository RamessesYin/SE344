#ifndef TOOL_H
#define TOOL_H
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include<stdlib.h>
#include <time.h>
#include<OpenGL/glu.h>
#include <cmath>
using namespace::std;
struct VERTEX                                       //顶点结构体
{
    float x, y, z;
};

struct face
{
    unsigned int v1,v2,v3,v4;
    unsigned int vn1,vn2,vn3,vn4;
    unsigned int vt1,vt2,vt3,vt4;
};

struct face3
{
    unsigned int v1,v2,v3;
    unsigned int vn1,vn2,vn3;
    unsigned int vt1,vt2,vt3;
};

struct material
{
    bool valid=false;
    GLfloat amb[3];
    GLfloat dif[3];
    GLfloat spe[3];
    GLfloat shininess;
    char *map_kd;
};

struct texture
{
    GLint width,height,iformat;
    GLenum format;
    GLubyte* textdata;
    GLuint id;
    bool valid=false;
};

struct vn
{
  float x;
  float y;
  float z;
};

struct vt
{
    float x;
    float y;
};

struct OBJECT
{
 vector<VERTEX>vertexs;
 vector<vn>vns;
 vector<face>faces;
 vector<face3>face3;
 vector<vt>vts;
 material mtl;
 texture tex;
};
#endif // TOOL_H
