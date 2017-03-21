#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include"tool.h"
#include"particle.h"

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();

protected:
    //对3个纯虚函数的重定义
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);           //处理键盘按下事件

private:
    void showParticles();
    void loadObject(const char* filename,OBJECT *obj);
    void loadMaterial(const char* filename,OBJECT *obj);
    void Render(OBJECT*obj);
    void InitParticle(float radius);
    VERTEX calculatev(int i);                        //计算第i个顶点变形过程每一步的位移
    vn calculaten(int i);                        //计算第i个顶点变形过程每一步的位移
    vt calculatet(int i);                        //计算第i个顶点变形过程每一步的位移
    vector<VERTEX>particles;
private:
    bool fullscreen;                                //是否全屏显示

       GLfloat m_xRot;                                 //x轴旋转角度
       GLfloat m_yRot;                                 //y轴旋转角度
       GLfloat m_zRot;                                 //z轴旋转角度
       GLfloat m_xSpeed;                               //x轴旋转速度
       GLfloat m_ySpeed;                               //y轴旋转速度
       GLfloat m_zSpeed;                               //z轴旋转速度
       GLfloat m_xPos;                                 //x轴坐标
       GLfloat m_yPos;                                 //y轴坐标
       GLfloat m_zPos;                                 //z轴坐标
       float origin=1.3;
       bool check=false;
       int particleHelper=1;
       int m_Key;                                      //物体的标示符
       int m_Step;                                     //当前变形步数
       int m_Steps;                                    //变形的总步数
       bool m_MorphOrNot;                              //是否在变形过程
       int _width;
       int _height;
       int min_SizeV;
       int min_SizeVn;
       int min_SizeVt;
       OBJECT m_Morph1;                                //要绘制的4个物体
       OBJECT m_Morph2;
       OBJECT m_Morph3;
       OBJECT m_Morph4;
       OBJECT m_Helper;                                //协助绘制变形过程的物体(中间模型)
       OBJECT *m_Src;                                  //变形的源物体
       OBJECT *m_Dest;                                 //变形的目标物体
};





#endif // MYGLWIDGET_H
