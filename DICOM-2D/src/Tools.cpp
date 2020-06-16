#include "Tools.h"
#include "Vectors.h"
#include "Matrices.h"
#include <SOIL.h>



//Read the shader files, given as parameter.
char* Tools::fileReadToCharArray(const char* fn) {

	char* content = NULL;
	FILE* fp = fopen(fn, "r");
	if (!fp)
	{
		std::cout << "Failed to load " << fn;
		return content;
	}
	else
	{
		std::cout << "Successfully loaded " << fn << "\n";
	}

	int count = 0;

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		rewind(fp);

		if (count > 0)
		{
			content = (char*)malloc(sizeof(char) * (count + 1));
			count = fread(content, sizeof(char), count, fp);
			content[count] = '\0';
		}
		fclose(fp);
	}
	return content;
}

char* Tools::ConcatenateStrings(char* left, char* right) {
	char* dest = new char[strlen(left) + strlen(right)];
	strcpy(dest, left);
	strcat(dest, right);
	return dest;
}

void Tools::writeStringToScreen(char* string, Vector3 position, Vector3 color) {
	char* c;
	glRasterPos3f(position.x, position.y, position.z);
	glColor3f(color.x, color.y, color.z);
	for (c = string; *c != NULL; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	}
}

void Tools::CompileShader(GLuint shader) {
	glCompileShaderARB(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		printf("Shader compilation failed:\n");
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		for (auto i : errorLog)
			std::cout << i;

		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.
		printf("\n");
		return;
	}
	else {
		std::cout << "Successfully loaded shader:" << shader << "\n";
	}
}

GLuint Tools::setShaders(const char* vertex, const char* fragment)
{

	GLuint p, v, f;
	char* vs = NULL, * fs = NULL;

	v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	vs = fileReadToCharArray(vertex);
	fs = fileReadToCharArray(fragment);

	const char* vv = vs;
	const char* ff = fs;

	glShaderSourceARB(v, 1, &vv, NULL);
	glShaderSourceARB(f, 1, &ff, NULL);

	CompileShader(v);
	CompileShader(f);

	free(vs);
	free(fs);

	p = glCreateProgramObjectARB();
	glAttachObjectARB(p, f);
	glAttachObjectARB(p, v);

	glLinkProgramARB(p);
	glUseProgramObjectARB(p);
	return p;
}

void Tools::setShaderUniform1f(GLuint program, char* variableName, GLfloat value)
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform1f(location, value);
}

void Tools::setShaderUniform2f(GLuint program, char* variableName, GLfloat value[2])
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform2f(location, value[0], value[1]);
}

void Tools::setShaderUniform3f(GLuint program, char* variableName, GLfloat value[3])
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform3f(location, value[0], value[1], value[2]);
}

void Tools::setShaderUniform4f(GLuint program, char* variableName, GLfloat value[4])
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform4f(location, value[0], value[1], value[2], value[3]);
}


void Tools::setShaderUniform1i(GLuint program, char* variableName, GLint value)
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform1i(location, value);
}

void Tools::setShaderUniform2i(GLuint program, char* variableName, GLint value[2])
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform2i(location, value[0], value[1]);
}

void Tools::setShaderUniform3i(GLuint program, char* variableName, GLint value[3])
{
	GLint location = glGetUniformLocation(program, variableName);
	glUniform3i(location, value[0], value[1], value[2]);
}

void Tools::setShaderUniform4i(GLuint program, char* variableName, GLint value[3]) {
	GLint location = glGetUniformLocation(program, variableName);
	glUniform4i(location, value[0], value[1], value[2], value[3]);
}

void Tools::setShaderUniform4x4f(GLuint program, char* variableName, Matrix4 value) {
	GLint location = glGetUniformLocation(program, variableName);
	glUniformMatrix4fv(location, 1, false, value.get()); // I'm an idiot, count !=16, it == 1
}



void Tools::initGraphics(float backgroundColor[4]) {
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

	if (GLEW_OK != glewInit())
	{
		printf("GLEW init error!\n");
		exit(1);
	}
	else {
		printf("GLEW init sucessful\n");
	}

	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
	{
		printf("No GLSL support\n");
		exit(1);
	}
}

Matrix4 Tools::GeneratePerspectiveMatrix(float ViewPlane, float ViewWidth, float ViewHeight, float NearPlane, float FarPlane)
{
	float ProjectionMatrix[16];

	ProjectionMatrix[0] = ProjectionMatrix[5] = ProjectionMatrix[10] = ProjectionMatrix[15] = 1;
	ProjectionMatrix[1] = ProjectionMatrix[2] = ProjectionMatrix[3] = ProjectionMatrix[4] =
		ProjectionMatrix[6] = ProjectionMatrix[7] = ProjectionMatrix[8] = ProjectionMatrix[9] =
		ProjectionMatrix[11] = ProjectionMatrix[12] = ProjectionMatrix[13] = ProjectionMatrix[14] = 0;

	ProjectionMatrix[0] = ViewPlane * 2 / ViewWidth;
	ProjectionMatrix[5] = ViewPlane * 2 / ViewHeight;
	ProjectionMatrix[10] = FarPlane / (NearPlane - FarPlane);
	ProjectionMatrix[11] = -1.0;
	ProjectionMatrix[14] = NearPlane * FarPlane / (NearPlane - FarPlane);
	ProjectionMatrix[15] = 0.0; 

	return Matrix4(ProjectionMatrix);
}

// Calculate the new orthographic projection matrix
Matrix4 Tools::GenerateOrthographicMatrix(float ViewWidth, float ViewHeight, float NearPlane, float FarPlane)
{
	float ProjectionMatrix[16];

	ProjectionMatrix[0] = ProjectionMatrix[5] = ProjectionMatrix[10] = ProjectionMatrix[15] = 1;
	ProjectionMatrix[1] = ProjectionMatrix[2] = ProjectionMatrix[3] = ProjectionMatrix[4] =
		ProjectionMatrix[6] = ProjectionMatrix[7] = ProjectionMatrix[8] = ProjectionMatrix[9] =
		ProjectionMatrix[11] = ProjectionMatrix[12] = ProjectionMatrix[13] = ProjectionMatrix[14] = 0;

	ProjectionMatrix[0] = 2 / ViewWidth;
	ProjectionMatrix[5] = 2 / ViewHeight;
	ProjectionMatrix[10] = NearPlane / (NearPlane - FarPlane);

	return Matrix4(ProjectionMatrix);
}

void Tools::drawCoordinateAxes(Matrix4 transformationMatrix, Color xAxis, Color yAxis, Color zAxis) {

	Vector4 origin = transformationMatrix * Vector4(0, 0, 0, 1);
	Vector4 xUnit = transformationMatrix * Vector4(1, 0, 0, 1);
	Vector4 yUnit = transformationMatrix * Vector4(0, 1, 0, 1);
	Vector4 zUnit = transformationMatrix * Vector4(0, 0, 1, 1);

	// Draw a X Axis
	glColor3f(xAxis.x, xAxis.y, xAxis.z);
	glBegin(GL_LINES);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(xUnit.x, xUnit.y, xUnit.z);
	glEnd();

	// Draw a Y Axis
	glColor3f(yAxis.x, yAxis.y, yAxis.z);
	glBegin(GL_LINES);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(yUnit.x, yUnit.y, yUnit.z);
	glEnd();

	// Draw a Z Axis
	glColor3f(zAxis.x, zAxis.y, zAxis.z);
	glBegin(GL_LINES);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(zUnit.x, zUnit.y, zUnit.z);
	glEnd();
}

float* Tools::Transform(float* verts, int count, Matrix4 transformationMatrix) {

	float* transformedVerts = new float[count];

	for (int i = 0; i < count; i = i + 3) {
		Vector4 vertex = Vector4(verts[i], verts[i + 1], verts[i + 2], 1);
		vertex = transformationMatrix * vertex;
		transformedVerts[i] = vertex.x;
		transformedVerts[i + 1] = vertex.y;
		transformedVerts[i + 2] = vertex.z;
	}
	return transformedVerts;
}
