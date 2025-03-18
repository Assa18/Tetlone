#include "GameRenderer.h"

#include <iostream>

static RenderData s_RenderData;

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
	m_ScreenShader.Initialize("res/shaders/vertex_screen.glsl", "res/shaders/fragment_screen.glsl");

	m_FrameBuffer.Create(800, 800);

	TextureData texData;
	texData.minFilter = GL_REPEAT;
	texData.magFilter = GL_REPEAT;
	texData.wrapS = GL_LINEAR;
	texData.wrapT = GL_LINEAR;
	texData.slotIndex = 1;

	m_Textures["gergoo"].Load("res/textures/gergo.jpg", texData);

	texData.slotIndex = 2;
	m_Textures["krizbi"].Load("res/textures/mekdolenc2.jpg", texData);

	texData.slotIndex = 3;
	m_Textures["background"].Load("res/textures/background.jpg", texData);

	glBindTextureUnit(m_Textures["gergoo"].GetIndex(), m_Textures["gergoo"].GetID());
	glBindTextureUnit(m_Textures["krizbi"].GetIndex(), m_Textures["krizbi"].GetID());
	glBindTextureUnit(m_Textures["background"].GetIndex(), m_Textures["background"].GetID());

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

	m_ScreenShader.Bind();
	m_ScreenShader.SetInt("screenTexture", 16);

	m_Shader.Bind();
}


void GameRenderer::BeginRender()
{
	s_RenderData.indicesCount = 0;

	s_RenderData.vertexIndexPtr = s_RenderData.vertices;
}

void GameRenderer::Draw(const Camera2D& cam)
{
	m_FrameBuffer.Bind();
	m_Shader.Bind();

	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLsizeiptr size = (uint8_t*)s_RenderData.vertexIndexPtr - (uint8_t*)s_RenderData.vertices;

	m_Shader.SetMat4("viewproj", cam.GetViewProjMatrix());
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_RenderData.vertices);

	glBindVertexArray(s_RenderData.VAO);
	glDrawElements(GL_TRIANGLES, s_RenderData.indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_FrameBuffer.Unbind();
	glClear(GL_COLOR_BUFFER_BIT);

	m_ScreenShader.Bind();
	m_FrameBuffer.BindVAO();
	glBindTextureUnit(16, m_FrameBuffer.GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GameRenderer::Quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
	float texIndex = 0.0f;
	glm::vec2 posReal = glm::vec2(pos.x - 1, pos.y - 2);

	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x + size.x, posReal.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x + size.x, posReal.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x, posReal.y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = color;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texIndex;
	s_RenderData.vertexIndexPtr++;

	s_RenderData.indicesCount += 6;
}

void GameRenderer::Quad(const glm::vec2& pos, const glm::vec2& size, int texId)
{
	glm::vec2 posReal = glm::vec2(pos.x - 1, pos.y - 2);
	glm::vec4 whiteColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(0.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x + size.x, posReal.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(1.0f, 0.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x + size.x, posReal .y + size.y, 0.0f);
	s_RenderData.vertexIndexPtr->color = whiteColor;
	s_RenderData.vertexIndexPtr->texCoords = glm::vec2(1.0f, 1.0f);
	s_RenderData.vertexIndexPtr->texIndex = texId;
	s_RenderData.vertexIndexPtr++;


	s_RenderData.vertexIndexPtr->pos = glm::vec3(posReal.x, posReal.y + size.y, 0.0f);
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

void GameRenderer::OnResize(uint32_t width, uint32_t height)
{
	m_FrameBuffer.Resize(width, height);
}