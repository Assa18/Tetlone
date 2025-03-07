#include "GameRenderer.h"

GameRenderer::GameRenderer()
{

}

GameRenderer::~GameRenderer()
{

}

static unsigned int VAO = 0, VBO = 0, EBO = 0;

void GameRenderer::Initialize()
{
	m_Shader.Initialize("res/shaders/vertex_scene.glsl", "res/shaders/fragment_scene.glsl");


	float vertexData[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	unsigned int indexData[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_Shader.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	/*model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 1.0f, 0.0f));*/
	m_Shader.SetMat4("model", model);
}

void GameRenderer::Draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	m_Shader.SetMat4("viewproj", m_GameData.Camera.GetViewProjMatrix());

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}