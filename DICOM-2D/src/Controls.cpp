#include "Controls.h"

#define PI (3.141592653589793)
#define ON 1
#define OFF 0

float Controls::cameraRotateSpeed = 100;
float Controls::cameraTranslateSpeed = 1;
float Controls::objectTranslateSpeed = 0.1f;
float Controls::objectRotateSpeed = 90;
float Controls::objectScaleSpeed = 0.1f;
bool Controls::left_held, Controls::right_held, Controls::middle_held = false;

int Controls::PERSPECTIVE = ON;
int Controls::COORDINATES = ON;
int Controls::OBJECT = OFF;
int Controls::VOLUME = ON;
int Controls::objectLookAtToggle = OFF;
int Controls::window = 500, Controls::level = 500;
int startWindow = 500, startLevel = 500;
float startOrthoScale = 3;
float currentOrthoScale = 3;

int Controls::currentDicom = 0;
int Controls::currentColormap = 0;
bool Controls::useColormaps = false;


int Controls::window_width, Controls::window_height = 500;
Vector2 Controls::startMousePos;
Matrix4 Controls::localMatrix, Controls::modelMatrix, Controls::viewMatrix, Controls::originMatrix, Controls::startViewMatrix;
GLuint Controls::shaderProgram;

void Controls::onResize(int w, int h)
{
	if (w > h) {
		glViewport(0, (h - w) / 2, w, w);
		window_width = w;
		window_height = w;
	}
	else {
		glViewport((w - h) / 2, 0, h, h);
		window_width = h;
		window_height = h;
	}
	Matrix4 ProjectionMatrix = Tools::GenerateOrthographicMatrix(currentOrthoScale, currentOrthoScale * ((float)Controls::window_height / (float)Controls::window_height), 0, 10);
	Tools::setShaderUniform4x4f(shaderProgram, (char*)"projectionMatrix", ProjectionMatrix);
}

void Controls::AdjustWindowLevel(Vector2 percent) {
	window = startWindow + percent.x * 500;
	level = startLevel - percent.y * 500;
	if (window < 0) {
		window = 0;
	}
	if (level < 0) {
		level = 0;
	}
	//std::cout << window << ":" << level << "\n";
}

void Controls::mouseMotion(int x, int y)
{
	Vector2 percent = (Vector2(x, y) - startMousePos) / Vector2(window_width, window_height);
	if (left_held)
		AdjustWindowLevel(percent);
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
	startWindow = window;
	startLevel = level;
	startOrthoScale = currentOrthoScale;
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
	case 3:
		std::cout << "scrolling";
		if (state == GLUT_UP) {
			currentDicom++;
			if (currentDicom >= DICOMImporter::numberOfTextures)
				currentDicom = 0;
		}
		break;

	case 4:
		std::cout << "scrolling";
		if (state == GLUT_UP) {
			currentDicom--;
			if (currentDicom < 0)
				currentDicom = DICOMImporter::numberOfTextures - 1;
		}
		break;
	}

}

void Controls::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':                           /* Quit */
		printf("Exiting Program...");
		exit(0);
		break;
	case 'c':
		useColormaps = !useColormaps;
		break;
#pragma region Global Translation Controls
	case '4':
		modelMatrix.translate(Vector3(objectTranslateSpeed, 0, 0));
		break;
	case '6':
		modelMatrix.translate(Vector3(-objectTranslateSpeed, 0, 0));
		break;
	case '8':
		modelMatrix.translate(Vector3(0, -objectTranslateSpeed, 0));
		break;
	case '2':
		modelMatrix.translate(Vector3(0, objectTranslateSpeed, 0));
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

void Controls::specialInputs(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		currentDicom++;
		if (currentDicom >= DICOMImporter::numberOfTextures)
			currentDicom = 0;
		break;
	case GLUT_KEY_DOWN:
		currentDicom--;
		if (currentDicom < 0)
			currentDicom = DICOMImporter::numberOfTextures - 1;
		break;
	case GLUT_KEY_HOME:
		currentColormap++;
		if (currentColormap >= DICOMImporter::numberOfColormaps)
			currentColormap = 0;
		break;
	case GLUT_KEY_END:
		currentColormap--;
		if (currentColormap < 0)
			currentColormap = DICOMImporter::numberOfColormaps - 1;
		break;
	case GLUT_KEY_LEFT:
		modelMatrix.rotate(-objectRotateSpeed, 0, 0, 1);
		break;
	case GLUT_KEY_RIGHT:
		modelMatrix.rotate(objectRotateSpeed, 0, 0, 1);
		break;
	case GLUT_KEY_PAGE_UP:
		modelMatrix.scale(1 + objectScaleSpeed);
		break;
	case GLUT_KEY_PAGE_DOWN:
		modelMatrix.scale(1 - objectScaleSpeed);
		break;
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
	currentOrthoScale = startOrthoScale + percent;
	//std::cout << currentOrthoScale;
	Matrix4 ProjectionMatrix = Tools::GenerateOrthographicMatrix(currentOrthoScale, currentOrthoScale*((float)Controls::window_height/(float)Controls::window_height), 0, 10);
	Tools::setShaderUniform4x4f(shaderProgram, (char*)"projectionMatrix", ProjectionMatrix);
}

void Controls::strafe(Vector2 percent) {
	Matrix4 tempViewMatrix = startViewMatrix;
	Vector3 translation = Vector3(1, -1, 0) * cameraTranslateSpeed * Vector3(percent.x, percent.y, 0);
	viewMatrix = tempViewMatrix.translate(translation);
}

