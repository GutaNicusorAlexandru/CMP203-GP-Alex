#include "Scene.h"
#include "Constants.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input* in) :
	utility(verts, normals, texCoords, indeces),
	sphere(1.0f, 50, 1),
	earth(0.3f, 30, 20, 1, 2)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	
	// Initialise scene variables
	camera.camX = 0, camera.camZ = 7;
}

void Scene::handleInput(float dt)
{
	// Light controls
	if (input->isKeyDown('n')) Light_Position[1] += 1.5 * dt;
	if (input->isKeyDown('m')) Light_Position[1] -= 1.5 * dt;
	if (input->isKeyDown(37)) Light_Position[2] -= 1.5 * dt;
	if (input->isKeyDown(38)) Light_Position[0] -= 1.5 * dt;
	if (input->isKeyDown(39)) Light_Position[0] += 1.5 * dt;
	if (input->isKeyDown(40)) Light_Position[2] += 1.5 * dt;

	// Wireframe toggle
	wireframeCooldown += dt;
	if (input->isKeyDown('k') && wireframeCooldown > 0.2f)
	{
		wireframe = !wireframe;
		wireframeCooldown = 0;
	}

	// Pause toggle
	pauseCooldown += dt;
	if (input->isKeyDown('p') && pauseCooldown > 0.2f)
	{
		pause = !pause;
		pauseCooldown = 0;
	}

	// LightSwitch toggle
	lightSwitchCooldown += dt;
	if (input->isKeyDown('l') && lightSwitchCooldown > 0.2f)
	{
		lightSwitch = !lightSwitch;
		lightSwitchCooldown = 0;
	}
}

void Scene::update(float dt)
{
	// update the camera
	camera.updateCamera(input, dt);

	// update scene related variables.
	if (!pause)
	{
		if (input->isKeyDown('f')) angle += dt * 2.5;
		else angle += dt * 0.5;
		if (angle > 360) angle = 0;
		Light_Position[0] = 20.f * sin(angle);
		Light_Position[1] = 20.f * sin(angle);
		Light_Position[2] = 20.f * cos(angle);
	}

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, verts->data());
	glNormalPointer(GL_FLOAT, 0, normals->data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords->data());
	glIndexPointer(3, GL_FLOAT, indeces->data());

	// Reset transformations
	glLoadIdentity();

	// Look-at calculation
	float lx = camera.camX + cos(camera.camPitch) * sin(camera.camYaw);
	float ly = camera.camY + sin(camera.camPitch);
	float lz = camera.camZ - cos(camera.camPitch) * cos(camera.camYaw);

	gluLookAt(camera.camX, camera.camY, camera.camZ, lx, ly, lz, 0.0f, 1.0f, 0.0f);

	// Render geometry/scene here -------------------------------------

	if (!lightSwitch) // Light 0
	{
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);

	} else // Light 1
	{
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}

	float skyAmbient[] = { 10.f, 10.f, 10.f, 1 };
	float skyDiffuse[] = { 0.0f, 0.0f, 0.0f, 1 };
	float skySpecular[] = { 0.0f, 0.0f, 0.0f, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, skyAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, skyDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, skySpecular);
	
	// Always draw skybox in fill mode 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();

		glTranslatef(camera.camX, camera.camY, camera.camZ);
		DrawCube(10, utility.GetTextureByName("SkyTexture"));

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	// Enable/Disable wireframe mode
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float source[] = { 1.f,1.f,1.f, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, source);

	// Light representation
	glPushMatrix();
		glTranslatef(Light_Position[0], Light_Position[1], Light_Position[2]);
		glColor3f(1, 1, 1);
		glutSolidSphere(1,40,40);
	glPopMatrix();

	float ambient[] = { 0.3f, 0.3f, 0.3f, 1 };
	float diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
	float specular[] = { 0.3f, 0.3f, 0.3f, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Earth sphere
	glPushMatrix();

	for (int i = 0; i < 6/*number of faces on the cubeshpere*/; i++)
	{
		earth.draw(utility.GetTextureByName("EarthTexture"));
	}

		

	glBindTexture(GL_TEXTURE_2D, NULL);
	glPopMatrix();

	// Outer sphere
	glPushMatrix();

		glRotatef(-angle * 20, 0, 1, 0);
		sphere.draw(utility.GetTextureByName("BallTexture"));

	glPopMatrix();

	// End render geometry --------------------------------------


	glVertexPointer(3, GL_FLOAT, 0, nullptr);
	glNormalPointer(GL_FLOAT, 0, nullptr);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
	glIndexPointer(3, GL_FLOAT, nullptr);

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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);			// Cornflour Blue Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(width / 2, height / 2);
	glEnable(GL_BLEND);

	GLfloat Light_Ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat Light_Diffuse[] = { 20.0f, 20.0f, 20.0f, 1.0f };
	GLfloat Light_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Specular);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	glEnable(GL_LIGHT0);

	GLfloat Light2_Ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat Light2_Diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat Light2_Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat Light2_Position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, Light2_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light2_Diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, Light2_Specular);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
	glEnable(GL_LIGHT1);

	GLuint texture;
	utility.generatePurpleTileTexture(&texture);
	utility.AddTexture(texture, "BallTexture");

	utility.LoadTexture("gfx/space_bk.png", "SkyTexture");

	utility.LoadTexture("gfx/earth.png", "EarthTexture");

	// couldn't get the model to load
	spaceshipModel = utility.loadOBJ("models/spaceship.obj");
	utility.LoadTexture("models/spaceship.jpg", "SpaceshipTexture");

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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

void Scene::DrawCube(float size, GLuint texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	
	// Front
	glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(size, size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, size);

	// Back
	glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(size, size, -size);

	// Top
	glTexCoord2f(0, 0); glVertex3f(-size, size, size);
	glTexCoord2f(1, 0); glVertex3f(size, size, size);
	glTexCoord2f(1, 1); glVertex3f(size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, -size);

	// Bottom
	glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(size, -size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, -size, size);

	// Left
	glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
	glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
	glTexCoord2f(1, 1); glVertex3f(-size, size, size);
	glTexCoord2f(0, 1); glVertex3f(-size, size, -size);

	// Right
	glTexCoord2f(0, 0); glVertex3f(size, -size, size);
	glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
	glTexCoord2f(1, 1); glVertex3f(size, size, -size);
	glTexCoord2f(0, 1); glVertex3f(size, size, size);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, NULL);
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
