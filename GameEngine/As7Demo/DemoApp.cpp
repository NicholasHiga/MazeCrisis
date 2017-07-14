#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include "GameEngine.h"
#include "FixedOpenGLRenderer.h"
#include "SceneGraph.h"
#include <gl/gl.h>
#include "Camera.h"
#include "ModelManager.h"

const int WIN_WIDTH = 1024;
const int WIN_HEIGHT = 768;
const float CAM_MOVEMENT = 0.02f;

const float LOG_UPDATE_INTERVAL = 2.0f; // seconds
float log_update_timer = LOG_UPDATE_INTERVAL;

const unsigned int NUM_MESHES = 4;

static TCHAR szWindowClass[] = _T("DemoApp");
static TCHAR szTitle[] = _T("Demo Application");

HDC hDC;
HINSTANCE hInst;

void update(float deltaTime);

class DemoApp : public GameEngine
{
public:
	DemoApp();
	~DemoApp();

	SceneGraph *scene;
	SceneNode *root;
	SceneNode *childNodes[NUM_MESHES];
	FixedOpenGLRenderer *renderer;
	Camera *camera;

	MeshType *meshes[NUM_MESHES];
};

DemoApp::DemoApp()
{
	scene = new SceneGraph();
	root = scene->getRootSceneNode();
	camera = new Camera();
	renderer = FixedOpenGLRenderer::getInstance();

	camera->setCameraPosition(Vec3(0, 0, 2.2), false);
	camera->setSceneCenter(Vec3(0, 0, 0), false);
	camera->setCameraUp(Vec3(0, 1, 0), true);

	meshes[0] = loadModel("goblet", "Assets/goblet/kelch12_lowpolyn2.lwo");
	meshes[1] = loadModel("bench", "Assets/bench/table_lowpoly.3ds");
	meshes[2] = loadModel("swords", "Assets/sword/swords2_a.3ds");
	meshes[3] = loadModel("glock", "Assets/glock/glock18c.3DS");

	childNodes[0] = new SceneNode();
	childNodes[0]->setLocation(Vec3(0.0f, -0.5f, 1.0f));
	childNodes[0]->setScale(Vec3(1.0f, 1.0f, 1.0f));
	childNodes[0]->setOrientation(Quaternion(-90, 0, 0, false));
	childNodes[0]->addModel(meshes[0]);
	root->appendChild(childNodes[0]);

	childNodes[1] = new SceneNode();
	childNodes[1]->setLocation(Vec3(-0.3f, 0.0f, 1.0f));
	childNodes[1]->setScale(Vec3(0.2f, 0.2f, 0.2f));
	childNodes[1]->setOrientation(Quaternion(90, 0, -90, false));
	childNodes[1]->addModel(meshes[1]);
	root->appendChild(childNodes[1]);

	childNodes[2] = new SceneNode();
	childNodes[2]->setLocation(Vec3(0.0f, 0.5f, 1.0f));
	childNodes[2]->setScale(Vec3(0.008f, 0.008f, 0.008f));
	childNodes[2]->setOrientation(Quaternion(0, 0, 90, false));
	childNodes[2]->addModel(meshes[2]);
	root->appendChild(childNodes[2]);

	childNodes[3] = new SceneNode();
	childNodes[3]->setLocation(Vec3(0.3f, 0.0f, 1.0f));
	childNodes[3]->setScale(Vec3(0.03f, 0.03f, 0.03f));
	childNodes[3]->setOrientation(Quaternion(0, 0, 0, false));
	childNodes[3]->addModel(meshes[3]);
	root->appendChild(childNodes[3]);
	
	renderer->setWireframeMode(false);
}

DemoApp::~DemoApp()
{
	delete scene;
	delete camera;
}

DemoApp *gameEngine;

void
setupPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		16,                             /* prefered color depth */
		0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
		0,                              /* no alpha buffer */
		0,                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		16,                             /* depth buffer */
		0,                              /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};
	int pixelFormat;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat == 0) {
		MessageBox(WindowFromDC(hDC), _T("ChoosePixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) {
		MessageBox(WindowFromDC(hDC), _T("SetPixelFormat failed."), _T("Error"),
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HGLRC hGLRC;
	TCHAR greeting[] = _T("Hello, World!");

	switch (message)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		setupPixelFormat(hDC);
		hGLRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hGLRC);
		gameEngine = new DemoApp();
		gameEngine->onStart();
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		gameEngine->onEnd();
		break;
	case WM_SIZE:
	{
		INT nWidth = LOWORD(lParam);
		INT nHeight = HIWORD(lParam);
		gameEngine->renderer->setWindowSize(nWidth, nHeight, gameEngine->camera);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'W':
			gameEngine->camera->moveForward(CAM_MOVEMENT);
			break;
		case 'A':
			gameEngine->camera->moveLeft(CAM_MOVEMENT);
			break;
		case 'S':
			gameEngine->camera->moveBackward(CAM_MOVEMENT);
			break;
		case 'D':
			gameEngine->camera->moveRight(CAM_MOVEMENT);
			break;
		case 'R':
			gameEngine->camera->moveUp(CAM_MOVEMENT);
			break;
		case 'F':
			gameEngine->camera->moveDown(CAM_MOVEMENT);
			break;
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

					   // The parameters to CreateWindow explained:
					   // szWindowClass: the name of the application
					   // szTitle: the text that appears in the title bar
					   // WS_OVERLAPPEDWINDOW: the type of window to create
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
					   // 500, 100: initial size (width, length)
					   // NULL: the parent of this window
					   // NULL: this application does not have a menu bar
					   // hInstance: the first parameter from WinMain
					   // NULL: not used in this application

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIN_WIDTH, WIN_HEIGHT, NULL, NULL, hInstance, NULL);
	hDC = GetDC(hWnd);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (true)
	{
		double deltaTime = Clock::getMilliseconds() - gameEngine->numMilliseconds;
		gameEngine->preRender(deltaTime);
		gameEngine->numMilliseconds = Clock::getMilliseconds();

		if (FixedOpenGLRenderer::getInstance()->getWireframeMode())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		update(deltaTime);
		gameEngine->scene->renderSceneGraph(gameEngine->renderer, gameEngine->camera);
		SwapBuffers(hDC);
		gameEngine->postRender();

		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
			{
				// Quit
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void update(float deltaTime)
{
	deltaTime /= 1000.0f;
	Vec3 increment(60.0f * deltaTime, 30.0f * deltaTime, 80.0f * deltaTime);

	log_update_timer -= deltaTime;
	if (log_update_timer <= 0)
	{
		for (int i = 0; i < 1; i++)
		{
			if (gameEngine->childNodes[i]->isBeingRendered)
				gameEngine->info("Model " + to_string(i) + " is being rendered.");
			else
				gameEngine->info("Model " + to_string(i) + " is NOT being rendered.");
		}
		log_update_timer = LOG_UPDATE_INTERVAL;
		gameEngine->info("");
	}
}