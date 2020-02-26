////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"
#include "Player.h"
#include "Cloth.h"
#include "Floor.h"

////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	ShaderProgram *Program;
	ShaderProgram* FloorProgram;
	SpinningCube *Cube;
	Camera *Cam;
	Skeleton* skeleton;
	Skin* skin;
	Animation* anim;
	Player* player;
	Cloth* cloth;
	Floor* floor;
	Tokenizer* t; 
};

////////////////////////////////////////////////////////////////////////////////
