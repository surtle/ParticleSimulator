////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include "Tester.h"
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER=0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	TESTER=new Tester("CSE 169 Project 4",argc,argv);
	glutMainLoop();
	delete TESTER;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0.78, 0.91, 0.94, 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0f);

	// Initialize components
	Program=new ShaderProgram("Model.glsl",ShaderProgram::eRender);
	FloorProgram = new ShaderProgram("Shader.glsl", ShaderProgram::eRender);
	Cube=new SpinningCube;
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));

	// initialize cloth
	//cloth = new Cloth();
	//cloth->init();

	// initialize floor
	floor = new Floor();

	// initialize system
	system = new ParticleSystem();

	// init GUI
	gui = new GUI(WinX, WinY, system);
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete Program;
	delete Cube;
	delete Cam;
	delete skeleton;
	delete skin;
	delete anim;
	delete player;
	delete system;
	delete floor;
	delete cloth;

	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cube->Update();
	Cam->Update();

	//cloth->update();
	//cloth->draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	system->update();

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));

	Cube->Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	//cloth->draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	floor->draw(Cam->GetViewProjectMtx(), FloorProgram->GetProgramID());
	system->draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	gui->draw();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	TwTerminate();
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	if (!TwEventKeyboardGLUT(key, x, y)) {
		switch (key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case '1':
			cloth->updateTopLeft(0, 0.2);
			break;
		case '2':
			cloth->updateTopLeft(1, 0.2);
			break;
		case '3':
			cloth->updateTopLeft(2, 0.2);
			break;
		case 'q':
			cloth->updateTopLeft(0, -0.2);
			break;
		case 'w':
			cloth->updateTopLeft(1, -0.2);
			break;
		case 'e':
			cloth->updateTopLeft(2, -0.2);
			break;
		case '8':
			cloth->updateTopRight(0, 0.2);
			break;
		case '9':
			cloth->updateTopRight(1, 0.2);
			break;
		case '0':
			cloth->updateTopRight(2, 0.2);
			break;
		case 'i':
			cloth->updateTopRight(0, -0.2);
			break;
		case 'o':
			cloth->updateTopRight(1, -0.2);
			break;
		case 'p':
			cloth->updateTopRight(2, -0.2);
			break;
		case 'n':
			cloth->modifyWind(-0.5);
			break;
		case 'm':
			cloth->modifyWind(0.5);
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if (!TwEventMouseButtonGLUT(btn, state, x, y)) {
		if (btn == GLUT_LEFT_BUTTON) {
			LeftDown = (state == GLUT_DOWN);
		}
		else if (btn == GLUT_MIDDLE_BUTTON) {
			MiddleDown = (state == GLUT_DOWN);
		}
		else if (btn == GLUT_RIGHT_BUTTON) {
			RightDown = (state == GLUT_DOWN);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {

	if (!TwEventMouseMotionGLUT(nx, ny)) {
		int maxDelta = 100;
		int dx = glm::clamp(nx - MouseX, -maxDelta, maxDelta);
		int dy = glm::clamp(-(ny - MouseY), -maxDelta, maxDelta);

		MouseX = nx;
		MouseY = ny;

		// Move camera
		// NOTE: this should really be part of Camera::Update()
		if (LeftDown) {
			const float rate = 1.0f;
			Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
			Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
		}
		if (RightDown) {
			const float rate = 0.005f;
			float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
			Cam->SetDistance(dist);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
