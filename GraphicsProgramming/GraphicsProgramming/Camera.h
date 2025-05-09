#include <glut.h>
#include <cmath>
#include <iostream>
#include "Input.h"
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI / 2.0)
#endif

class Camera 
{
public:
      void updateCamera(Input* input, float dt) {
          mouseMotion(input->getMouseX(), input->getMouseY());

          float fx = cos(camPitch) * sin(camYaw);
          float fy = sin(camPitch);
          float fz = -cos(camPitch) * cos(camYaw);

          // Movement
          if (input->isKeyDown('w')) {
              camX += fx * speed * dt;
              camY += fy * speed * dt;
              camZ += fz * speed * dt;
          }
          if (input->isKeyDown('s')) {
              camX -= fx * speed * dt;
              camY -= fy * speed * dt;
              camZ -= fz * speed * dt;
          }

          if (input->isKeyDown('q')) camY += speed * dt;
          if (input->isKeyDown('a')) camY -= speed * dt;

          if (input->isKeyDown('e')) speed += 5 * dt;
          if (input->isKeyDown('d')) speed -= 5 * dt;

          if (speed < minSpeed) speed = minSpeed;
          if (speed > maxSpeed) speed = maxSpeed;
      }

      void mouseMotion(int x, int y) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        camYaw += dx * sensitivity;
        camPitch -= dy * sensitivity;

        // Clamp pitch to prevent flipping
        if (camPitch > M_PI_2 - 0.01f) camPitch = M_PI_2 - 0.01f;
        if (camPitch < -M_PI_2 + 0.01f) camPitch = -M_PI_2 + 0.01f;

        // Recenter the mouse
        glutWarpPointer(400, 300);
        lastMouseX = 400;
        lastMouseY = 300;
    }

      // Camera position and orientation
      float camX = 0.0f, camY = 0.0f, camZ = 5.0f;
      float camYaw = 0.0f;    // Horizontal angle (left/right)
      float camPitch = 0.0f;  // Vertical angle (up/down)
      float speed = 1.f;
      float maxSpeed = 5;
      float minSpeed = 0.5;

      // Mouse control
      int lastMouseX = 400, lastMouseY = 300;
      const float sensitivity = 0.0005f;
};