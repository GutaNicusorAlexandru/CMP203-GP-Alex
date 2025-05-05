#include "Scene.h"
#include "Constants.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in) :
	sphere(1.0f, 10, 1, 10)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Initialise scene variables
	
}

void Scene::drawTriangle(Vertex ver1, Vertex ver2, Vertex ver3)
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

void Scene::drawStripTriangle(Vertex vertexes[6])
{
	glBegin(GL_TRIANGLE_STRIP);

	for (int i = 0; i < 6; i++)
	{
		glColor3f(vertexes[i].color.x, vertexes[i].color.y, vertexes[i].color.z);
		glVertex3f(vertexes[i].position.x, vertexes[i].position.y, vertexes[i].position.z);
	}

	glEnd();
}

void Scene::drawFanTriangle(Vertex vertexes[6])
{
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 6; i++)
	{
		glColor3f(vertexes[i].color.x, vertexes[i].color.y, vertexes[i].color.z);
		glVertex3f(vertexes[i].position.x, vertexes[i].position.y, vertexes[i].position.z);
	}

	glEnd();
}

void Scene::drawSquareTriangle(Vertex ver1, Vertex ver2, Vertex ver3, Vertex ver4, Vertex ver5, Vertex ver6)
{
	glBegin(GL_TRIANGLES);
	glColor3f(ver1.color.x, ver1.color.y, ver1.color.z);
	glVertex3f(ver1.position.x, ver1.position.y, ver1.position.z);

	glColor3f(ver2.color.x, ver2.color.y, ver2.color.z);
	glVertex3f(ver2.position.x, ver2.position.y, ver2.position.z);

	glColor3f(ver3.color.x, ver3.color.y, ver3.color.z);
	glVertex3f(ver3.position.x, ver3.position.y, ver3.position.z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(ver4.color.x, ver4.color.y, ver4.color.z);
	glVertex3f(ver4.position.x, ver4.position.y, ver4.position.z);

	glColor3f(ver5.color.x, ver5.color.y, ver5.color.z);
	glVertex3f(ver5.position.x, ver5.position.y, ver5.position.z);

	glColor3f(ver6.color.x, ver6.color.y, ver6.color.z);
	glVertex3f(ver6.position.x, ver6.position.y, ver6.position.z);
	glEnd();
}

void Scene::drawSquare(Vertex ver1, Vertex ver2, Vertex ver3, Vertex ver4)
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

void Scene::handleInput(float dt)
{
	// Handle user input
}

void Scene::update(float dt)
{
	// update scene related variables.

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	glVertexPointer(3, GL_FLOAT, 0, verts.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glColorPointer(3, GL_FLOAT, 0, colors.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());


	// Render geometry/scene here -------------------------------------
	Vertex vertexes[6];

	Vertex v1(Vector3(1.0f, 1.0f, 0.0f), Colors::Red);
	Vertex v2(Vector3(1.0f, -1.0f, 0.0f), Colors::Blue);
	Vertex v3(Vector3(-1.0f, -1.0f, 0.0f), Colors::Red);
	Vertex v4(Vector3(-1.0f, 1.0f, 0.0f), Colors::Green);

	vertexes[0] = Vertex(Vector3(1.0f, 1.0f, 0.0f), Colors::Red);
	vertexes[1] = Vertex(Vector3(1.5f, -1.0f, 0.0f), Colors::Blue);
	vertexes[2] = Vertex(Vector3(2.0f, 1.0f, 0.0f), Colors::Green);
	vertexes[3] = Vertex(Vector3(2.5f, -1.0f, 0.0f), Colors::Blue);
	vertexes[4] = Vertex(Vector3(3.0f, 1.0f, 0.0f), Colors::Green);
	vertexes[5] = Vertex(Vector3(3.5f, -1.0f, 0.0f), Colors::Blue);
	
	//drawSquare(v1, v2, v3, v4);

	float ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
	float diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1 };
	float shininess = 128;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

	glPushMatrix();
		sphere.draw();
	glPopMatrix();
	
	// End render geometry --------------------------------------


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	GLfloat Light_Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat Light_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light_Position[] = { 0.0f, 7.0f, -5.0f, 1.0f };
	GLfloat spot_Direction[] = { 0.0f, -1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_Direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 50.0);
	glEnable(GL_LIGHT0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blending function
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
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
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
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
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
