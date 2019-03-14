/// RETARGET SOLUTION MAY BE NECESSARY TO RUN AS INTENDED. 
//code framework taken from coventry university moodle 212

#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Shaders.h"
#include "getBMP.h"
#include "vertex.h"
#include "sphere.h"
#include "GameObject.h"

#define MY_FRAME 16

using namespace std;
using namespace glm;

static BitMapFile *image[2];

// d floats affect camera possition.
static float d = 0.0, d2 = 0.0, d3 = -2.5;
float GameObject::d = -800.0;
float GameObject::d2 = -14.99f;
float GameObject::d3 = 20;
float GameObject::d4 = 0;
float GameObject::d5 = 0;
float GameObject::d6 = 0;
int Sphere::idChecker = 0;
bool CameraMode = true;
static float xVal = 0, zVal = 0; // Anim vars for testing
static unsigned int *sphereIndices = NULL;
static VertexWithNormal *sphereVerticesNor = NULL;
static Sphere tSphere;

static unsigned int 
modelViewMatLoc, objectLoc, 
vao[3], vertexShaderId, fragmentShaderId,
programId,buffer[3], projMatLoc,
texture[2], grassTexLoc, skyTexLoc, sphereTexLoc;

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

static const Material sphereFandB =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.9, 0.8, 0.5, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	50.0f
};

static const Material sphereMetal =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.4, 0.4, 0.4, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	100.0f
};

/*static const Material sphereBlue =
{
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0),
	50.0f
};*/

static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);
static const Light light0 =
{
	vec4(0.0, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 0.0, 0.0)
};

// Globals. // The Vec2's represent how many times the texture repeats
static Vertex fieldVertices[] =
{
	{ vec4(30.0, 0.0, 30.0, 1.0), vec2(1.0, 0.0) },
	{ vec4(30.0, 0.0, -30.0, 1.0), vec2(1.0, 1.0) },
	{ vec4(-30.0, 0.0, 30.0, 1.0), vec2(0.0, 0.0) },
	{ vec4(-30.0, 0.0, -30.0, 1.0), vec2(0.0, 1.0) }
};

static Vertex skyVerticesSIDE[4] =
{
	{ vec4(100.0, 0.0, -70.0, 1.0), vec2(1.0, 0.0) },
	{ vec4(100.0, 120.0, -70.0, 1.0), vec2(1.0, 1.0) },
	{ vec4(100.0, 0.0, 70.0, 1.0), vec2(0.0, 0.0) },
	{ vec4(100.0, 120.0, 70.0, 1.0), vec2(0.0, 1.0) }
};

static Vertex skyVertices[4] =
{
	{ vec4(100.0, 0.0, -70.0, 1.0), vec2(1.0, 0.0) },
	{ vec4(100.0, 120.0, -70.0, 1.0), vec2(1.0, 1.0) },
	{ vec4(-100.0, 0.0, -70.0, 1.0), vec2(0.0, 0.0) },
	{ vec4(-100.0, 120.0, -70.0, 1.0), vec2(0.0, 1.0) }
};


vec3 bowlPosits[] = {
	{ 0.0, 5.0, 26.0 },
	{ -10.0, 5.0, 26.0 },
	{ -13.5, 5.0, 26.0 },
	{ -16.5, 5.0, 26.0 },
	{ -19.5, 5.0, 26.0 },
	{ 10.0, 5.0, 26.0 },
	{ 13.5, 5.0, 26.0 },
	{ 16.5, 5.0, 26.0 },
	{ 19.5, 5.0, 26.0 }
};

vector<Sphere*> bowlsTest;

void ballColour(Material color) {
	//codes for OpenGL lighting SPHERE MATS
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &color.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &color.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &color.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &color.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), color.shininess);
}

void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	vertexShaderId = setShader("vertex", "vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	//codes for OpenGL lighting SPHERE MATS
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.ambRefl"), 1, &sphereFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.difRefl"), 1, &sphereFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.specRefl"), 1, &sphereFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "sphereFandB.emitCols"), 1, &sphereFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "sphereFandB.shininess"), sphereFandB.shininess);

	
	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

	// Create VAOs and VBOs... 
	glGenVertexArrays(3, vao); ///add one more object
	glGenBuffers(3, buffer);  ///add one more object
	//glGenVertexArrays(1, &vao[2]); ///add one more object
	//glGenBuffers(1, &buffer[2]);  ///add one more object

	int verCount, triCount;
	sphereVerticesNor = tSphere.GetVerData(verCount);
	sphereIndices = tSphere.GetTriData(triCount);

	// ...and associate data with vertex shader. FIELD
	glBindVertexArray(vao[Sphere::FIELD]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Sphere::FIELD_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fieldVertices), fieldVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), 0);  //layout(location=0) in vec4 fieldCoords;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(fieldVertices[0]), (void*)(sizeof(fieldVertices[0].coords)));  //layout(location=1) in vec2 fieldTexCoords;
	glEnableVertexAttribArray(1);

	// ...and associate data with vertex shader. SKY
	glBindVertexArray(vao[Sphere::SKY]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Sphere::SKY_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVertices[0]), (void*)(sizeof(skyVertices[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);

	// ...and associate data with vertex shader. SPHERE
	glBindVertexArray(vao[Sphere::SPHERE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Sphere::SPHERE_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithNormal)*verCount, sphereVerticesNor, GL_STATIC_DRAW);  ///please note the change
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[Sphere::SPHERE_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*triCount, sphereIndices, GL_STATIC_DRAW); ///please note the change
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), 0);  //layout(location=4) in vec4 fieldCoords;
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(sphereVerticesNor[0]), (GLvoid*)sizeof(sphereVerticesNor[0].normals));
	glEnableVertexAttribArray(5);

	glGenVertexArrays(1, &vao[3]); ///add one more object
	glGenBuffers(1, &buffer[3]);  ///add one more object
	glGenVertexArrays(1, &vao[4]); ///add one more object
	glGenBuffers(1, &buffer[4]);  ///add one more object

	
	// ...and associate data with vertex shader. SKY
	glBindVertexArray(vao[Sphere::SKY2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Sphere::SKY2_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVerticesSIDE), skyVerticesSIDE, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(skyVerticesSIDE[0]), 0);  //layout(location=2) in vec4 skyCoords;
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(skyVerticesSIDE[0]), (void*)(sizeof(skyVerticesSIDE[0].coords))); //layout(location=3) in vec2 skyTexCoords;
	glEnableVertexAttribArray(3);
	
	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");   //uniform mat4 projMat;
	projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Obtain modelview matrix uniform and object uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");   //uniform mat4 modelViewMat;
	objectLoc = glGetUniformLocation(programId, "object");  //uniform uint object;

	//Texture Stuff
	// Load the images.
	image[0] = getbmp("./Textures/grassWithDitch1.bmp");
	image[1] = getbmp("./Textures/sky.bmp");

	// Create texture ids.
	glGenTextures(2, texture);

	// Bind grass image.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0);

	// Bind sky image.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[1]->sizeX, image[1]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[1]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	skyTexLoc = glGetUniformLocation(programId, "skyTex");
	glUniform1i(skyTexLoc, 1);


}

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate and update modelview matrix.
	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Draw field.
	glUniform1ui(objectLoc, Sphere::FIELD);  //if (object == FIELD)
	glBindVertexArray(vao[Sphere::FIELD]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Gravel transform
	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(40.0, 0.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Draw gravel.
	glUniform1ui(objectLoc, Sphere::GRAVEL);  //if (object == FIELD)
	glBindVertexArray(vao[Sphere::GRAVEL]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Sky Front Transform
	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(0.0, -30.0, 35.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));


	// Draw sky.
	glUniform1ui(objectLoc, Sphere::SKY);  //if (object == SKY)
	glBindVertexArray(vao[Sphere::SKY]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//SKy Side Transform
	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(-60.0, -30.0, 0.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Draw sky.
	glUniform1ui(objectLoc, Sphere::SKY2);  //if (object == SKY)
	glBindVertexArray(vao[Sphere::SKY2]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	

	//Sky Front Transform
	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(-25.0, -30.0, 104.0));
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Draw sky.
	glUniform1ui(objectLoc, Sphere::SKY);  //if (object == SKY)
	glBindVertexArray(vao[Sphere::SKY]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



	int triCount;
	sphereIndices = tSphere.GetTriData(triCount);

	int ill = 0;

	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			ballColour(sphereFandB);
		}
		if (i > 0 && i < 5) {
			ballColour(sphereMetal);
		}
		//if(i > 4) {
			//ballColour(sphereBlue);
		//}
		modelViewMat = mat4(1.0);
		modelViewMat = lookAt(vec3(0.0 + GameObject::d6, 10.0 + GameObject::d3, 15.0 + GameObject::d2), vec3(0.0 + GameObject::d4, 10.0 + GameObject::d, 0.0 + GameObject::d5), vec3(0.0, 1.0, 0.0));
		if (i > 0) {

			if (i == 1) {
				modelViewMat = translate(modelViewMat, vec3(bowlsTest[i]->positionC.x, -bowlsTest[i]->positionC.y, bowlsTest[i]->positionC.z));
			}
			else {
				modelViewMat = translate(modelViewMat, vec3(bowlsTest[i]->positionC.x, -bowlsTest[i]->positionC.y, bowlsTest[i]->positionC.z));
			}
		}
		if (i == 0) {
			modelViewMat = translate(modelViewMat, vec3(bowlsTest[i]->positionC.x, -bowlsTest[i]->positionC.y+1, bowlsTest[i]->positionC.z));
			modelViewMat = scale(modelViewMat, vec3(0.8, 0.8, 0.8));
		}

		glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));  //send tp the shader
		glUniform1ui(objectLoc, Sphere::SPHERE);  //if (object == SPHERE)
		glBindVertexArray(vao[Sphere::SPHERE]);
		glDrawElements(GL_TRIANGLE_STRIP, triCount, GL_UNSIGNED_INT, sphereIndices);  ///use index array to control triangle drawing

		ill += 3.5;

		}

	glutSwapBuffers();
}

void update() {
	
	glutPostRedisplay();
}

// window resize
void windowResize(int w, int h)
{
	glViewport(0, 0, w, h);
}

int count2 = 1;

//  Ascii keys input
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'c':
		if (CameraMode) {
			CameraMode = false;
			GameObject::d6 = -35.0f;
			GameObject::d3 = 19.5f;
			GameObject::d2 = -10.0f;

			GameObject::d4 = 300.0f;
			GameObject::d = -200.0f;
			GameObject::d5 = -0.0f;
		}
		else {
			CameraMode = true;
			GameObject::d6 = 0;
			GameObject::d3 = 20;
			GameObject::d2 = -14.99f;

			GameObject::d4 = 0;
			GameObject::d = -800.0f;
			GameObject::d5 = 0;
		}
		break;
	default:
		break;
	}
}

// Non Ascii keys input
void sKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		//if (d > -50.0) d -= 0.1;
		GameObject::d3 -= 0.5f;
		cout << GameObject::d3 << endl;
	}
	if (key == GLUT_KEY_DOWN)
	{
		//if (d < 15.0) d += 0.1;
		GameObject::d3 += 0.5f;
		cout << GameObject::d3 << endl;
	}
	glutPostRedisplay();
}

// Prints incstructions and info to C++ Console window.
void consolePrint()
{
	
	cout << "-------------------------------" << endl;
	cout << "    --**-- CONTROLS --**--    " << endl;
	cout << "-------------------------------" << endl;
	cout << "Press C to switch camera angles" << endl;
	cout << "Use the Up and Down arrows to move the camera" << endl;

}

int main(int argc, char **argv) {

	consolePrint();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Pequante game");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(windowResize);
	glutIdleFunc(update); // anim/update function
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(sKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	for (int i = 0; i < 9; i++) {
		bowlsTest.push_back(new Sphere(bowlPosits[i], i));
	}
	setup();

	glutMainLoop();

}