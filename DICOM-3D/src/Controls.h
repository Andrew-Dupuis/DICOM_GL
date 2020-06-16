#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <string.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <windows.h>
#include <random>
#include "Tools.h"
#include "Matrices.h"
#include "Vectors.h"

class Controls
{
 
public:
	static int PERSPECTIVE, COORDINATES, OBJECT, objectLookAtToggle, VOLUME, SIMULATED;
	static float cameraRotateSpeed, cameraTranslateSpeed, objectTranslateSpeed, objectRotateSpeed, objectScaleSpeed;
	static bool left_held, right_held, middle_held;
	static int window_width, window_height, window, level, useColormaps;
	static Vector2 startMousePos;
	static Matrix4 localMatrix, modelMatrix, viewMatrix, originMatrix, startViewMatrix;

	static void onResize(int w, int h);
	static void mouseMotion(int x, int y);
	static void mouseButton(int button, int state, int x, int y);
	static void keyboard(unsigned char key, int x, int y);
	static void lookAround(Vector2 percent);
	static void changeDistance(float percent);
	static void strafe(Vector2 percent);
};

