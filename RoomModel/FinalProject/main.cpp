#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <bits/stdc++.h>

#define ID_TIMER    1                        // 自动旋转时钟
#define PI 3.14159
#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

GLuint texFloor;
GLuint texShip;
GLuint texSea;
GLuint texCarpet;
GLuint texDoor;
GLuint texSky;
GLfloat ylookAt = 2;
GLfloat zlookAt = 5.0;
GLfloat xlookAt = 0.0;

GLuint control = 0;
GLuint projector = 0;

GLfloat rotStep = 30.0f;
GLfloat paceStep = 30.0f;
GLfloat moveStep = 0.05f;
GLfloat backRot = 0.0f;
GLfloat lookStep = 0.1f;
GLboolean flag = false;
GLboolean flag2 = false;                     //控制后退
GLboolean flag3 = false;                     //控制前进
GLboolean flag4 = false;


GLfloat xRot;                           // 设置在X方向的旋转的速度定义
GLfloat yRot;                           // 设置在Y方向的旋转的速度定义
GLfloat zRot;                           // 设置在Z方向的旋转的速度定义

GLfloat armL;
GLfloat armR;

GLfloat legL = 15.0f;
GLfloat legR = -15.0f;

const int n = 10000;
GLfloat R1 = 0;
GLfloat R2 = 0;

GLfloat xPosition;                           // X方向的位置定义
GLfloat yPosition;                           // Y方向的位置定义
GLfloat zPosition;                           // Z方向的位置定义

GLfloat xPosition_person;
GLfloat yPosition_person;
GLfloat zPosition_person;

GLfloat color[4]={0.0f, 0.0f, 0.0f, 1.0f};

GLboolean keys[256];                                         // 用于键盘例程的数组
static GLboolean mouseFlag = FALSE;

GLuint load_texture(const char* file_name);
int power_of_two(int n);
void SetupRC();
GLvoid Init();
GLvoid ChangeSize(GLsizei, GLsizei);
GLvoid Draw3D1();
GLvoid Draw3D2();
GLvoid Draw3D3();
GLvoid Draw3D();
GLuint KeyPress();
GLuint MainWndMenu();
GLvoid CALLBACK timerProc(HWND,UINT,UINT,DWORD);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // 窗体过程函数声明
int commandBtns(HWND);


HINSTANCE hInstance;                                         // 系统实例句柄
HWND      hWndMain;                                          // 主窗体句柄
HMENU     hPopup;
HWND      hWnd;                                              // 窗体句柄变量定义
HDC       hDC;                                               // 设备描述表变量定义
PIXELFORMATDESCRIPTOR pfd;                                   // 像素格式结构变量定义
HGLRC     hGLRC;                                             // OpenGL渲染描述表变量定义

HMENU hMenuMain;                                             // 主窗体菜单句柄
HMENU hMenuWindow;
#define WINDOW_MENU_POS    1000
#define IDM_CTRL           2001
#define IDM_AUTO           2002
#define IDM_STOP           2003
#define IDM_BLACK          2004
#define IDM_GRAY           2005
#define IDM_SKYBLUE        2006
#define IDM_WHITE          2007

HWND    hLabel;
HWND    hCmdBtns;

#define IDC_FRAME          10000                // 框架控件唯一标识
#define IDC_RotateX1       10001                // 按钮控件唯一标识
#define IDC_RotateX2       10002                // 按钮控件唯一标识
#define IDC_RotateY1       10003                // 按钮控件唯一标识
#define IDC_RotateY2       10004                // 按钮控件唯一标识
#define IDC_RotateZ1       10005                // 按钮控件唯一标识
#define IDC_RotateZ2       10006                // 按钮控件唯一标识

#define IDC_TranslateX1    10011                // 按钮控件唯一标识
#define IDC_TranslateX2    10012                // 按钮控件唯一标识
#define IDC_TranslateY1    10013                // 按钮控件唯一标识
#define IDC_TranslateY2    10014                // 按钮控件唯一标识
#define IDC_TranslateZ1    10015                // 按钮控件唯一标识
#define IDC_TranslateZ2    10016                // 按钮控件唯一标识
#define IDC_Room1          10017                // 按钮控件唯一标识
#define IDC_Room2          10018                // 按钮控件唯一标识
#define IDC_ON             10019                // 按钮控件唯一标识
#define IDC_OFF            10020                // 按钮控件唯一标识

//********************************************************************
//*  打开OpenGL事件定义                                              *
//********************************************************************
//
GLuint Open(HWND phWnd)
{
  hWnd = phWnd;

  hDC = GetDC(hWnd);

  pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);                          // 格式描述符的大小
  pfd.nVersion        = 1;                       // 版本号
  pfd.dwFlags         = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER; // 支持窗口双缓冲OPENGL
  pfd.dwLayerMask     = PFD_MAIN_PLANE;          //
  pfd.iPixelType      = PFD_TYPE_RGBA;           // RGBA格式
  pfd.cColorBits      = 24;                      // 色彩深度
  pfd.cRedBits        = 0;                       // 忽略的色彩位
  pfd.cRedShift       = 0;                       // 忽略的色彩位
  pfd.cGreenBits      = 0;                       // 忽略的色彩位
  pfd.cGreenShift     = 0;                       // 忽略的色彩位
  pfd.cBlueBits       = 0;                       // 忽略的色彩位
  pfd.cBlueShift      = 0;                       // 忽略的色彩位
  pfd.cAlphaBits      = 0;                       // 无Alpha缓存
  pfd.cAlphaShift     = 0;                       // 忽略Shift Bit
  pfd.cAccumBits      = 0;                       // 无聚集缓存
  pfd.cAccumRedBits   = 0;                       // 忽略聚集位
  pfd.cAccumGreenBits = 0;                       // 忽略聚集位
  pfd.cAccumBlueBits  = 0;                       // 忽略聚集位
  pfd.cAccumAlphaBits = 0;                       // 忽略聚集位
  pfd.cDepthBits      = 24;                      // 24位 Z-缓存 (深度缓存)
  pfd.cStencilBits    = 0;                       // 无模板缓存
  pfd.cAuxBuffers     = 0;                       // 无辅助缓存
  pfd.iLayerType      = 0;                       // 主绘图层
  pfd.bReserved       = 0;                       // 保留
  pfd.iLayerType      = 0;                       // 忽略层遮罩
  pfd.dwVisibleMask   = 0;                       // 忽略层遮罩
  pfd.dwDamageMask    = 0;                       // 忽略层遮罩

  int pixelformat = ChoosePixelFormat(hDC, &pfd);

  SetPixelFormat(hDC, pixelformat, &pfd);

  hGLRC = wglCreateContext(hDC);

  wglMakeCurrent(hDC,hGLRC);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);          // 初始化背景为灰色

  return 0;
} // 打开OpenGL事件定义

/********************************************************************/
/*  主窗体类注册函数定义                                            */
/********************************************************************/
//
GLuint MainWndCls()
{
  //主窗体类注册函数定义
  WNDCLASSEX wcex;
  wcex.cbSize        = sizeof(WNDCLASSEX);
  wcex.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  wcex.lpfnWndProc   = (WNDPROC)WndProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = LoadIcon(hInstance,(LPCTSTR)IDI_APPLICATION);
  wcex.hCursor       = LoadCursor(NULL,IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = TEXT("WndCls");
  wcex.hIconSm       = LoadIcon(NULL,(LPCTSTR)IDI_APPLICATION);

  if(!RegisterClassEx(&wcex)) return 1;
  return 0;
} // 主窗体类注册函数

/********************************************************************/
/*  建立主窗体函数定义                                              */
/********************************************************************/
//
GLuint MainWndCreate()
{
  //建立主窗体
  hWndMain = CreateWindowEx(WS_EX_CLIENTEDGE,
                        TEXT("WndCls"),
                        TEXT("OpenGL-Windows GUI!"),
                        WS_OVERLAPPED   |   WS_SYSMENU   |WS_MINIMIZEBOX|WS_CLIPCHILDREN,
                        100, 100, 1400, 600,
                        NULL,
                        hMenuMain,
                        hInstance,
                        NULL);

  if(!hWndMain) return 1;

  ShowWindow(hWndMain,SW_SHOWNORMAL);
  UpdateWindow(hWndMain);

  Init();

  return 0;
} // 建立主窗体函数

//********************************************************************
//*  主窗体过程函数定义                                          *
//********************************************************************
//
LRESULT CALLBACK WndProc(HWND   hWnd,
                         UINT   message,
                         WPARAM wParam,
                         LPARAM lParam)
{
  POINT pt;
  POINT ptCurr;
  PAINTSTRUCT ps;
  switch (message)
  {
    case WM_CREATE:                                  // 窗体创建讯息
      commandBtns(hWnd);
      Open(hWnd);
      break;

    case WM_PAINT:                                   // 重新绘制讯息
      BeginPaint(hWnd,&ps);
      SetCursor (LoadCursor (NULL, IDC_HAND));

      Draw3D();
      if(!flag){
        flag = true;
        SendMessage(hWnd, WM_PAINT, 0, 0L);
      }

      EndPaint(hWnd,&ps);
      break;

    case WM_SIZE:                                    // 改变窗体大小讯息
      ChangeSize(LOWORD(lParam),HIWORD(lParam));
      break;

    case WM_KEYDOWN:                                 // 响应按键操作讯息
      keys[wParam] = TRUE;
      KeyPress();
      SendMessage(hWnd, WM_PAINT, 0, 0L);
      break;

    case WM_KEYUP:                                   // 响应松开键操作讯息
      keys[wParam] = FALSE;
      SendMessage(hWnd, WM_PAINT, 0, 0L);
      break;

    case WM_LBUTTONDOWN:                             // 响应按下鼠标左键讯息
      mouseFlag = TRUE;
      GetCursorPos(&ptCurr);
      break;

    case WM_MOUSEMOVE:
      if(mouseFlag)
      {
        xRot = (((int)(short)LOWORD(lParam)) - ptCurr.x)*0.5;
        yRot = (((int)(short)HIWORD(lParam)) - ptCurr.y)*0.5;
        SendMessage(hWnd, WM_PAINT, 0, 0L);
      }
      break;

    case WM_LBUTTONUP:                               // 抬起鼠标左键讯息
      mouseFlag = FALSE;
      xRot = 0;
      yRot = 0;
      break;

    case WM_RBUTTONDOWN:                               // 抬起鼠标左键讯息
      GetCursorPos(&pt);
      TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, pt.x, pt.y, (DWORD)NULL, hWnd, NULL);
      break;


    case WM_COMMAND:
      switch(wParam)
	  {
        case IDM_AUTO:
          SetTimer(hWnd, ID_TIMER, 200 ,timerProc);
          break;
        case IDM_STOP:
          KillTimer(hWnd,ID_TIMER);
          break;
        case IDM_BLACK:
          color[0] = 0.0f;
          color[1] = 0.0f;
          color[2] = 0.0f;
          break;
        case IDM_GRAY:
          color[0] = 0.5f;
          color[1] = 0.5f;
          color[2] = 0.5f;
          break;
        case IDM_SKYBLUE:
          color[0] = 0.3398f;
          color[1] = 0.9766f;
          color[2] = 0.9961f;
          break;
        case IDM_WHITE:
          color[0] = 1.0f;
          color[1] = 1.0f;
          color[2] = 1.0f;
          //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
          break;

        case IDC_Room1:
          //flag = false;
          if(flag4){
            flag4 = false;
          }
          else{
            flag4 = true;
          }
          SendMessage(hWnd, WM_PAINT, 0, 0L);
          break;

        case IDC_Room2:
          //flag = false;
          control = 2;
          SendMessage(hWnd, WM_PAINT, 0, 0L);
          break;

        case IDC_ON:
          projector = 1;
          break;

        case IDC_OFF:
          projector = 0;
          break;

        case IDC_RotateX1:
          xlookAt += lookStep;
          break;

        case IDC_RotateX2:
          xlookAt -= lookStep;
          break;

        case IDC_RotateY1:
          ylookAt += lookStep;
          break;

        case IDC_RotateY2:
          ylookAt -= lookStep;
          break;

        case IDC_RotateZ1:
          zlookAt += lookStep;
          break;

        case IDC_RotateZ2:
          zlookAt -= lookStep;
          break;

        case IDC_TranslateX1:
          xPosition -= moveStep;
          break;

        case IDC_TranslateX2:
          xPosition += moveStep;
          break;

        case IDC_TranslateY1:
          yPosition += moveStep;
          break;

        case IDC_TranslateY2:
          yPosition -= moveStep;
          break;

        case IDC_TranslateZ1:
          zPosition += moveStep;
          break;

        case IDC_TranslateZ2:
          zPosition -= moveStep;
          break;

        default:
          return DefWindowProc(hWnd,message,wParam,lParam);
	  }
	  SendMessage(hWnd, WM_PAINT, 0, 0L);
	  SetFocus(hWnd);
      break;

    case WM_DESTROY:                                 // 退出程序
      PostQuitMessage(0);
      break;

    default:
      return DefWindowProc(hWnd,message,wParam,lParam);

  }
  return DefWindowProc(hWnd,message,wParam,lParam);

}// 窗体过程定义

//********************************************************************
//*  主控函数                                                        *
//********************************************************************
//
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
  MSG msg;                                           // 定义消息

  if(MainWndCls())          return 1;                // 主窗体类注册

  MainWndMenu();                                     // 显示主窗体菜单

  if(MainWndCreate())       return 2;                // 建立主窗体

  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);                          // 转换消息
    DispatchMessage(&msg);                           // 分发消息
  }

  return (int)msg.wParam;
}// 主控函数


//********************************************************************
//*  OpenGL初始化场景事件                                            *
//********************************************************************
//
GLvoid Init()
{
glBlendFunc(GL_SRC_ALPHA,GL_ONE);                       // 使用调和
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);			// 设置背景色为黑色
glEnable(GL_TEXTURE_2D);
glClearDepth(1.0);					// 清除深度缓存
glDepthFunc(GL_LESS);					// 设置为LESS
glEnable(GL_DEPTH_TEST);				// 深度缓存可用

glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 呈现Niceset
} //OpenGL初始化场景事件

//********************************************************************
//*  使场景随着窗口的大小变化而变化事件定义                          *
//********************************************************************
//
GLvoid ChangeSize(GLsizei width, GLsizei height)
{
    float ar = (float) (width-180) / (float) height;

    glViewport(0, 0, width-180, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}// 使场景随着窗口的大小变化而变化事件定义
GLfloat position;
//********************************************************************
//*  对图形进行绘制(渲染)事件                                        *
//********************************************************************
//
GLvoid Draw3D()
{
    glClearColor(color[0], color[1], color[2], color[3]);
    if(!flag4){
       Draw3D1();
    }
    else if(flag4){
       Draw3D2();
    }
                           // 交换缓存(双缓存)
} //对图形进行绘制(渲染)事件

GLvoid Draw3D1()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
gluLookAt(xlookAt,ylookAt,zlookAt,0, 0, 0, 0, 0, -1);
GLfloat light2_position[] = {-0.57f, -0.15f , -0.87f, 1.0f };
glColor3f( 0, 1, 0 );

glPushMatrix();

    //glDisable(GL_TEXTURE_2D);

    glTranslated(xPosition,yPosition+0.5,zPosition);
    //glTranslated(0, 0, -2.5);
    glRotated(xRot,1,0,0);
    glRotated(yRot,0,1,0);
    glRotated(zRot,0,0,1);
    glTranslated(0, -0.12, 0);
    //glRotated(90,1,0,0);
    //glRotated(30,0,0,1);

    glDisable(GL_TEXTURE_2D);
        //纹理
     glPushMatrix();
        glTranslated(0, -0.1, 0);
        glColor3f(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        //墙1
        glBindTexture(GL_TEXTURE_2D, texShip);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 2.7, -1.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 2.7, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5,-1.0, -1.2);
        glEnd();

        //壁画
        glBindTexture(GL_TEXTURE_2D, texSea);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.3, -1.18);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 1.5, -1.18);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5, 1.5, -1.18);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5,0.3, -1.18);
        glEnd();

        //墙2
        glBindTexture(GL_TEXTURE_2D, texShip);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, 2.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 2.7, 2.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5, 2.7, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
        glEnd();
//        glBindTexture(GL_TEXTURE_2D, texShip);
//        glBegin(GL_QUADS);
//            glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5, -1.0, 2.2);
//            glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, 2.7, 2.2);
//            glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 2.7, -1.2);
//            glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5, -1.0, -1.2);
//        glEnd();

        glBindTexture(GL_TEXTURE_2D, texCarpet);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, 2.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, -1.0, 2.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5,-1.0, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
        glEnd();

        //窗户
        glBindTexture(GL_TEXTURE_2D, texSky);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.48, 0.0, 2.0);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.48, 1.3, 2.0);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.48, 1.3, 0.65);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.48, 0.0, 0.65);
        glEnd();

//        //地毯
//        glBindTexture(GL_TEXTURE_2D, texCarpet);
//        glBegin(GL_QUADS);
//            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.9, 1.8);
//            glTexCoord2f(0.0f, 1.0f); glVertex3f(1.5, 0.9, 1.8);
//            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5,0.9, -0.8);
//            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 0.9, -0.8);
//        glEnd();
        glDisable(GL_TEXTURE_2D);
     glPopMatrix();
    glPushMatrix();
    //灯球
      glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
      glTranslated(-0.57f, -0.15f , -0.87f);
      glDisable(GL_LIGHTING);
      glColor3f(1.0f, 1.0f, 0.0f);
      glutSolidSphere(0.05f, 50.0f, 50.0f);

    glPopMatrix();
    glPushMatrix();
      glEnable(GL_LIGHTING);
      glTranslated(0.4, -0.8, -0.46);

    //床体
      glPushMatrix();
        //glRotated(-90, 0, 1, 0);
        glTranslated(0, 0, 0.15);
        glScaled(4.5, 1.5, 5);
        glutSolidCube(0.3);
      glPopMatrix();
    //床靠背
      glPushMatrix();
        //glRotated(-90, 0, 1, 0);
        glTranslated(0, 0.155, -0.63);
        glScaled(4.5, 3, 0.5);
        glutSolidCube(0.3);
      glPopMatrix();

      //枕头
      glPushMatrix();
        glColor3f(0.5, 0, 0.5);
        glTranslated(0, 0.3, -0.5);
        glScaled(1, 0.3, 0.3);
        glutSolidSphere(0.3, 60, 60);
      glPopMatrix();

      glTranslated(0, 0, -0.6);
      //床头柜
      glPushMatrix();
        //glDisable(GL_LIGHTING);

        glTranslated(0.22, 0, 0);
        glPushMatrix();
          glColor3f(1, 1, 1);
          glTranslated(-1.2, 0.20, 0.17);
          glScaled(1.7, 0.2, 1.7);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-0.93, -0.02, 0.17);
          glScaled(0.2, 1.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-1.47, -0.02, 0.17);
          glScaled(0.2, 1.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-1.2, -0.02, -0.10);
          glScaled(1.7, 1.5, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子底面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0, 0.15);
          glScaled(1.5, 0.2, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子左侧面
        glPushMatrix();
          glTranslated(-1.4, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子右侧面
        glPushMatrix();
          glTranslated(-1.0, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子前面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(1.5, 0.7, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子把手
        glPushMatrix();
          glColor3f(0, 0, 0);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(0.5, 0.5, 0.5);
          glutSolidSphere(0.1, 60, 60);
        glPopMatrix();

        glTranslated(0, -0.21, 0);
        //下面柜子底面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0, 0.15);
          glScaled(1.5, 0.2, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子左侧面
        glPushMatrix();
          glTranslated(-1.4, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子右侧面
        glPushMatrix();
          glTranslated(-1.0, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子前面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(1.5, 0.7, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子把手
        glPushMatrix();
          glColor3f(0, 0, 0);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(0.5, 0.5, 0.5);
          glutSolidSphere(0.1, 60, 60);
        glPopMatrix();
      glPopMatrix();

      glTranslated(2.2, 0, 0);
      glPushMatrix();
        //glDisable(GL_LIGHTING);
        glPushMatrix();
          glColor3f(1, 1, 1);
          glTranslated(-1.2, 0.20, 0.17);
          glScaled(1.7, 0.2, 1.7);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-0.93, -0.02, 0.17);
          glScaled(0.2, 1.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-1.47, -0.02, 0.17);
          glScaled(0.2, 1.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        glPushMatrix();
          glTranslated(-1.2, -0.02, -0.10);
          glScaled(1.7, 1.5, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子底面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0, 0.15);
          glScaled(1.5, 0.2, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子左侧面
        glPushMatrix();
          glTranslated(-1.4, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子右侧面
        glPushMatrix();
          glTranslated(-1.0, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子前面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(1.5, 0.7, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //柜子把手
        glPushMatrix();
          glColor3f(0, 0, 0);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(0.5, 0.5, 0.5);
          glutSolidSphere(0.1, 60, 60);
        glPopMatrix();

        glTranslated(0, -0.21, 0);
        //下面柜子底面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0, 0.15);
          glScaled(1.5, 0.2, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子左侧面
        glPushMatrix();
          glTranslated(-1.4, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子右侧面
        glPushMatrix();
          glTranslated(-1.0, 0.08, 0.15);
          glScaled(0.2, 0.6, 1.5);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子前面
        glPushMatrix();
          glColor3f(0, 0, 1);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(1.5, 0.7, 0.2);
          glutSolidCube(0.3);
        glPopMatrix();

        //下面柜子把手
        glPushMatrix();
          glColor3f(0, 0, 0);
          glTranslated(-1.2, 0.07, 0.39);
          glScaled(0.5, 0.5, 0.5);
          glutSolidSphere(0.1, 60, 60);
        glPopMatrix();
      glPopMatrix();

      //香薰
      glPushMatrix();
        glColor3f(1, 0.5, 0.5);
        glTranslated(-1.3, 0.25, 0.3);
        glBegin(GL_QUAD_STRIP);
           R1 = 0.07;
           for(int i=0;i<n;i=i++){
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0.15, R1*sin(2*PI/n*i)-0.1);
           }
        glEnd();
      glPopMatrix();

      //香薰棒1
      glPushMatrix();
        glColor3f(0, 1, 1);
        glTranslated(-1.3, 0.25, 0.3);
        glRotated(15,0, 0, 1);
        glBegin(GL_QUAD_STRIP);
           R1 = 0.01;
           for(int i=0;i<n;i=i++){
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0.25, R1*sin(2*PI/n*i)-0.1);
           }
        glEnd();
      glPopMatrix();

      //香薰棒2
      glPushMatrix();
        glColor3f(1, 1, 0);
        glTranslated(-1.3, 0.30, 0.3);
        glRotated(-15,0, 0, 1);
        glBegin(GL_QUAD_STRIP);
           R1 = 0.01;
           for(int i=0;i<n;i=i++){
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0.25, R1*sin(2*PI/n*i)-0.1);
           }
        glEnd();
      glPopMatrix();

      //香薰棒3
      glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslated(-1.3, 0.25, 0.3);
        glRotated(30,0, 0, 1);
        glBegin(GL_QUAD_STRIP);
           R1 = 0.01;
           for(int i=0;i<n;i=i++){
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
              glVertex3f(R1*cos(2*PI/n*i)+0.1, 0.25, R1*sin(2*PI/n*i)-0.1);
           }
        glEnd();
      glPopMatrix();
    glPopMatrix();

    //窗户架
    glPushMatrix();
      glColor3f(1, 1, 1);
      //glTranslated(-1.45, 1.2, 1.34);
      glPushMatrix();
        glTranslated(-1.45, 1.2, 1.34);
        glScaled(0.25, 0.25, 4.6);
        glutSolidCube(0.3);
      glPopMatrix();
      glPushMatrix();
        glTranslated(-1.45, -0.1, 1.34);
        glScaled(0.25, 0.25, 4.6);
        glutSolidCube(0.3);
      glPopMatrix();
      glPushMatrix();
        glTranslated(-1.45, 0.55, 1.34);
        glScaled(0.25, 4.6, 0.25);
        glutSolidCube(0.3);
      glPopMatrix();

      glPushMatrix();
        glTranslated(-1.45, 0.55, 2.0);
        glScaled(0.25, 4.6, 0.25);
        glutSolidCube(0.3);
      glPopMatrix();

      glPushMatrix();
        glTranslated(-1.45, 0.55, 0.67);
        glScaled(0.25, 4.6, 0.25);
        glutSolidCube(0.3);
      glPopMatrix();

    glPopMatrix();

    //梳妆台
    glPushMatrix();
      glTranslated(-1.0, -0.28, 1.3);
      glScaled(0.8, 0.8, 0.8);

      //桌子面
      glPushMatrix();
        glRotated(90,1,0,0);
        glRotated(90,0,0,1);
        glScalef (2.0, 1.4, 0.1);
        glColor3f( 0, 1, 0 );
        glutSolidCube(0.8);

      glPopMatrix();
      //glRotated(-90,0,1,0);
      //桌子腿1
      glPushMatrix();
        glRotated(-90,0,1,0);
        glTranslated(-0.6, -0.5, 0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(0.8);
      glPopMatrix();
      //桌子腿2
      glPushMatrix();

        glRotated(-90,0,1,0);
        glTranslated(-0.6, -0.5, -0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(0.8);
      glPopMatrix();
      //桌子腿3
      glPushMatrix();
        glRotated(-90,0,1,0);
        glTranslated(0.6, -0.5, 0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(0.8);
      glPopMatrix();
//桌子腿4
      glPushMatrix();
        glRotated(-90,0,1,0);
        glTranslated(0.6, -0.5, -0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.1, 1.2, 0.1);
        glutSolidCube(0.8);
      glPopMatrix();

      glTranslated(0.5, 0, 0.2);
     //凳子腿1
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-90,0,1,0);
        glRotated(-180,0,1,0);
        glTranslated(0.23, 0.0, -0.18);
        glTranslated(-0.6, -0.5, 0.5);
        glColor3f( 0, 1, 0 );
        glScalef(0.2, 1.5, 0.2);
        glutSolidCube(0.4);
      glPopMatrix();
//凳子腿2
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-90,0,1,0);
        glRotated(-180,0,1,0);
        glTranslated(0.23, 0.0, 0.18);
        glTranslated(-0.6, -0.5, -0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.2, 1.5, 0.2);
        glutSolidCube(0.4);
      glPopMatrix();
//凳子腿3
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-90,0,1,0);
        glRotated(-180,0,1,0);
        glTranslated(-0.46, 0.0, -0.2);
        glTranslated(0.6, -0.5, 0.5);
        glColor3f( 0, 1, 0 );
        glScalef(0.2, 1.5, 0.2);
        glutSolidCube(0.4);
      glPopMatrix();
//凳子腿4
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-90,0,1,0);
        glRotated(-180,0,1,0);
        glTranslated(-0.46, 0.0, 0.19);
        glTranslated(0.6, -0.5, -0.4);
        glColor3f( 0, 1, 0 );
        glScalef(0.2, 1.5, 0.2);
        glutSolidCube(0.4);
      glPopMatrix();
//凳子面
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-180,0,1,0);
        glRotated(90,1,0,0);
        glRotated(90,0,0,1);
        glTranslated(-0.1, 0, 0.2);
        glColor3f( 0, 1, 0 );
        glScalef (0.8, 0.8, 0.1);
    //glColor3f( 1, 1, 0 );
        glutSolidCube(0.8);

      glPopMatrix();
//椅背
      glPushMatrix();
        glTranslated(0, -0.2, -0.5);
        glRotated(-180,0,1,0);
        glRotated(-90,0,1,0);
        glTranslated(-0.1, 0.08, 0.36);
        glColor3f( 0, 1, 0 );
        glScalef (0.8, 0.8, 0.1);
        glutSolidCube(0.8);
      glPopMatrix();

    glPopMatrix();


     //台灯底座及灯罩
     glPushMatrix();
        glTranslated(-0.6, -0.45, -0.85);
        glColor3f(1, 1, 1);
        glutSolidCube(0.2);
        glTranslated(0, 0.27, 0);
        glScaled(0.2, 0.2, 0.2);
        glutWireIcosahedron();
        GLfloat mat_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat mat_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
     glPopMatrix();

     //衣柜
     glPushMatrix();
       //glTranslated(-1.2, -1.0, -0.6);
       glPushMatrix();
         glTranslated(-1.2, 0.9, -0.42);
         glScaled(1.8, 0.2, 4.8);
         glutSolidCube(0.3);
       glPopMatrix();
       glPushMatrix();
         glTranslated(-1.2, -0.1, -1.12);
         glScaled(1.8, 6.6, 0.2);
         glutSolidCube(0.3);

       glPopMatrix();
       glPushMatrix();
         glTranslated(-1.2, -0.1, 0.27);
         glScaled(1.8, 6.6, 0.2);
         glutSolidCube(0.3);
       glPopMatrix();

       //右柜门
       glPushMatrix();
         glTranslated(-0.9, -0.1, -0.75);
         glScaled(0.2, 6.6, 2.2);
         glutSolidCube(0.3);
       glPopMatrix();

       //左柜门
       glPushMatrix();
         glColor3f(0, 0, 1);
         glTranslated(-0.9, -0.1, -0.1);
         glScaled(0.2, 6.6, 2.2);
         glutSolidCube(0.3);
       glPopMatrix();

       glPushMatrix();
         glColor3f(0, 0, 0);
         glTranslated(-0.8, 0.0, -0.5);
         glScaled(0.5, 0.5, 0.5);
         glutSolidSphere(0.1, 60, 60);
       glPopMatrix();

       glPushMatrix();
         glColor3f(0, 0, 0);
         glTranslated(-0.8, 0.0, -0.3);
         glScaled(0.5, 0.5, 0.5);
         glutSolidSphere(0.1, 60, 60);
       glPopMatrix();
     glPopMatrix();

     //笔记本电脑
     glPushMatrix();

       glColor3f(0.996, 0.83, 0);
       glPushMatrix();
         glTranslated(-1.3, -0.08, 1.1);
         glRotated(15, 0, 0, 1);
         glScaled(0.1, 1.0, 1.5);
         glutSolidCube(0.3);
         glTranslated(0.22, 0, 0);
         glColor3f(0, 0, 0);
         glutSolidCube(0.27);
       glPopMatrix();
       glPushMatrix();
         glColor3f(0.996, 0.83, 0);
         glTranslated(-1.15, -0.22, 1.1);
         glScaled(1.0,0.1,  1.5);
         glutSolidCube(0.3);
       glPopMatrix();
     glPopMatrix();

     //衣架主体
     glPushMatrix();
     glTranslated(0.8, 0, -0.8);
     glPushMatrix();
       glTranslated(1.2, -0.1, 0.2);
       glColor3f(1, 0, 0);
       glScaled(0.3, 3.0, 0.3);
       R1 = 0.6;
       R2 = 0.6;
       glutSolidSphere(0.3, 60, 60);
       glTranslated(-0.1, -0.3, 0.15);
       glBegin(GL_QUAD_STRIP);
          for(int i=0;i<n;i=i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
             glVertex3f(R2*cos(2*PI/n*i)+0.1, 0.03, R2*sin(2*PI/n*i)-0.1);
          }
       glEnd();
       glBegin(GL_POLYGON);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
             glVertex3f(R2*cos(2*PI/n*i)+0.1, 0.03, R1*sin(2*PI/n*i)-0.1);
          }
       glEnd();
       glColor3f(0, 0, 0);
       glBegin(GL_LINE_STRIP);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1,0, R1*sin(2*PI/n*i)-0.1);

          }
       glEnd();
       glBegin(GL_LINE_STRIP);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1,0.03, R1*sin(2*PI/n*i)-0.1);
          }
       glEnd();
     glPopMatrix();

     glPushMatrix();
       glTranslated(1.07, 0.4, 0.2);
       glRotated(30, 0, 0, 1);
       glColor3f(1, 0, 0);
       glScaled(0.2, 1.2, 0.2);
       glutSolidSphere(0.2, 60, 60);
       glTranslated(-1.3, -0.4, 0);
       glutSolidSphere(0.2, 60, 60);
     glPopMatrix();

     glPushMatrix();
       glTranslated(1.27, 0.4, 0.2);
       glRotated(-30, 0, 0, 1);
       glColor3f(1, 0, 0);
       glScaled(0.2, 1.2, 0.2);
       glutSolidSphere(0.2, 60, 60);
       glTranslated(1.5, -0.4, 0);
       glutSolidSphere(0.2, 60, 60);
     glPopMatrix();

     glPopMatrix();

     //投影仪
     glPushMatrix();
       glColor3f(1, 1, 1);
       glTranslated(0.4, 1.5, -1.1);
       glScaled(5, 0.4, 0.4);
       glutSolidCube(0.3);
     glPopMatrix();

     if(projector == 1){
        Draw3D3();
     }

     glPushMatrix();
       glColor3f(0, 1, 1);
       glRotated(90, 1, 0, 0);
       glTranslated(0.5, 1.4, 0.8);
       glutSolidTorus(0.15, 0.25, 60, 60);
     glPopMatrix();
glPopMatrix();

glPushMatrix();
    glTranslated(xPosition_person,yPosition_person+0.1,zPosition_person-1);
//    glTranslated(xPosition_person,yPosition_person+0.1,zPosition_person-5)
    glTranslated(0, 0, zPosition_person);
    //glRotated(backRot, 0, 1, 0);
    glRotated(xRot,1,0,0);
    glRotated(yRot,0,1,0);
    glRotated(zRot,0,0,1);
    glTranslated(1.5, 0.4, 1.5);
 // glEnable(GL_LIGHTING);
  //glRotated(90, 0, 1, 0);
  glColor3f(1, 0, 0);
  glRotated(backRot, 0, 1, 0);
  //头和脖子
  glScaled(0.5, 0.5, 0.5);
  glPushMatrix();
    glutSolidCube(0.5);
    glTranslated(0, -0.2, 0);
    glColor3f(1, 1, 1);
    glScaled(1, 2.2, 1);
    glutSolidCube(0.2);
  glPopMatrix();

  glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(0.13, 0.08, 0.26);
    glScaled(0.5, 0.5, 0.1);
    glutSolidCube(0.2);
  glPopMatrix();

  glPushMatrix();
    glTranslated(0, 0.08, 0.26);
    glScaled(1.0, 0.1, 0.1);
    glutSolidCube(0.2);
  glPopMatrix();

  glPushMatrix();
    glTranslated(0, -0.12, 0.26);
    glScaled(1.0, 0.3, 0.1);
    glutSolidCube(0.2);
  glPopMatrix();

  glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(-0.12, 0.08, 0.26);
    glScaled(0.5, 0.5, 0.1);
    glutSolidCube(0.2);
  glPopMatrix();
  //肩膀
  glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(0, -0.50, 0);
    glScaled(1.8, 0.4, 0.4);
    glutSolidCube(0.6);
  glPopMatrix();

  //胳膊和身子
  glPushMatrix();
    glTranslated(-0.4, -1.05, 0);
    glScaled(0.3, 1.4, 0.3);
    glutSolidCube(0.6);
    glTranslated(2.7, 0, 0);
    glutSolidCube(0.6);
    glTranslated(-1.38, 0, 0);
    glColor3f(1, 0, 0);
    glScaled(3.3, 1.2, 1.2);
    glutSolidCube(0.6);
  glPopMatrix();

//  //左腿
  glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(-0.18, -1.8, 0);
    glRotated(legL, 1, 0, 0);
    glScaled(0.34, 1.4, 0.34);
    glutSolidCube(0.6);
  glPopMatrix();
  //右腿
  glPushMatrix();
    glTranslated(0.18, -1.8, 0);
    glRotated(legR, 1, 0, 0);
    glScaled(0.34, 1.4, 0.34);
    glutSolidCube(0.6);
  glPopMatrix();

glPopMatrix();
SwapBuffers(hDC);
glClearColor(0,0,0,1);
glEnable(GL_DEPTH_TEST);

GLfloat light0_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light0_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light0_position[] = { 5.0f, 5.0f, 1.0f, 0.0f };

GLfloat light2_diffuse[] = {1.0f,1.0f,1.0f,1.0f};
//GLfloat light2_ambient[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat light2_ambient[] = {1.0f,0.5f,0.5f,1.0f};

GLfloat light1_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light1_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light1_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light1_position[] = { 2.0f, -1.0f, 5.0f, 0.0f };

GLfloat spot_direction[] = {0.0, -1.0, 0.0};

GLfloat mat_specular[]   = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat high_shininess[] = { 100.0f };
//GLfloat spot_direction[]=  { 1.0f, 1.0f, -1.0f};

glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
//glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);

glEnable(GL_COLOR_MATERIAL);
glEnable(GL_LIGHT0);
glEnable(GL_LIGHT1);
glEnable(GL_LIGHT2);
glEnable(GL_LIGHTING);

SetupRC();

                           // 交换缓存(双缓存)
} //对图形进行绘制(渲染)事件

GLvoid Draw3D3()
{
    glPushMatrix();
       glColor3f(1, 1, 1);
       glBegin(GL_POLYGON);
         glVertex3f(-0.3, 0.3, -1.15);
         glVertex3f(-0.3, 1.6, -1.15);
         glVertex3f(1.1, 1.6, -1.15);
         glVertex3f(1.1,0.3, -1.15);
       glEnd();
     glPopMatrix();
}

GLvoid Draw3D2()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
gluLookAt(xlookAt, ylookAt, zlookAt,0, 0, 0, 0, 0, -1);
GLfloat light2_position[] = { -0.57f, 0.36f , 0.3f, 1.0f };

//glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
//glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
//glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslated(0, 0, -1);
    glTranslated(xPosition,yPosition,zPosition+1);
    glRotated(xRot,1,0,0);
    glRotated(yRot,0,1,0);
    glRotated(zRot,0,0,1);
    glTranslated(0, -0.1, 0);
glPushMatrix();
    //灯球
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glTranslated(-0.57f, 0.36f , 0.3f);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(0.05f, 50.0f, 50.0f);
	//glEnable(GL_LIGHTING);
glPopMatrix();
glColor3f(0, 1, 0 );
glEnable(GL_LIGHTING);
    //桌子腿1
glPushMatrix();
    glRotated(-30,0,1,0);
    glTranslated(-0.6, -0.5, 0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.1, 1.2, 0.1);
    glutSolidCube(0.8);
glPopMatrix();
//桌子腿2
glPushMatrix();

    glRotated(-30,0,1,0);
    glTranslated(-0.6, -0.5, -0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.1, 1.2, 0.1);
    glutSolidCube(0.8);
glPopMatrix();
//桌子腿3
glPushMatrix();
    glRotated(-30,0,1,0);
    glTranslated(0.6, -0.5, 0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.1, 1.2, 0.1);
    glutSolidCube(0.8);
glPopMatrix();
//桌子腿4
glPushMatrix();
    glRotated(-30,0,1,0);
    //glRotated(10,1,0,0);
    glTranslated(0.6, -0.5, -0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.1, 1.2, 0.1);
    glutSolidCube(0.8);
glPopMatrix();

//凳子腿1
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-30,0,1,0);
    glRotated(-180,0,1,0);
    glTranslated(0.23, 0.0, -0.18);
    glTranslated(-0.6, -0.5, 0.5);
    glColor3f( 0, 1, 0 );
    glScalef(0.2, 1.5, 0.2);
    glutSolidCube(0.4);
glPopMatrix();
//凳子腿2
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-30,0,1,0);
    glRotated(-180,0,1,0);
    glTranslated(0.23, 0.0, 0.18);
    glTranslated(-0.6, -0.5, -0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.2, 1.5, 0.2);
    glutSolidCube(0.4);
glPopMatrix();
//凳子腿3
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-30,0,1,0);
    glRotated(-180,0,1,0);
    glTranslated(-0.46, 0.0, -0.2);
    glTranslated(0.6, -0.5, 0.5);
    glColor3f( 0, 1, 0 );
    glScalef(0.2, 1.5, 0.2);
    glutSolidCube(0.4);
glPopMatrix();
//凳子腿4
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-30,0,1,0);
    glRotated(-180,0,1,0);
    glTranslated(-0.46, 0.0, 0.19);
    glTranslated(0.6, -0.5, -0.4);
    glColor3f( 0, 1, 0 );
    glScalef(0.2, 1.5, 0.2);
    glutSolidCube(0.4);
glPopMatrix();
//凳子面
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-180,0,1,0);
    glRotated(90,1,0,0);
    glRotated(30,0,0,1);
    glTranslated(-0.1, 0, 0.2);
    glColor3f( 0, 1, 0 );
    glScalef (0.8, 0.8, 0.1);
    //glColor3f( 1, 1, 0 );
    glutSolidCube(0.8);

glPopMatrix();
//椅背
glPushMatrix();
    glTranslated(0, -0.2, -0.5);
    glRotated(-180,0,1,0);
    glRotated(-30,0,1,0);
    glTranslated(-0.1, 0.08, 0.36);
    glColor3f( 0, 1, 0 );
    glScalef (0.8, 0.8, 0.1);
    glutSolidCube(0.8);

glPopMatrix();
//茶壶及托盘
glPushMatrix();
    glTranslated(0, 0.05, 0.5);
    R1 = 0.2;
    R2 = 0.4;
    glColor3f( 0, 1, 1 );
    glScaled(0.5, 0.5, 0.5);
    glBegin(GL_QUAD_STRIP);
        for(int i=0;i<n;i=i++){
           glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
           glVertex3f(R2*cos(2*PI/n*i)+0.1, 0.1, R2*sin(2*PI/n*i)-0.1);
        }
    glEnd();
    glBegin(GL_POLYGON);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
          }
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1,0, R1*sin(2*PI/n*i)-0.1);
          }
    glEnd();
    glTranslated(0, -0.05, -0.35);
    glTranslated(0.07, 0.2, 0.3);
    glColor3f( 0, 0, 1 );
    glutSolidTeapot(0.2);

glPopMatrix();

//台灯底座及灯罩
glPushMatrix();
    glTranslated(-0.6, 0.15, 0.2);
    glColor3f(1, 1, 1);
    glutSolidCube(0.2);
    glTranslated(0, 0.27, 0);
    glScaled(0.2, 0.2, 0.2);
    glutWireIcosahedron();
    GLfloat mat_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
glPopMatrix();

//衣架主体
glPushMatrix();
    glTranslated(1.2, -0.1, 0.2);
    glColor3f(1, 0, 0);
    glScaled(0.3, 3.0, 0.3);
    R1 = 0.6;
    R2 = 0.6;
    glutSolidSphere(0.3, 60, 60);
    glTranslated(-0.1, -0.3, 0.15);
    glBegin(GL_QUAD_STRIP);
        for(int i=0;i<n;i=i++){
           glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
           glVertex3f(R2*cos(2*PI/n*i)+0.1, 0.03, R2*sin(2*PI/n*i)-0.1);
        }
    glEnd();
    glBegin(GL_POLYGON);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1, 0, R1*sin(2*PI/n*i)-0.1);
             glVertex3f(R2*cos(2*PI/n*i)+0.1, 0.03, R1*sin(2*PI/n*i)-0.1);
          }
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1,0, R1*sin(2*PI/n*i)-0.1);

          }
    glEnd();
    glBegin(GL_LINE_STRIP);
          for(int i=0;i<n;i++){
             glVertex3f(R1*cos(2*PI/n*i)+0.1,0.03, R1*sin(2*PI/n*i)-0.1);
          }
    glEnd();
glPopMatrix();

glPushMatrix();
    glTranslated(1.07, 0.4, 0.2);
    glRotated(30, 0, 0, 1);
    glColor3f(1, 0, 0);
    glScaled(0.2, 1.2, 0.2);
    glutSolidSphere(0.2, 60, 60);
    glTranslated(-1.3, -0.4, 0);
    glutSolidSphere(0.2, 60, 60);

glPopMatrix();

glPushMatrix();
    glTranslated(1.27, 0.4, 0.2);
    glRotated(-30, 0, 0, 1);
    glColor3f(1, 0, 0);
    glScaled(0.2, 1.2, 0.2);
    glutSolidSphere(0.2, 60, 60);
    glTranslated(1.5, -0.4, 0);
    glutSolidSphere(0.2, 60, 60);

glPopMatrix();
glPopMatrix();

//桌子面及墙壁地板的纹理
glPushMatrix();
    //glDisable(GL_TEXTURE_2D);
    glTranslated(0, 0, -1);
    glTranslated(xPosition,yPosition,zPosition+1);
    glRotated(xRot,1,0,0);
    glRotated(yRot,0,1,0);
    glRotated(zRot,0,0,1);
    glTranslated(0, -0.12, 0);
    glRotated(90,1,0,0);
    glRotated(30,0,0,1);
    glPushMatrix();
        glRotated(-90, 1, 0, 0);
        glColor3f(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texShip);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 3.5, -1.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 3.5, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5,-1.0, -1.2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, texSea);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.3, -1.18);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 1.5, -1.18);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5, 1.5, -1.18);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5,0.3, -1.18);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, texShip);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, 2.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 3.5, 2.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5, 3.5, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, texFloor);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, 2.2);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, -1.0, 2.2);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5,-1.0, -1.2);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, -1.0, -1.2);
        glEnd();


        glBindTexture(GL_TEXTURE_2D, texDoor);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.45, -1.0, 1.8);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.45, 2.2, 1.8);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.45, 2.2, 0.3);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.45, -1.0, 0.3);
        glEnd();
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glScalef (2.0, 1.4, 0.1);
    glColor3f( 0, 1, 0 );
    glutSolidCube(0.8);

glPopMatrix();

SwapBuffers(hDC);                              // 交换缓存(双缓存)

glClearColor(0,0,0,1);
glEnable(GL_DEPTH_TEST);

GLfloat light0_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light0_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light0_position[] = { -5.0f, 5.0f, 1.0f, 0.0f };

GLfloat light2_diffuse[] = {1.0f,1.0f,1.0f,1.0f};
//GLfloat light2_ambient[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat light2_ambient[] = {1.0f,0.5f,0.5f,1.0f};


GLfloat light1_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat light1_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light1_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat light1_position[] = { -5.0f, -3.0f, -10.0f, 0.0f };

GLfloat spot_direction[] = {0.0, -1.0, 0.0};

GLfloat mat_specular[]   = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat high_shininess[] = { 100.0f };
//GLfloat spot_direction[]=  { 1.0f, 1.0f, -1.0f};

glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

//glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
//glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
//glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
//glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_diffuse);
glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
//glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);

glEnable(GL_COLOR_MATERIAL);
glEnable(GL_LIGHT0);
//glEnable(GL_LIGHT1);
glEnable(GL_LIGHT2);
glEnable(GL_LIGHTING);

SetupRC();
} //对图形进行绘制(渲染)事件
void SetupRC()
{
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
    texFloor = load_texture("data\\floor0.bmp");  //加载纹理
    texShip = load_texture("data\\wall1.bmp");
    texSea = load_texture("data\\ocean.bmp");
    texCarpet = load_texture("data\\carpet.bmp");
    texDoor = load_texture("data\\door.bmp");
    texSky = load_texture("data\\sky.bmp");

}

//********************************************************************
//*  键盘控制图形事件定义                                            *
//********************************************************************
//
GLuint KeyPress()
{
  if (keys[VK_UP])                                   // 按上箭头键
    xRot -= rotStep;

  if (keys[VK_DOWN])                                 // 按下箭头键
    xRot += rotStep;

  if (keys[VK_RIGHT]){                               // 按右箭头键
    yRot += rotStep;
  }

  if (keys[VK_LEFT])                                 // 按左箭头键
    yRot -= rotStep;

  if (keys[VK_HOME])                                 // 按HOME键
    zRot += rotStep;

  if (keys[VK_END])                                  // 按END键
    zRot -= rotStep;

  if (keys['A'])                                     // 按A键
    xPosition -= moveStep;

  if (keys['D'])                                     // 按D键
    xPosition += moveStep;

  if (keys['W'])                                     // 按W键
    yPosition += moveStep;

  if (keys['S'])                                     // 按S键
    yPosition -= moveStep;

  if (keys['Q'])                                     // 按Q键
    zPosition -= moveStep;

  if (keys['E'])                                     // 按E键
    zPosition += moveStep;

  if (keys[VK_ESCAPE])                               // 按ESC键
    SendMessage(hWnd,WM_DESTROY,0,0);

  return 0;
} // 键盘控制图形事件定义

//********************************************************************
//*  计时器函数定义                                          *
//********************************************************************
//
GLvoid CALLBACK timerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{

    glEnable(GL_TEXTURE_2D);
    //前进
    if(zPosition_person <= 0.9 && !flag2){
        if(legR < 0 && legL > 0){
            legR += paceStep;
            legL -= paceStep;
        }
        else{
            legR -= paceStep;
            legL += paceStep;
        }
        zPosition_person += moveStep;
    }
    if(zPosition_person > 0.9 && !flag3){
        backRot += 180.0;
        flag3 = true;
        flag2 = true;
    }
    //后退
    if(zPosition_person >= 0 && flag2){
        if(legR < 0 && legL > 0){
            legR += paceStep;
            legL -= paceStep;
        }
        else{
            legR -= paceStep;
            legL += paceStep;
        }
        zPosition_person -= moveStep;
    }
    if(zPosition_person < 0 && flag3){
        backRot -= 180.0;
        flag3 = false;
        flag2 = false;
    }
    SendMessage(hWnd, WM_PAINT, 0, 0L);
}

/********************************************************************/
/*  主窗体菜单定义函数定义                                          */
/********************************************************************/
//
GLuint MainWndMenu()
{
  hMenuMain = CreateMenu();
  hPopup = CreatePopupMenu();
  AppendMenu(hPopup,MF_STRING,IDM_AUTO,TEXT("Walk"));
  AppendMenu(hPopup,MF_STRING,IDM_STOP,TEXT("Stop"));
  AppendMenu(hPopup,MF_SEPARATOR,0,NULL);
  AppendMenu(hPopup,MF_STRING,IDM_BLACK,TEXT("Black"));
  AppendMenu(hPopup,MF_STRING,IDM_GRAY,TEXT("Gray"));
   AppendMenu(hPopup,MF_STRING,IDM_SKYBLUE,TEXT("SkyBlue"));
  AppendMenu(hPopup,MF_STRING,IDM_WHITE,TEXT("White"));

  AppendMenu(hMenuMain,MF_POPUP,(int)hPopup,TEXT("CONTROL"));

  return 0;
} // 主窗体菜单定义函数

/********************************************************************/
/*  建立窗体控件函数定义                                            */
/********************************************************************/
int commandBtns(HWND hWndParent)
{
  int diff = 1210;
  hCmdBtns = CreateWindowEx((DWORD)NULL,
                               "BUTTON",
                               "",
                               WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
                               diff+0,0,180,600,
                               hWndParent,
                               (HMENU)IDC_FRAME,
                               hInstance,
                               NULL);
  if(!hCmdBtns) return 1;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "x-",
                       WS_CHILD|WS_VISIBLE,
                       diff+20,100,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateX1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 2;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "x+",
                       WS_CHILD|WS_VISIBLE,
                       diff+120,100,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateX2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 3;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "y+",
                       WS_CHILD|WS_VISIBLE,
                       diff+72,60,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateY1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 4;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "y-",
                       WS_CHILD|WS_VISIBLE,
                       diff+72,140,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateY2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 5;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "z+",
                       WS_CHILD|WS_VISIBLE,
                       diff+120,60,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateZ1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 6;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "z-",
                       WS_CHILD|WS_VISIBLE,
                       diff+20,140,42,32,
                       hWndParent,
                       (HMENU)IDC_RotateZ2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 7;

//  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
//                       "BUTTON",
//                       "Room2",
//                       WS_CHILD|WS_VISIBLE,
//                       diff+100,180,60,32,
//                       hWndParent,
//                       (HMENU)IDC_Room2,
//                       hInstance,
//                       NULL);
//  if(!hCmdBtns) return 17;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "Change",
                       WS_CHILD|WS_VISIBLE,
                       diff+60,180,60,32,
                       hWndParent,
                       (HMENU)IDC_Room1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 16;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "OFF",
                       WS_CHILD|WS_VISIBLE,
                       diff+100,220,60,32,
                       hWndParent,
                       (HMENU)IDC_OFF,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 18;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "ON",
                       WS_CHILD|WS_VISIBLE,
                       diff+20,220,60,32,
                       hWndParent,
                       (HMENU)IDC_ON,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 19;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建标签控件
                       "STATIC",
                       "LootAt Control:",
                       WS_CHILD|WS_VISIBLE,
                       diff+8,20,120,32,
                       hWndParent,
                       (HMENU)IDC_RotateZ2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 8;

    hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建标签控件
                       "STATIC",
                       "Translated Control:",
                       WS_CHILD|WS_VISIBLE,
                       diff+8,260,160,32,
                       hWndParent,
                       (HMENU)IDC_RotateZ2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 9;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "<<",
                       WS_CHILD|WS_VISIBLE,
                       diff+20,340,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateX1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 10;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       ">>",
                       WS_CHILD|WS_VISIBLE,
                       diff+120,340,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateX2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 11;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       TEXT("UP"),
                       WS_CHILD|WS_VISIBLE,
                       diff+72,300,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateY1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 12;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "DN",
                       WS_CHILD|WS_VISIBLE,
                       diff+72,380,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateY2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 13;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "ZI",
                       WS_CHILD|WS_VISIBLE,
                       diff+120,300,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateZ1,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 14;

  hCmdBtns = CreateWindowEx((DWORD)NULL,                 // 创建按钮控件
                       "BUTTON",
                       "ZO",
                       WS_CHILD|WS_VISIBLE,
                       diff+20,300,42,32,
                       hWndParent,
                       (HMENU)IDC_TranslateZ2,
                       hInstance,
                       NULL);
  if(!hCmdBtns) return 15;

  return 0;
}

// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLuint last_texture_ID=0, texture_ID = 0;

    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }

    // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 若图像宽高超过了OpenGL规定的最大值，也缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width) || !power_of_two(height) || width > max || height > max )
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            // 进行像素缩放
            gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    GLint lastTextureID=last_texture_ID;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,0x80E0, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
    free(pixels);
    return texture_ID;
}

