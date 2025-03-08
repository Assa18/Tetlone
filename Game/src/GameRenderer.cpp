#include "GameRenderer.h"


static RenderData s_RenderData;

static GameData s_GameData;

GameRenderer::GameRenderer()
{
	s_RenderData.maxTileCount = 500;
	s_RenderData.maxVertexCount = s_RenderData.maxTileCount * 4;
	s_RenderData.maxIndexCount = s_RenderData.maxTileCount * 6;
}

GameRenderer::~GameRenderer()
{

}

void GameRenderer::Initialize()
{
	m_Shader.Initialize("res/shaders/vertex_scene.glsl", "res/shaders/fragment_scene.glsl");

	s_RenderData.vertices = new Vertex[s_RenderData.maxVertexCount];
	s_RenderData.indices = new uint32_t[s_RenderData.maxIndexCount];

	uint32_t offset = 0;
	for (int i = 0; i < s_RenderData.maxIndexCount; i += 6)
	{
		s_RenderData.indices[i + 0] = 0 + offset;
		s_RenderData.indices[i + 1] = 1 + offset;
		s_RenderData.indices[i + 2] = 2 + offset;
		s_RenderData.indices[i + 3] = 2 + offset;
		s_RenderData.indices[i + 4] = 3 + offset;
		s_RenderData.indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glGenVertexArrays(1, &s_RenderData.VAO);
	glBindVertexArray(s_RenderData.VAO);

	glGenBuffers(1, &s_RenderData.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.VBO);
	glBufferData(GL_ARRAY_BUFFER, s_RenderData.maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texIndex));
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &s_RenderData.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.maxIndexCount * sizeof(uint32_t), s_RenderData.indices, GL_STATIC_DRAW);

	m_Shader.Bind();

	int samplers[16];
	for (int i = 0; i < 16; i++)
		samplers[i] = i;

	glUniform1iv(glGetUniformLocation(m_Shader.GetId(), "uTextures"), 16, samplers);

	// white texture for the simple colored quads
	glCreateTextures(GL_TEXTURE_2D, 1, &s_RenderData.whiteTexture);
	glBindTexture(GL_TEXTURE_2D, s_RenderData.whiteTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	uint32_t color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	s_RenderData.textureSlots[0] = s_RenderData.whiteTexture;
}


void GameRenderer::BeginRender()
{
	s_RenderData.indicesCount = 0;

	s_RenderData.vertexIndexPtr = s_RenderData.vertices;
}

void GameRenderer::EndRender()
{
	Draw();
}

void GameRenderer::Draw()
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLsizeiptr size = (uint8_t*)s_RenderData.vertexIndexPtr - (uint8_t*)s_RenderData.vertices;

	m_Shader.SetMat4("viewproj", s_GameData.Camera.GetViewProjMatrix());
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_RenderData.vertices);


	glBindVertexArray(s_RenderData.VAO);
	glDrawElements(GL_TRIANGLES, s_RenderData.indicesCount, GL_UNSIGNED_INT, 0);
}

void GameRenderer::Quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
	float texIndex = 0.0f;

	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x + size.x, pos.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x + size.x, pos.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x, pos.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;

	s_RenderData.indicesCount += 6;
}

void GameRenderer::Quad(const glm::vec2& pos, const glm::vec2& size, int texId)
{
	glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x + size.x, pos.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(1.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x + size.x, pos.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(1.0f, 1.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(pos.x, pos.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 1.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;

	s_RenderData.indicesCount += 6;
}

void GameRenderer::Destroy()
{
	glDeleteBuffers(1, &s_RenderData.VBO);
	glDeleteBuffers(1, &s_RenderData.EBO);
	glDeleteVertexArrays(1, &s_RenderData.VAO);

	delete[] s_RenderData.vertices;
}