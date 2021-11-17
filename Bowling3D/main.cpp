// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<math.h>
#include<vector>
#include<fstream>
#include<string>
using namespace std;

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"
#include "Camera/camera.h"

//variables
GLFWwindow* window;
int width = 1024, height = 768;

glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 10.0f);
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera *camera = new Camera(cameraPos, cameraDir, cameraUp);

std::vector<glm::vec3> positionsPins;

double Xpos=0, Ypos = 0;
float deltaTime = 0.f;
float lastFrame = 0.f;

float sphereSpeed = 0.f;
float sphereRadius = 1.f;

float posxx=0.f;
float posyy=0.f;
bool launched = false;

int score = 0;
bool executingHack = false;
glm::vec3 spherePosition = glm::vec3(0.f);
bool reseting = false;



void window_callback(GLFWwindow* window, int new_width, int new_height) {
glViewport(0, 0, new_width, new_height); height = new_height; width = new_width;}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
	Xpos = xpos;
	Ypos = ypos;
	posxx = Xpos / (width / 2) - 1;
	posyy = 1 - Ypos / (height / 2);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		reseting = true;
}



void processInput(GLFWwindow *window)
{
	float cameraSpeed = 10.0f * deltaTime;
	float rotationSpeed = 30.f* deltaTime;


	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->translateFront(cameraSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->translateFront(-cameraSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera->translateUp(cameraSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		camera->translateUp(-cameraSpeed );
	}
	else
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->translateRight(cameraSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->translateRight(-cameraSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->rotateOx(rotationSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->rotateOx(-rotationSpeed);
	}else
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->rotateOy(rotationSpeed);
	}
	else
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->rotateOy(-rotationSpeed);
	}

	



}
bool checkCollision(glm::vec3 spherePosition, float sphereRadius, glm::vec3 cylinderPosition,float cylinderRadius,float cylinderHeigth) {
	//get spehere collision using position and radius
	
	glm::vec3 sphereCollision[2];
	sphereCollision[0] = spherePosition - sphereRadius;
	sphereCollision[1] = spherePosition + sphereRadius;//AR TREBUI SCHIMBAT AICI CA MERGEM PE -Z

	glm::vec3 cylinderCollision[2];
	cylinderCollision[0] = glm::vec3(cylinderPosition.x - cylinderRadius, cylinderPosition.y - cylinderHeigth / 2, cylinderPosition.z - cylinderRadius);
	cylinderCollision[1] = glm::vec3(cylinderPosition.x + cylinderRadius, cylinderPosition.y + cylinderHeigth / 2, cylinderPosition.z + cylinderRadius);

	//get cylinders collision again using position heigth and radius


	//chechk if intersect 
	if (!(sphereCollision[0].x <= cylinderCollision[1].x && sphereCollision[1].x >= cylinderCollision[0].x)) {
		return false;
	}
	if (!(sphereCollision[0].y <= cylinderCollision[1].y && sphereCollision[1].y >= cylinderCollision[0].y)) {
		
		return false;
	}
	if (!(sphereCollision[0].z <= cylinderCollision[1].z && sphereCollision[1].z >= cylinderCollision[0].z)) {
		return false;
	}
	
	return true;


	//(StartA <= EndB) and (EndA >= StartB) -intersect intervals
}
void resetStage(std::vector<glm::vec3>& PositionsPins) {
	PositionsPins = positionsPins;
	launched = false;
	sphereSpeed = 0.f;
	executingHack = false;
	spherePosition = glm::vec3(0.f, sphereRadius, 0.f);
	camera->setCameraPosition(glm::vec3(0.0f, 3.0f, 10.0f));
	camera->setCameraViewDirection(glm::vec3(0.0f, 0.0f, -1.0f));
	camera->setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
	score = 0;

	//camera Right Set


}


int main(void)
{
	
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "3D demo", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	// More about this in the following labs
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//---------------------------------------------------------------------------CIRCLE
	double PI = 2 * acos(0.0);
	double TAU = 2 * PI;



	std::vector<glm::vec3> positionsSphere = std::vector<glm::vec3>();
	std::vector<unsigned int> indicesSphere = std::vector<unsigned int>();
	sphereRadius = 1.f;
	spherePosition = glm::vec3(0.f, sphereRadius, 0.f);

	unsigned int ySegments = 20, xSegments = 20;
	for (unsigned int y = 0; y <= ySegments; ++y)
	{
		for (unsigned int x = 0; x <= xSegments; ++x)
		{
			float xSegment = (float)x / (float)xSegments;
			float ySegment = (float)y / (float)ySegments;
			float xPos =sphereRadius* std::cos(xSegment * TAU) * std::sin(ySegment * PI); // TAU is 2PI
			float yPos = sphereRadius * std::cos(ySegment * PI);
			float zPos = sphereRadius * std::sin(xSegment * TAU) * std::sin(ySegment * PI);

			positionsSphere.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < ySegments; ++y)
	{
		for (int x = 0; x < xSegments; ++x)
		{
			indicesSphere.push_back((y + 1) * (xSegments + 1) + x);
			indicesSphere.push_back(y       * (xSegments + 1) + x);
			indicesSphere.push_back(y       * (xSegments + 1) + x + 1);

			indicesSphere.push_back((y + 1) * (xSegments + 1) + x);
			indicesSphere.push_back(y       * (xSegments + 1) + x + 1);
			indicesSphere.push_back((y + 1) * (xSegments + 1) + x + 1);
		}
	}

	//----------------------------------------------------------------------------CYLINDER
	std::vector<glm::vec3> positionsCylinder = std::vector<glm::vec3>();
	std::vector<unsigned int> indicesCylinder = std::vector<unsigned int>();

	unsigned int cylinderSegments = 20;//x is for cylinder
	
	float cylinderRadius = 0.35f;
	float cylinderHeigth = 2.5f;
	positionsCylinder.push_back(glm::vec3(0, cylinderHeigth / 2, 0));
	for (unsigned int x = 0; x < cylinderSegments; ++x)
	{
		float xSegment = (float)x / (float)cylinderSegments;
		float xPos = cylinderRadius * std::cos(xSegment * TAU); // TAU is 2PI
		float yPos = cylinderRadius * std::sin(xSegment * TAU);


		positionsCylinder.push_back(glm::vec3(xPos, cylinderHeigth / 2, yPos));
	}

	positionsCylinder.push_back(glm::vec3(0, -cylinderHeigth / 2, 0));
	for (unsigned int x = 0; x < cylinderSegments; ++x)
	{
		float xSegment = (float)x / (float)cylinderSegments;
		float xPos = cylinderRadius * std::cos(xSegment * TAU); // TAU is 2PI
		float yPos = cylinderRadius * std::sin(xSegment * TAU);


		positionsCylinder.push_back(glm::vec3(xPos, -cylinderHeigth / 2, yPos));
	}



	for (int x = 1; x < positionsCylinder.size() / 2 - 1; ++x)
	{
		indicesCylinder.push_back(0);
		indicesCylinder.push_back(x);
		indicesCylinder.push_back(x + 1);
	}
	indicesCylinder.push_back(0);
	indicesCylinder.push_back(positionsCylinder.size() / 2 - 1);
	indicesCylinder.push_back(1);



	for (int x = positionsCylinder.size() / 2; x < positionsCylinder.size() - 1; ++x)
	{
		indicesCylinder.push_back(positionsCylinder.size() / 2);
		indicesCylinder.push_back(x);
		indicesCylinder.push_back(x + 1);
	}

	indicesCylinder.push_back(positionsCylinder.size() / 2);
	indicesCylinder.push_back(positionsCylinder.size() - 1);
	indicesCylinder.push_back(positionsCylinder.size() / 2 + 1);



	for (int x = 1; x < positionsCylinder.size() / 2 - 1; ++x)
	{
		indicesCylinder.push_back(x);
		indicesCylinder.push_back(x + 1);
		indicesCylinder.push_back(positionsCylinder.size() / 2 + x);


		indicesCylinder.push_back(positionsCylinder.size() / 2 + x);
		indicesCylinder.push_back(positionsCylinder.size() / 2 + x + 1);
		indicesCylinder.push_back(x + 1);
	}


	indicesCylinder.push_back(positionsCylinder.size() / 2 - 1);
	indicesCylinder.push_back(1);
	indicesCylinder.push_back(positionsCylinder.size() - 1);



	indicesCylinder.push_back(positionsCylinder.size() / 2 + 1);
	indicesCylinder.push_back(positionsCylinder.size() - 1);
	indicesCylinder.push_back(1);


	glm::vec3 positionsFloor[] = {
		glm::vec3(4.f,0.f,1.f),
		glm::vec3(-4.f,0.f,1.f),
		glm::vec3(-4.f,0.f,-20.f),
		glm::vec3(4.f,0.f,-20.f)
	};
	unsigned int indicesFloor[] = {
		0,3,2,
		0,2,1
	};

	
	int rows = 4;



	positionsPins.push_back(glm::vec3(0.f, cylinderHeigth / 2 , -20.f + (rows)*(cylinderRadius * 2 +0.25f)));
	for (int i = 1; i < rows; ++i) {
		if ((i + 1) % 2) {
			positionsPins.push_back(glm::vec3(0.f, cylinderHeigth / 2 , -20.f + (rows - i)*(cylinderRadius * 2 +0.25f) ));
		}
		for (int j = 0; j < (i+1)/2; ++j) {
			float shift = (j+1)*(2*cylinderRadius+cylinderRadius);
			if (i % 2 == 0 || j%2==1) {
				shift += 2* cylinderRadius +cylinderRadius;
			}
			positionsPins.push_back(glm::vec3(shift, cylinderHeigth/2 , -20.f + (rows-i) * (cylinderRadius * 2 + 0.25f) ));
			positionsPins.push_back(glm::vec3(-shift, cylinderHeigth / 2 , -20.f + (rows-i) * (cylinderRadius * 2 + 0.25f) ));
		}

	}
	std::vector<glm::vec3> stagePositionsPins = positionsPins;




	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store 
	// the information for a complete rendered object.
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionsSphere[0])*positionsSphere.size(), &positionsSphere[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSphere[0])*indicesSphere.size(), &indicesSphere[0], GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	


	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, window_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	// Check if the window was closed
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;
	float startTime=0.f;
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !launched && !executingHack) {
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
					if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
						score = 9000;
						stagePositionsPins[0].x += 2.f;
						stagePositionsPins[8].x += 4.f;
						stagePositionsPins[3].y += 8.f;
						stagePositionsPins[0].z += 12.f;
						stagePositionsPins[2].x += 2.f;
						stagePositionsPins[1].y += 4.f;
						executingHack = true;
						startTime = glfwGetTime();
						spherePosition.z = 12.f;
						sphereSpeed = 1.f;


					}
				}
			}
		}

		if (reseting || (spherePosition.z < -40.f && !executingHack)) {
			reseting = false;
				std::cout << "Your score was: " << score << ".\nCONGRATULATIONS\n\n";
				resetStage(stagePositionsPins);
		}
		if (executingHack) {
			if (glfwGetTime() - startTime < 7.f)
				std::cout << "How did you do that it's imposible!\nYOUR SCORE IS OVER:" << score << "!\n\n";
			else if (glfwGetTime() - startTime <= 14.f)
				std::cout << "OVER HIGH SCORE DETECTED...INSERTING HACK ON COMPUTER...\nRESETING IN:" << 14.f - (glfwGetTime() - startTime) << "\n\n";
			else std::cout << "BYE!";
		}
		if (launched) {
			
			processInput(window);			
		}
		else if(posyy<=0.f) spherePosition.x = 4 * posxx;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !launched && posyy <= 0.f) {
			launched = true;
			sphereSpeed = 0.03f;
		}
		
		


		view = glm::lookAt(camera->getCameraPosition(), camera->getCameraViewDirection() + camera->getCameraPosition(), camera->getCameraUp());
		//calculate delta time 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		projection = glm::perspective(45.0f, (float)width /
			height, 0.1f, 10000.0f);


		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		//add view and projection matrices
			model=glm::mat4(1.f);
			spherePosition.z -= sphereSpeed;
			
			model = glm::translate(model, glm::vec3(spherePosition));
			if (sphereSpeed != 0.f)
				model = glm::rotate(model, (float)glfwGetTime()*-135.f, glm::vec3(1.f, 0.f, 0.f));

			model = projection * view*model;







			//calculate MVP matrix
			glUniformMatrix4fv(glGetUniformLocation(programID, "transform"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform4fv(glGetUniformLocation(programID, "color"), 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));

			glBufferData(GL_ARRAY_BUFFER, sizeof(positionsSphere[0])*positionsSphere.size(), &positionsSphere[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSphere[0])*indicesSphere.size(), &indicesSphere[0], GL_STATIC_DRAW);

			glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_INT, 0);
			

			glBufferData(GL_ARRAY_BUFFER, sizeof(positionsCylinder[0])*positionsCylinder.size(), &positionsCylinder[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCylinder[0])*indicesCylinder.size(), &indicesCylinder[0], GL_STATIC_DRAW);
			
			
			
			
			for (int i = 0; i < stagePositionsPins.size(); ++i) {
				if (checkCollision(spherePosition, sphereRadius, stagePositionsPins[i], cylinderRadius, cylinderHeigth) && !executingHack) {
					stagePositionsPins.erase(stagePositionsPins.begin()+i);
					++score;
					continue;
					
				}
				model = glm::mat4(1.f);
				model = glm::translate(model, stagePositionsPins[i]);
				model = projection * view*model;
				glUniformMatrix4fv(glGetUniformLocation(programID, "transform"), 1, GL_FALSE, glm::value_ptr(model));
				glUniform4fv(glGetUniformLocation(programID, "color"), 1, glm::value_ptr(glm::vec4(0.8f, 0.9f, 0.9f, 1.0f)));
				
				glDrawElements(GL_TRIANGLES, indicesCylinder.size(), GL_UNSIGNED_INT, 0);
			}


			model = projection * view;
			glUniformMatrix4fv(glGetUniformLocation(programID, "transform"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform4fv(glGetUniformLocation(programID, "color"), 1, glm::value_ptr(glm::vec4(0.5f, 0.35f, 0.05f, 1.0f)));
			glBufferData(GL_ARRAY_BUFFER, sizeof(positionsFloor), &positionsFloor[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesFloor), indicesFloor, GL_STATIC_DRAW);
			glDrawElements(GL_TRIANGLES, sizeof(indicesFloor)/sizeof(indicesFloor[0]), GL_UNSIGNED_INT, 0);
		


	}

	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

