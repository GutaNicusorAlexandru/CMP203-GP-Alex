#include "PlanetScene.h"
#include "Constants.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
PlanetScene::PlanetScene(Input* in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.


	// Initialise scene variables

}

void PlanetScene::drawTriangle(Vertex ver1, Vertex ver2, Vertex ver3)
{
	glBegin(GL_TRIANGLES);
	glColor3f(ver1.color.x, ver1.color.y, ver1.color.z);
	glVertex3f(ver1.position.x, ver1.position.y, ver1.position.z);

	glColor3f(ver2.color.x, ver2.color.y, ver2.color.z);
	glVertex3f(ver2.position.x, ver2.position.y, ver2.position.z);

	glColor3f(ver3.color.x, ver3.color.y, ver3.color.z);
	glVertex3f(ver3.position.x, ver3.position.y, ver3.position.z);
	glEnd();
}

void PlanetScene::drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides, Vector3 color)
{
	int numberOfVertices = numberOfSides + 2;

	GLfloat twicePi = 2.0f * 3.14159256f;

	GLfloat* circleVerticesX = new GLfloat[numberOfVertices];
	GLfloat* circleVerticesY = new GLfloat[numberOfVertices];
	GLfloat* circleVerticesZ = new GLfloat[numberOfVertices];

	circleVerticesX[0] = x;
	circleVerticesY[0] = y;
	circleVerticesZ[0] = z;

	for (int i = 1; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * twicePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * twicePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat *allCircleVertices = new GLfloat [(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glColor3f(color.x, color.y, color.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void PlanetScene::drawSquare(Vertex ver1, Vertex ver2, Vertex ver3, Vertex ver4)
{
	glBegin(GL_QUADS);
	glColor3f(ver1.color.x, ver1.color.y, ver1.color.z);
	glVertex3f(ver1.position.x, ver1.position.y, ver1.position.z);

	glColor3f(ver2.color.x, ver2.color.y, ver2.color.z);
	glVertex3f(ver2.position.x, ver2.position.y, ver2.position.z);

	glColor3f(ver3.color.x, ver3.color.y, ver3.color.z);
	glVertex3f(ver3.position.x, ver3.position.y, ver3.position.z);

	glColor3f(ver4.color.x, ver4.color.y, ver4.color.z);
	glVertex3f(ver4.position.x, ver4.position.y, ver4.position.z);
	glEnd();
}

void PlanetScene::handleInput(float dt)
{
	// Handle user input
}

void PlanetScene::update(float dt)
{
	// update scene related variables.

	i += dt * 10;
	// Calculate FPS for output
	calculateFPS();
}

void PlanetScene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Render geometry/scene here -------------------------------------
	
	drawTriangle(Vertex(Vector3(1, 1, 0), Colors::Red), Vertex(Vector3(-1, 1, 0), Colors::Blue), Vertex(Vector3(1, -1, 0), Colors::Blue));



	glPushMatrix();
	if (i < i + 1)
	{
		glRotatef(-10 * i, 0, 0, 1);
		glTranslatef(1, 0, 0);
	}
	
	drawCircle(0, 0, 0, 0.2, 100, Colors::Red);
	glPopMatrix();

	glPushMatrix();
	if (i < i + 1)
	{
		glRotatef(-6 * i, 0, 0, 1);
		glTranslatef(1.5, 0, 0);
		glPushMatrix();
		if (i < i + 1)
		{
			glRotatef(-2 * i, 0, 0, 1);
			glTranslatef(0.5, 0, 0);
		}

		drawCircle(0, 0, 0, 0.1, 100, Colors::Black);
		glPopMatrix();
	}

	drawCircle(0, 0, 0, 0.35, 100, Colors::Blue);
	glPopMatrix();

	glPushMatrix();
	int j = i;
	if (j % 20 <= 10)
	{
		glTranslatef(0.1 * j, 0, 0);
	}

	drawCircle(0, 0, 0, 0.5, 100, Colors::White);
	glPopMatrix();
	

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();

	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void PlanetScene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void PlanetScene::resize(int w, int h)
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void PlanetScene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void PlanetScene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void PlanetScene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width / (float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
