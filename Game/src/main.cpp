#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/Shader.h"

#include <iostream>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int32_t width = 750;
	int32_t height = 750;

	GLFWwindow* window = glfwCreateWindow(width, height, "Lab-1", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "ERROR: failed to crate grfw window!\n";
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: failed to load glad!\n";
		return 1;
	}

	Shader shader("res/vertex_transform.txt", "res/fragment_transform.txt");

	float vertexData[] = {
		// right square
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.4f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.4f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		// left square
		-0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.4f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f,

		// top square
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.2f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.4f, 0.1f, 0.0f, 1.0f, 1.0f, 1.0f
	};

	unsigned int indexData[] = {
		// right square
		0, 1, 2,
		0, 2, 3,

		// left square
		4, 5, 7,
		5, 6, 7,

		// top square
		8, 9, 10,
		8, 10, 11
	};

	unsigned int VAO = 0, VBO = 0, EBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader.bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 0.0f));

	shader.SetMat4("model", model);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3 * 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}