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
#include "DICOMImporter.h"
class Controls
{
 
public:
	static int PERSPECTIVE, COORDINATES, OBJECT, objectLookAtToggle, VOLUME;
	static float cameraRotateSpeed, cameraTranslateSpeed, objectTranslateSpeed, objectRotateSpeed, objectScaleSpeed;
	static bool left_held, right_held, middle_held, useColormaps;
	static int window_width, window_height, window, level, currentDicom, currentColormap;
	static Vector2 startMousePos;
	static Matrix4 localMatrix, modelMatrix, viewMatrix, originMatrix, startViewMatrix;
	static GLuint shaderProgram;


	static void onResize(int w, int h);
	static void AdjustWindowLevel(Vector2 percent);
	static void mouseMotion(int x, int y);
	static void mouseButton(int button, int state, int x, int y);
	static void keyboard(unsigned char key, int x, int y);
	static void specialInputs(int key, int x, int y);
	static void lookAround(Vector2 percent);
	static void changeDistance(float percent);
	static void strafe(Vector2 percent);
};

