// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include "Vector3.h"
#include "Cubesphere.h"
#include "Sphere.h"
#include "Utility.h"
#include "Camera.h"

class Scene{

public:
	Scene(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();


	// Other functions
	void DrawCube(float size, GLuint texture);

	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];

	// Storage arrays
	std::vector<float>* verts = new std::vector<float>;
	std::vector<float>* normals = new std::vector<float>;
	std::vector<float>* texCoords = new std::vector<float>;
	std::vector<int>* indeces = new std::vector<int>;

	// Other Variables
	bool wireframe = false;
	float wireframeCooldown = 0;
	bool pause = false;
	float pauseCooldown = 0;
	bool lightSwitch = false;
	float lightSwitchCooldown = 0;
	GLfloat Light_Position[4] = {20,0,20,1.f };
	Cubesphere sphere;
	Sphere earth;
	float angle;
	Utility utility;
	Camera camera;
	Model* spaceshipModel;
};

#endif