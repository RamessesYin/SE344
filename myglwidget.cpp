#include "myglwidget.h"
#include"read.h"
#include<QKeyEvent>
#include <QTimer>
#include <QTextStream>



MyGLWidget::MyGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    fullscreen = false;
    m_xRot = 0.0f;
    m_yRot = 0.0f;
    m_zRot = 0.0f;
    m_xSpeed = 0.0f;
    m_ySpeed = 0.0f;
    m_zSpeed = 0.0f;
    m_xPos = 0.0f;
    m_yPos = -1.2f;
    m_zPos = -10.0f;
    m_Key = 1;                                              //当前模型为球
    m_Step = 0;
    m_Steps = 200;
    m_MorphOrNot = false;
    _width=800;
    _height=800;
    min_SizeV=0;
    loadObject("/Users/Ramesses/Documents/Code/Qt/myOpenGL/arcticcondor.obj",&m_Morph2);
    loadObject("/Users/Ramesses/Documents/Code/Qt/myOpenGL/arakkoa.obj", &m_Morph1);//加载模型
     min_SizeV=m_Morph1.vertexs.size()>m_Morph2.vertexs.size()?m_Morph2.vertexs.size():m_Morph1.vertexs.size();
     min_SizeVt=m_Morph1.vts.size()>m_Morph2.vts.size()?m_Morph2.vts.size():m_Morph1.vts.size();
     min_SizeVn=m_Morph1.vns.size()>m_Morph2.vns.size()?m_Morph2.vns.size():m_Morph1.vns.size();
     m_Src = m_Dest = &m_Morph1;                             //源模型和目标模型都设置为第一个模型
     m_Helper=m_Morph1;
     QTimer *timer = new QTimer(this);                       //创建一个定时器
     //将定时器的计时信号与updateGL()绑定
     connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
     timer->start(10);                                       //以10ms为一个计时周期

}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()                         //此处开始对OpenGL进行所以设置
{
    glClearColor(0.0, 0.0, 0.0, 0.0);                   //黑色背景            
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑
    glClearDepth(1.0);                                  //设置深度缓存
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);                               //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


void MyGLWidget::resizeGL(int width, int height)                 //重置OpenGL窗口的大小
{
       glViewport( 0, 0, (GLint)width, (GLint)height );
        //Choose the Matrix mode
        glMatrixMode( GL_PROJECTION );
        //reset projection
        glLoadIdentity();
        //set perspection
        gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
        //choose Matrix mode
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
}
void MyGLWidget::paintGL()                              //从这里开始进行所以的绘制
{
       glLoadIdentity();                                   //重置当前的模型观察矩阵
       glTranslatef(m_xPos, m_yPos, m_zPos);               //平移和旋转
       glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);
       glRotatef(m_yRot, 1.0f, 1.0f, 1.0f);
       glRotatef(m_zRot, 1.0f, 1.0f, 1.0f);
       if(m_MorphOrNot)
       {
           VERTEX q; vn q1;vt q2;                                          //保存计算的临时顶点
           for(int i=0;i<min_SizeV;i++)
           {
               if (m_MorphOrNot)
               {
                   q = calculatev(i);
               }
               m_Helper.vertexs[i].x -= q.x;               //如果在变形过程，则计算中间模型
               m_Helper.vertexs[i].y -= q.y;
               m_Helper.vertexs[i].z -= q.z;

           }
           for(int i=0;i<min_SizeVn;i++)
           {
               if (m_MorphOrNot)
               {
                   q1 = calculaten(i);
               }
               m_Helper.vns[i].x -= q.x;               //如果在变形过程，则计算中间模型
               m_Helper.vns[i].y -= q.y;
               m_Helper.vns[i].z -= q.z;

           }
           for(int i=0;i<min_SizeVt;i++)
           {
               if (m_MorphOrNot)
               {
                   q2 = calculatet(i);
               }
               m_Helper.vts[i].x -= q.x;               //如果在变形过程，则计算中间模型
               m_Helper.vts[i].y -= q.y;

           }
           m_Helper.tex=m_Dest->tex;
           m_Helper.mtl=m_Dest->mtl;
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           glEnable(GL_TEXTURE_2D);
           Render(&m_Helper);
           glDisable(GL_TEXTURE_2D);
           glFlush();

       }
       else
       {
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           glEnable(GL_TEXTURE_2D);
           Render(&m_Helper);
           glDisable(GL_TEXTURE_2D);
           glFlush();

              if(check)
              {
                  float distance = rand() / double(RAND_MAX)*0.01;
                  float tmp = origin;
                  if (tmp > 1.8)particleHelper = 1;
                  if (tmp < 1.3)particleHelper = 0;
                  if (!particleHelper)
                  {
                      tmp += distance;
                  }
                  else
                  {
                      tmp -= distance;
                  }

                  origin = tmp;
                  InitParticle(origin);
                  showParticles();

              }

       }

       if (m_MorphOrNot && (m_Step <= m_Steps))
       {
           m_Step++;                                       //如果在变形过程则把当前变形步数增加
       }
       else
       {
           m_MorphOrNot = false;                           //当前变形步数大于总步数时，退出变形过程
           m_Src = m_Dest;
           m_Step = 0;
           m_Helper=*m_Dest;
       }

       m_xRot += m_xSpeed;                                 //自动增加旋转角度
       m_yRot += m_ySpeed;
       m_zRot += m_zSpeed;
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1:                                    //F1为全屏和普通屏的切换键
        fullscreen = !fullscreen;
        if (fullscreen)
        {
            showFullScreen();
        }
        else
        {
            showNormal();
        }
        break;
    case Qt::Key_Escape:                                //ESC为退出键
        close();
        break;

    case Qt::Key_PageUp:                                //PageUp按下增加m_zSpeed
        m_zSpeed += 0.1f;
        break;
    case Qt::Key_PageDown:                              //PageDown按下减少m_zSpeed
        m_zSpeed -= 0.1f;
        break;
    case Qt::Key_Down:                                  //Down按下增加m_xSpeed
        m_xSpeed += 0.1f;
        break;
    case Qt::Key_Up:                                    //Up按下减少m_xSpeed
        m_xSpeed -= 0.1f;
        break;
    case Qt::Key_Right:                                 //Right按下增加m_ySpeed
        m_ySpeed += 0.1f;
        break;
    case Qt::Key_Left:                                  //Left按下减少m_ySpeed
        m_ySpeed -= 0.1f;
        break;
    case Qt::Key_P:                                     //A按下左移物体
        m_ySpeed=0;
        m_xSpeed=0;
        m_zSpeed=0;
        break;
    case Qt::Key_I:
        check=!check;
    case Qt::Key_1:                                     //1按下进入变形过程，变形到模型1
        if ((m_Key != 1) && !m_MorphOrNot)
        {
            m_Key = 1;
            m_MorphOrNot = true;
            m_Dest = &m_Morph1;
        }
        break;
    case Qt::Key_2:                                     //2按下进入变形过程，变形到模型2
        if ((m_Key != 2) && !m_MorphOrNot)
        {

            m_Key = 2;
            m_MorphOrNot = true;
            m_Dest = &m_Morph2;
        }
        break;
    case Qt::Key_3:                                     //3按下进入变形过程，变形到模型3
        if ((m_Key != 3) && !m_MorphOrNot)
        {
            m_Key = 3;
            m_MorphOrNot = true;
            m_Dest = &m_Morph3;
        }
        break;
    case Qt::Key_4:                                     //4按下进入变形过程，变形到模型4
        if ((m_Key != 4) && !m_MorphOrNot)
        {
            m_Key = 4;
            m_MorphOrNot = true;
            m_Dest = &m_Morph4;
        }
        break;
    }
}

VERTEX MyGLWidget::calculatev(int i)                     //计算第i个顶点变形过程每一步的位移
{
    VERTEX a;
    a.x = (m_Src->vertexs[i].x - m_Dest->vertexs[i].x) / m_Steps;
    a.y = (m_Src->vertexs[i].y - m_Dest->vertexs[i].y) / m_Steps;
    a.z = (m_Src->vertexs[i].z - m_Dest->vertexs[i].z) / m_Steps;
    return a;
}

vt MyGLWidget::calculatet(int i)                     //计算第i个顶点法向量变形过程每一步的位移
{
    vt a;
    a.x = (m_Src->vts[i].x - m_Dest->vts[i].x) / m_Steps;
    a.y = (m_Src->vts[i].y - m_Dest->vts[i].y) / m_Steps;
    return a;
}


vn MyGLWidget::calculaten(int i)                     //计算第i个顶点法向量变形过程每一步的位移
{
    vn a;
    a.x = (m_Src->vns[i].x - m_Dest->vns[i].x) / m_Steps;
    a.y = (m_Src->vns[i].y - m_Dest->vns[i].y) / m_Steps;
    a.z = (m_Src->vns[i].z - m_Dest->vns[i].z) / m_Steps;
    return a;
}



void MyGLWidget::showParticles()
{

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPointSize(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    srand((unsigned)time(NULL));
    float tmpx, tmpy, tmpz, color1, color2, color3, color4 = 0.0f;
    glPushMatrix();
    for (int i = 0; i<particles.size(); i++)
    {

       color1 = rand() / double(RAND_MAX);

       color2 = rand() / double(RAND_MAX);

       color3 = rand() / double(RAND_MAX);

       glColor3f(color1, color2, color3);

       if(m_Key==1)
       {
           glVertex3f(particles[i].x*1.5, particles[i].y*1.5+0.8f, particles[i].z*1.52);
       }

       if(m_Key==2)
       {
           glVertex3f(particles[i].x*2.5, particles[i].y*2.5+1.5f, particles[i].z*2.5);
       }
    }
    glPopMatrix();
    particles.clear();
    glEnd();
    glEnable(GL_LIGHTING);
}

void MyGLWidget::InitParticle(float radius)
{
    GLfloat  xx, yy, zz, M, N;
    xx = yy = zz = 0;
    M = N = 27;
    float step_z = M_PI / M;  //z方向每次步进的角度
    float step_xy = 2 * M_PI / N; //x,y平面每次步进的角度
    float x[4], y[4], z[4];//用来存坐标
    float nx[4], ny[4], nz[4];

    float angle_z = 0.0; //起始角度
    float angle_xy = 0.0;
    int i = 0, j = 0;

    for (i = 0; i<M; i++)
    {
        angle_z = i * step_z;

        for (j = 0; j<N; j++)
        {
            angle_xy = j * step_xy;
            VERTEX p1;
            p1.x = radius * sin(angle_z) * cos(angle_xy);
            p1.y = radius * sin(angle_z) * sin(angle_xy);
            p1.z = radius * cos(angle_z);

            VERTEX p2;
            p2.x = radius * sin(angle_z + step_z) * cos(angle_xy);
            p2.y = radius * sin(angle_z + step_z) * sin(angle_xy);
            p2.z = radius * cos(angle_z + step_z);

            VERTEX p3;
            p3.x = radius*sin(angle_z + step_z)*cos(angle_xy + step_xy);
            p3.y = radius*sin(angle_z + step_z)*sin(angle_xy + step_xy);
            p3.z = radius*cos(angle_z + step_z);

            particles.push_back(p1);
            particles.push_back(p2);
            particles.push_back(p3);

        }
    }
}

void MyGLWidget::loadObject(const char* filename,OBJECT* obj)
{
  cout<<"load OBJ"<<filename<<endl;
  string line;
  cout<<filename<<endl;
  ifstream fin(filename);
  if(!fin)
  {
      cout<<"cannot open file\n";
      return;
  }
  while(getline(fin,line))
  {
      stringstream is(line);
      string s;
      is>>s;
      if(s=="mtllib")
      {
          is>>s;
          s="/Users/Ramesses/Documents/Code/Qt/myOpenGL/"+s;
          loadMaterial(s.c_str(),obj);
          cout<<obj->mtl.map_kd<<endl;
      }
      else if(s=="v")
      {
          VERTEX v;
          sscanf(line.c_str()+2,"%f %f %f\n",&v.x,&v.y,&v.z);
          obj->vertexs.push_back(v);
      }
      else if(s=="vn")
      {
          vn v;
          sscanf(line.c_str()+2,"%f %f %f\n",&v.x,&v.y,&v.z);
          obj->vns.push_back(v);
      }
      else if(s=="vt")
      {
          vt v;
          sscanf(line.c_str()+2,"%f %f\n",&v.x,&v.y);
          obj->vts.push_back(v);
      }
      else if(s=="f")
      {
          face f;
          face3 f3;
          if(sscanf(line.c_str()+2,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&f3.v1,&f3.vt1,&f3.vn1,&f3.v2,&f3.vt2,&f3.vn2,&f3.v3,&f3.vt3,&f3.vn3))
              obj->face3.push_back(f3);
          if(sscanf(line.c_str()+2,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d \n",&f.v1,&f.vt1,&f.vn1,&f.v2,&f.vt2,&f.vn2,&f.v3,&f.vt3,&f.vn3,&f.v4,&f.vn4,&f.vt4))
              obj->faces.push_back(f);
      }
      else if(s=="Ks")
      {
          sscanf(line.c_str()+3,"%f %f %f\n",obj->mtl.spe,obj->mtl.spe+1,obj->mtl.spe+2);
      }
  }
  if(!obj->mtl.valid)return;
  string t=string(obj->mtl.map_kd);
  t=t.substr(t.size()-3,t.size());
  if(t=="tga")
      obj->tex.textdata=gltReadTGABits(obj->mtl.map_kd,&obj->tex.width,&obj->tex.height,&obj->tex.iformat,&obj->tex.format);
  else if(t=="bmp")
  {
      obj->tex.textdata=gltReadBMPBits(obj->mtl.map_kd,&obj->tex.width,&obj->tex.height);
      cout<<"here for bmp"<<endl;
      obj->tex.format=GL_BGR_EXT;
      obj->tex.iformat=GL_RGB;
  }
  else
  {
      cout<<"texture file type not supported\n";
  }
  if(obj->tex.textdata==NULL)
  {
      cout<<"cannot open texture file:"<<obj->mtl.map_kd<<endl;
  }
  if(obj->tex.textdata)
  {
      cout<<"here to init the text"<<endl;
      obj->tex.valid=true;
  }
}

void MyGLWidget::loadMaterial(const char *filename,OBJECT* obj)
{
    cout<<"parsing MTL: "<<filename<<endl;
    string line;
    ifstream fin(filename);
    if(!fin)
    {
        cout<<"cannot open file\n";
        return;
    }
    obj->mtl.valid=true;
    while(getline(fin,line))
    {
        stringstream is(line);
        string s;
        is>>s;
        if(s=="map_Kd")
        {
            is>>s;
            s.size();
            char *ss=(char*) malloc(line.size());
            sscanf(line.c_str()+7,"%s",ss);
            obj->mtl.map_kd=ss;
        }
        else if(s=="Ns")
        {
          sscanf(line.c_str()+3,"%f\n",&obj->mtl.shininess);
          cout<<"Ns"<<endl;
        }
        else if(s=="Ka")
        {
            cout<<"Ka"<<endl;
          sscanf(line.c_str()+3,"%f %f %f\n",obj->mtl.amb,obj->mtl.amb+1,obj->mtl.amb+2);
        }
        else if(s=="Kd")
        {
            cout<<"Kd"<<endl;
            sscanf(line.c_str()+3,"%f %f %f\n",obj->mtl.dif,obj->mtl.dif+1,obj->mtl.dif+2);
        }

    }
}

void MyGLWidget::Render(OBJECT* obj)
{
  if(1)
  {
      glGenTextures(1,&obj->tex.id);
      glBindTexture(GL_TEXTURE_2D,obj->tex.id);
      glTexImage2D(GL_TEXTURE_2D,0,obj->tex.iformat,obj->tex.width,obj->tex.height,0,obj->tex.format,GL_UNSIGNED_BYTE,obj->tex.textdata);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
      glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
      glBindTexture(GL_TEXTURE_2D,obj->tex.id);
      if(obj->mtl.valid)
      {
          glMaterialfv(GL_FRONT,GL_AMBIENT,obj->mtl.amb);
          glMaterialfv(GL_FRONT,GL_DIFFUSE,obj->mtl.dif);
          glMaterialfv(GL_FRONT,GL_SPECULAR,obj->mtl.spe);
          glMaterialf(GL_FRONT,GL_SHININESS,obj->mtl.shininess);
      }
  }

  glBegin(GL_QUADS);
  if(obj->face3.size()!=0)
  {
      for(int i=0;i<obj->face3.size();i++)
      {
          //cout<<"print the f3 points"<<endl;
          VERTEX v1=obj->vertexs[obj->face3[i].v1-1];
          VERTEX v2=obj->vertexs[obj->face3[i].v2-1];
          VERTEX v3=obj->vertexs[obj->face3[i].v3-1];
          vn vn1=obj->vns[obj->face3[i].vn1-1];
          vn vn2=obj->vns[obj->face3[i].vn2-1];
          vn vn3=obj->vns[obj->face3[i].vn3-1];
          vt vt1=obj->vts[obj->face3[i].vt1-1];
          vt vt2=obj->vts[obj->face3[i].vt2-1];
          vt vt3=obj->vts[obj->face3[i].vt3-1];

          glNormal3f(vn1.x,vn1.y,vn1.z);
          glTexCoord2f(vt1.x,vt1.y);
          glVertex3f(v1.x,v1.y,v1.z);

          glNormal3f(vn2.x,vn2.y,vn2.z);
          glTexCoord2f(vt2.x,vt2.y);
          glVertex3f(v2.x,v2.y,v2.z);

          glNormal3f(vn3.x,vn3.y,vn3.z);
          glTexCoord2f(vt3.x,vt3.y);
          glVertex3f(v3.x,v3.y,v3.z);

      }
  }
  else if(obj->faces.size()!=0)
  {
      for(int i=0;i<obj->faces.size();i++)
      {
          //cout<<"print the faces points"<<endl;
          VERTEX v1=obj->vertexs[obj->faces[i].v1-1];
          VERTEX v2=obj->vertexs[obj->faces[i].v2-1];
          VERTEX v3=obj->vertexs[obj->faces[i].v3-1];
          VERTEX v4=obj->vertexs[obj->faces[i].v4-1];
          vn vn1=obj->vns[obj->faces[i].vn1-1];
          vn vn2=obj->vns[obj->faces[i].vn2-1];
          vn vn3=obj->vns[obj->faces[i].vn3-1];
          vn vn4=obj->vns[obj->faces[i].vn4-1];
          vt vt1=obj->vts[obj->faces[i].vt1-1];
          vt vt2=obj->vts[obj->faces[i].vt2-1];
          vt vt3=obj->vts[obj->faces[i].vt3-1];
          vt vt4=obj->vts[obj->faces[i].vt4-1];

          glNormal3f(vn1.x,vn1.y,vn1.z);
          glTexCoord2f(vt1.x,vt1.y);
          glVertex3f(v1.x,v1.y,v1.z);

          glNormal3f(vn2.x,vn2.y,vn2.z);
          glTexCoord2f(vt2.x,vt2.y);
          glVertex3f(v2.x,v2.y,v2.z);

          glNormal3f(vn3.x,vn3.y,vn3.z);
          glTexCoord2f(vt3.x,vt3.y);
          glVertex3f(v3.x,v3.y,v3.z);

          glNormal3f(vn4.x,vn4.y,vn4.z);
          glTexCoord2f(vt4.x,vt4.y);
          glVertex3f(v4.x,v4.y,v4.z);
      }
  }

  glEnd();



}

