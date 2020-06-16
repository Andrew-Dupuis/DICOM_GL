#include "Controls.h"

#define PI (3.141592653589793)
#define ON 1
#define OFF 0

float Controls::cameraRotateSpeed = 100;
float Controls::cameraTranslateSpeed = 5;
float Controls::objectTranslateSpeed = 0.1f;
float Controls::objectRotateSpeed = 1;
float Controls::objectScaleSpeed = 0.01f;
bool Controls::left_held, Controls::right_held, Controls::middle_held = false;

int Controls::PERSPECTIVE = ON;
int Controls::COORDINATES = ON;
int Controls::OBJECT = OFF;
int Controls::VOLUME = ON;
int Controls::SIMULATED = OFF;
int Controls::objectLookAtToggle = OFF;
int Controls::window = 500;
int Controls::level = 500;
int Controls::useColormaps = 0;

int Controls::window_width, Controls::window_height = 500;
Vector2 Controls::startMousePos;
Matrix4 Controls::localMatrix, Controls::modelMatrix, Controls::viewMatrix, Controls::originMatrix, Controls::startViewMatrix;

void Controls::onResize(int w, int h)
{
	glViewport(0, 0, w, h);
	window_width = w;
	window_height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)window_width / window_height, 0.01, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Controls::mouseMotion(int x, int y)
{
	Vector2 percent = (Vector2(x, y) - startMousePos) / Vector2(window_width, window_height);
	if (left_held)
		lookAround(percent);
	if (middle_held)
		strafe(percent);
	if (right_held)
		changeDistance(percent.y);
	glutPostRedisplay();
}

void Controls::mouseButton(int button, int state, int x, int y)
{
	startMousePos = Vector2(x, y);
	startViewMatrix = viewMatrix;

	switch (button) {
	case 0:
		left_held = !(bool)state;
		break;
	case 1:
		middle_held = !(bool)state;
		break;
	case 2:
		right_held = !(bool)state;
		break;
	}
}

void Controls::keyboard(unsigned char key, int x, int y)
{
	switch (key) {

#pragma region General Application Controls
	case 'q':                           /* Quit */
		printf("Exiting Program...");
		exit(0);
		break;
	case 'c':
		useColormaps = !useColormaps;
		break;
	case 'w':
		window -= 50;
		break;
	case 'e':
		window += 50;
		break;
	case 'k':
		level -= 50;
		break;
	case 'l':
		level += 50;
		break;
	case 'p':
		if (PERSPECTIVE) {
			PERSPECTIVE = OFF;
			printf("Switched to Orthographic Display\n");
		}
		else {
			PERSPECTIVE = ON;
			printf("Switched to Perspective Display\n");
		}
		break;
	case 's':
		if (SIMULATED) {
			SIMULATED = OFF;
			printf("Switching to simulated volume\n");
		}
		else {
			SIMULATED = ON;
			printf("Switching to DICOM volume\n");
		}
		break;

#pragma endregion


#pragma region Global Scale Controls
	case '=':
		modelMatrix.scale(1 + objectScaleSpeed);
		break;
	case'-':
		modelMatrix.scale(1 - objectScaleSpeed);
		break;
#pragma endregion

	default:
		break;
	}
	glutPostRedisplay();
}

void Controls::lookAround(Vector2 percent) {
	Matrix4 tempViewMatrix = startViewMatrix;
	tempViewMatrix.rotate(percent.x * cameraRotateSpeed, 0, 1, 0);
	viewMatrix = tempViewMatrix.rotate(percent.y * cameraRotateSpeed, 1, 0, 0);
}

void Controls::changeDistance(float percent) {
	Matrix4 tempViewMatrix = startViewMatrix;
	Vector3 translation = Vector3(0, 0, 1) * cameraTranslateSpeed * percent;
	viewMatrix = tempViewMatrix.translate(translation);
}

void Controls::strafe(Vector2 percent) {
	Matrix4 tempViewMatrix = startViewMatrix;
	Vector3 translation = Vector3(1, -1, 0) * cameraTranslateSpeed * Vector3(percent.x, percent.y, 0);
	viewMatrix = tempViewMatrix.translate(translation);
}

