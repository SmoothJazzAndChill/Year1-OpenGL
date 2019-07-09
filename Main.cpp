

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// CT4TOGA OpenGL without using GLUT - uses excerpts from here:
// http://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
// Feel free to adapt this for what you need, but please leave these comments in.

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#pragma once

#define GLEW_STATIC
#include <gl\glew.h>
#include <windows.h>	// need this file if you want to create windows etc
#include <gl\gl.h>		// need this file to do graphics with opengl
#include <gl\glu.h>		// need this file to set up a perspective projection easily
#include "Libs\glext.h"
#include "gl\SOIL.h"
#include <queue>

#include "Game.h"
#include "EventHandler.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// function prototypes:
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow);


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// In a C++ Windows app, the starting point is WinMain() rather than _tmain() or main().
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	// some basic numbers to hold the position and size of the window
#define WIDTH		1920
#define HEIGHT		1080
#define TOPLEFTX	0
#define TOPLEFTY	0
#define TITLE		"Simon Newman Coursework"

	// this bit creates a window class, basically a template for the window we will make later, so we can do more windows that look the same.
	WNDCLASS myWindowClass;
	myWindowClass.cbClsExtra = 0;											// Allocate extra bytes following the structure
	myWindowClass.cbWndExtra = 0;											// Allocate extra bytes following the window instance
	myWindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);		// background fill black
	myWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// arrow cursor       
	myWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// type of icon to use (default app icon)
	myWindowClass.hInstance = hInstance;									// window instance name (given by the OS when the window is created)   
	myWindowClass.lpfnWndProc = WndProc;									// window callback function - this is our function below that is called whenever something happens
	myWindowClass.lpszClassName = TEXT("GameWindow");						// our new window class name
	myWindowClass.lpszMenuName = 0;											// window menu name (0 = default menu?) 
	myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// redraw if the window is resized horizontally or vertically, allow different context for each window instance

	// Register that class with the Windows OS
	RegisterClass(&myWindowClass);


	// create a rect structure to hold the dimensions of our window
	RECT rect;
	SetRect(&rect,	TOPLEFTX,				// the top-left corner x-coordinate
					TOPLEFTY,				// the top-left corner y-coordinate
					TOPLEFTX + WIDTH,		// far right
					TOPLEFTY + HEIGHT);		// far left


	// adjust the window, no idea why.
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);



	// call CreateWindow to create the window
	HWND myWindow = CreateWindow(TEXT("GameWindow"),					// Window class to use - in this case the one we created a minute ago
					TEXT(TITLE),										// Window Title
					WS_OVERLAPPEDWINDOW,								// Window Style - How the window looks and functionality of the window
					TOPLEFTX, TOPLEFTY,									// Window Position (X, Y)
					TOPLEFTX + WIDTH, TOPLEFTY + HEIGHT,				// Window Size
					NULL, NULL,											// Parent Window, Something to do with menus
					hInstance, NULL);									// ??


	// check to see that the window created okay
	if (myWindow == NULL)
	{
		FatalAppExit(NULL, TEXT("CreateWindow() failed!"));
	}

	// if so, show it
	ShowWindow(myWindow, SW_SHOWMAXIMIZED);


	// get a device context from the window
	HDC myDeviceContext = GetDC(myWindow);


	// we create a pixel format descriptor, to describe our desired pixel format. 
	// we set all of the fields to 0 before we do anything else
	// this is because PIXELFORMATDESCRIPTOR has loads of fields that we won't use
	PIXELFORMATDESCRIPTOR myPfd = { 0 };


	// now set only the fields of the pfd we care about:
	myPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);		// size of the pfd in memory
	myPfd.nVersion = 1;									// always 1

	myPfd.dwFlags = PFD_SUPPORT_OPENGL	|				// OpenGL support - not DirectDraw
					PFD_DOUBLEBUFFER	|				// double buffering support
					PFD_DRAW_TO_WINDOW;					// draw to the app window, not to a bitmap image (for example)

	myPfd.iPixelType = PFD_TYPE_RGBA;					// red, green, blue, alpha for each pixel
	myPfd.cColorBits = 24;								// 24 bit == 8 bits for red, 8 for green, 8 for blue.
														// This count of color bits EXCLUDES alpha.

	myPfd.cDepthBits = 32;								// 32 bits to measure pixel depth.



	// now we need to choose the closest pixel format to the one we wanted...	
	int chosenPixelFormat = ChoosePixelFormat(myDeviceContext, &myPfd);

	// if windows didnt have a suitable format, 0 would have been returned...
	if (chosenPixelFormat == 0)
	{
		FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
	}

	// if we get this far it means we've got a valid pixel format
	// so now we need to set the device context up with that format...
	int result = SetPixelFormat(myDeviceContext, chosenPixelFormat, &myPfd);

	// if it failed...
	if (result == NULL)
	{
		FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
	}


	// we now need to set up a render context (for opengl) that is compatible with the device context (from windows)...
	HGLRC myRenderContext = wglCreateContext(myDeviceContext);

	// then we connect the two together
	wglMakeCurrent(myDeviceContext, myRenderContext);


	// opengl display setup
	glMatrixMode(GL_PROJECTION);								// select the projection matrix, i.e. the one that controls the "camera"
	glLoadIdentity();											// reset it
	gluPerspective(45.0, (float)WIDTH / (float)HEIGHT, 0.01, 1000);	// set up fov, and near / far clipping planes
	glViewport(0, 0, WIDTH, HEIGHT);							// make the viewport cover the whole window
	glClearColor(0.0, 0.2, 0.4, 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#pragma endregion
	//Allow console for debugging
	//AllocConsole();
	//FILE* pCout;
	//FILE* pCerr;
	//freopen_s(&pCout, "CONOUT$", "w", stdout);
	//freopen_s(&pCerr, "CONOUT$", "w", stderr);

	bool Quit = false;
	MSG msg;
	Game* currentInstance = new Game();
	EventHandler* EventManager = new EventHandler();
	
	// main game loop starts here!
	// keep doing this as long as the player hasn't exited the app: 
	while (!Quit)
	{
		// we need to listen out for OS messages.
		// if there is a windows message to process...
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// if the message was a "quit" message...
			if (msg.message == WM_QUIT)
			{
				Quit = true; // we want to quit asap
			}
			// if it was any other type of message (i.e. one we don't care about), process it as normal
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen

		currentInstance->Update();

		SwapBuffers(myDeviceContext);							// update graphics
	}
	// the next bit will therefore happen when the player quits the app,
	// because they are trapped in the previous section as long as (keepPlaying == true).

	delete currentInstance;
	currentInstance = NULL;

	delete EventManager;
	EventManager = NULL;

	// UNmake our rendering context (make it 'uncurrent')
	wglMakeCurrent(NULL, NULL);

	// delete the rendering context, we no longer need it.
	wglDeleteContext(myRenderContext);

	// release our window's DC from the window
	ReleaseDC(myWindow, myDeviceContext);

	// end the program
	return msg.wParam;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// this part contains some code that should be collapsed for now too...
#pragma region keep_this_bit_collapsed_too!

// this function is called when any events happen to our window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

	switch (message)
	{
		// if they exited the window...	
	case WM_DESTROY:
		// post a message "quit" message to the main windows loop
		PostQuitMessage(0);
		return 0;
		break;
	}

	// must do this as a default case (i.e. if no other event was handled)...
	return DefWindowProc(hwnd, message, wparam, lparam);

}

#pragma endregion
