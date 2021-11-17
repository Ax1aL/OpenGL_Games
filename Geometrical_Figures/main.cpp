// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<vector>
#include<math.h>
#include<random>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
int width = 1024, height = 768;


bool flagT = false, flagS = false, flagR = false;

float scaleValue = 1.f;
float rotationValue = 0.f;
glm::vec3 randomPosition = glm::vec3(0.0f, 0.0f, 0.f);

unsigned int triangles = 6;
double TAU = 4 * acos(0);

std::vector<glm::vec3> Positions;
std::vector<glm::vec3> hexPositions;

unsigned int offSet = 0;
unsigned int drawsTriangles = 0;

float diffScale = 0.005f;
float diffTranslate = 0.003f;
float diffRotation = 0.5f;
unsigned short spawn = 0;








void window_callback(GLFWwindow* window, int new_width, int new_height) { glViewport(0, 0, new_width, new_height); width = new_width; height = new_height; }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void resetValues(const glm::mat4& projection, const glm::mat4& view,glm::vec3& Position, float& angle,float& scale,int drawsTriangles) {
	flagT = flagS = flagR = false;
	angle =0.f;
	scale = 0.3f;
	diffScale = 0.005f;
	diffTranslate = 0.003f;
	diffRotation = 0.5f;
	Position = glm::vec3(0.2f);

	

		float x = 2 * ((float)(rand()) / RAND_MAX) - 1;
		float y = 1-2 * ((float)(rand()) / RAND_MAX);
		//std::cout << x << " " << y << std::endl<<std::endl;
		x *= (1-0.25f);
		y *= (1-0.25f);
		//x = 0.f;
		//y = 0.f;
		



		
		glm::mat4 tempModel = glm::mat4(1.f);
		tempModel = projection * view;
		tempModel = glm::inverse(tempModel);
		glm::vec4 tp = glm::vec4(x, y,0.f, 1.f);
		glm::vec4 pos = tempModel * tp;

		//pos.w = 1.0 / pos.w;

		//pos.x *= pos.w;
		//pos.y *= pos.w;
		//pos.z *= pos.w;



		//tempModel = glm::translate(tempModel, glm::vec3(x, y, 0.f));
		
		
		Position= glm::vec3(pos.x,pos.y,0.f);
		//std::cout << pos.x << " " << pos.y << std::endl << std::endl;
		
		/*Position.x = tempPos.x;
		Position.y = tempPos.y;
		Position.z = 0.f;*/

		
	
}


void checkCollision(const glm::mat4& model,const std::vector<glm::vec3>& Positions, unsigned int max, unsigned int step) {

	//model = glm::translate(model, randomPosition);
	//model = glm::scale(model, glm::vec3(scaleValue));
	//model = glm::rotate(model, rotationValue, glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i <= max; i += step) {
		glm::vec4 h_pos = model * glm::vec4(Positions[i], 1.f);
		//std::cout << h_pos.x << " " << h_pos.y << std::endl;

		if (!(h_pos.x >= -1 && h_pos.x <= 1) || !(h_pos.y >= -1 && h_pos.y <= 1)) {
			if (flagS)diffScale *= -1;
			if (flagT)diffTranslate *= -1;
			if (flagR)diffRotation *= -1;
			break;
		}
	}


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

	 unsigned int indices[] = {
		 //sq
		 0,2,4,
		 0,4,6,
		 //hex
		 0,1,2,
		 0,2,3,
		 0,3,4,
		 0,4,5,
		 //oct		
		 0,1,2,
		 0,2,3,
		 0,3,4,
		 0,4,5,
		 0,5,6,
		 0,6,7,
		
	 };
	 //std::vector<unsigned int> indc[3];
	 


	for (unsigned int i = 0; i < 8; ++i)
	{
		float Segment = (float)i / (float)8;
		float xPos = .25f*std::cos(Segment * TAU); 
		float yPos = .25f*std::sin(Segment * TAU);
		Positions.push_back(glm::vec3(xPos, yPos, 0));
	}
	
	for (unsigned int i = 0; i < 6; ++i)
	{
		float Segment = (float)i / (float)6;
		float xPos = .25f*std::cos(Segment * TAU);
		float yPos = .25f*std::sin(Segment * TAU);
		hexPositions.push_back(glm::vec3(xPos, yPos, 0));
	}




	

	

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store 
	// the information for a complete rendered object.
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0])*Positions.size(), &Positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	glfwSetFramebufferSizeCallback(window, window_callback);
	glfwSetKeyCallback(window, key_callback);





	// Check if the window was closed
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 model;
	while (!glfwWindowShouldClose(window))



	{	// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			break;
		}
		if (spawn == 0) {
			glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
			drawsTriangles = 0;
			continue;
		}


		view = glm::lookAt(glm::vec3(0.0f, 0.f, 1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		 projection = glm::perspective(45.0f, (float)width /
			height, 0.1f, 10000.0f);
		model=glm::mat4(1.f);
		model = projection * view*model;

		if (spawn==1) {//DRAW SQUARE
			spawn = 5;
			drawsTriangles = 2 * 3;
				glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0])*Positions.size(), &Positions[0], GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				resetValues(projection, view, randomPosition, rotationValue, scaleValue, drawsTriangles);
				rotationValue = 45.f;

		}
		if (spawn==2) {//DRAW HEXAGON
			spawn = 5;
			drawsTriangles = 4 * 3;
			offSet = 2 * 3;
				glBufferData(GL_ARRAY_BUFFER, sizeof(hexPositions[0])*hexPositions.size(), &hexPositions[0], GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) - offSet, indices + (offSet), GL_STATIC_DRAW);
				resetValues(projection, view, randomPosition, rotationValue, scaleValue, drawsTriangles);
		}
		if (spawn==3) {//DRAW OCTAGON
			spawn = 5;
			drawsTriangles = 6 * 3;
			offSet = 6 * 3;
				glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0])*Positions.size(), &Positions[0], GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) - offSet, indices + (offSet), GL_STATIC_DRAW);
				resetValues(projection, view, randomPosition, rotationValue, scaleValue, drawsTriangles);
			
			//start where octagon indices starts(shift 6*3)
		}
		
		
		if (flagT) {
			
			model = glm::translate(model, glm::vec3(randomPosition.x += diffTranslate, randomPosition.y, 0.f));
		}
		else model = glm::translate(model, randomPosition);

		if (flagS) {
			model = glm::scale(model, glm::vec3(scaleValue += diffScale));
		}
		else model = glm::scale(model, glm::vec3(scaleValue));
		if (flagR) {
			model = glm::rotate(model, rotationValue += diffRotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else model = glm::rotate(model, rotationValue, glm::vec3(0.0f, 0.0f, 1.0f));

		if (drawsTriangles == 2 * 3) {checkCollision(model, Positions, 6, 2);}
		else if (drawsTriangles == 4 * 3) { checkCollision(model, hexPositions, 5, 1); }
		else  if (drawsTriangles == 6 * 3) { checkCollision(model, Positions, 7, 1); }
		

		glUniformMatrix4fv(glGetUniformLocation(programID, "transform"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform4fv(glGetUniformLocation(programID, "color"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 1.0f)));


		glDrawElements(GL_TRIANGLES, drawsTriangles, GL_UNSIGNED_INT, 0);



	}






	// Cleanup VBO
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_T: {flagT = true; flagS = flagR = false;    break; }
		case GLFW_KEY_S: {flagS = true; flagT = flagR = false;    break; }
		case GLFW_KEY_R: {flagR = true; flagT = flagS = false;    break; }
		case GLFW_KEY_O: {flagR = flagT = flagS = false; spawn = 0;    break; }
		case GLFW_KEY_1: {spawn = 1; break; }
		case GLFW_KEY_2: {spawn = 2; break; }
		case GLFW_KEY_3: {spawn = 3; break; }
		}
	}

}

