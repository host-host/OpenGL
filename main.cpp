#include<windows.h>
#include<gl/gl.h>
#include<cmath>
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	switch(message) {
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			PostQuitMessage (0);
			return 0;
		case WM_DESTROY:
			return 0;
		case WM_KEYDOWN:
			switch(wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
			return 0;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}
}
void EnableOpenGL(HWND hWnd,HDC *hDC,HGLRC *hRC) {
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;
	*hDC=GetDC(hWnd);
	ZeroMemory(&pfd,sizeof(pfd));
	pfd.nSize=sizeof(pfd);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=24;
	pfd.cDepthBits=16;
	pfd.iLayerType=PFD_MAIN_PLANE;
	iFormat=ChoosePixelFormat(*hDC,&pfd);
	SetPixelFormat(*hDC,iFormat,&pfd);
	*hRC=wglCreateContext(*hDC);
	wglMakeCurrent(*hDC,*hRC);
}
void DisableOpenGL(HWND hWnd,HDC hDC,HGLRC hRC) {
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd,hDC);
}
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow) {
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	wc.style=CS_OWNDC;
	wc.lpfnWndProc=WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName="OpenGL";
	RegisterClass(&wc);
	hWnd=CreateWindow("OpenGL","OpenGL",WS_CAPTION|WS_POPUPWINDOW|WS_VISIBLE,0,0,256,256,
	                  NULL,NULL,hInstance,NULL);
	EnableOpenGL(hWnd,&hDC,&hRC);
	float theta=3;
	while(1) {
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if(msg.message==WM_QUIT)break;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			continue;
		}
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 2.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LEQUAL);
		glClear(GL_COLOR_BUFFER_BIT);
		if(KEY_DOWN(VK_LEFT))glRotatef(theta,0,0.5f,0);
		if(KEY_DOWN(VK_RIGHT))glRotatef(theta,0,-0.5f,0);
		if(KEY_DOWN(VK_UP))glRotatef(theta,0.5,0,0);
		if(KEY_DOWN(VK_DOWN))glRotatef(theta,-0.5,0,0);
		if(KEY_DOWN(VK_LBUTTON))glRotatef(theta,0,0,0.5);
		if(KEY_DOWN(VK_RBUTTON))glRotatef(theta,0,0,-0.5);
		glBegin(GL_QUADS);
		glColor3f(0,1,0);//1
		glVertex3f(0.5,0.5,0.5);
		glVertex3f(0.5,-0.5,0.5);
		glVertex3f(0.5,-0.5,-0.5);
		glVertex3f(0.5,0.5,-0.5);
		glColor3f(1,0,0);//2
		glVertex3f(-0.5,0.5,0.5);
		glVertex3f(-0.5,-0.5,0.5);
		glVertex3f(-0.5,-0.5,-0.5);
		glVertex3f(-0.5,0.5,-0.5);
		glColor3f(1,0,1);//3
		glVertex3f(0.5,0.5,0.5);
		glVertex3f(0.5,0.5,-0.5);
		glVertex3f(-0.5,0.5,-0.5);
		glVertex3f(-0.5,0.5,0.5);
		glColor3f(1,1,0);//4
		glVertex3f(0.5,-0.5,0.5);
		glVertex3f(0.5,-0.5,-0.5);
		glVertex3f(-0.5,-0.5,-0.5);
		glVertex3f(-0.5,-0.5,0.5);
		glColor3f(0,1,1);//5
		glVertex3f(0.5,-0.5,-0.5);
		glVertex3f(0.5,0.5,-0.5);
		glVertex3f(-0.5,0.5,-0.5);
		glVertex3f(-0.5,-0.5,-0.5);
		glColor3f(0,0,1);//6
		glVertex3f(0.5,-0.5,0.5);
		glVertex3f(0.5,0.5,0.5);
		glVertex3f(-0.5,0.5,0.5);
		glVertex3f(-0.5,-0.5,0.5);
		glEnd();
		SwapBuffers(hDC);
		Sleep(10);
	}
	DisableOpenGL(hWnd,hDC,hRC);
	DestroyWindow(hWnd);
	return msg.wParam;
}
