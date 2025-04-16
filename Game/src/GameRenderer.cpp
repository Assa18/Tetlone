#include "GameRenderer.h"
#include "Game.h"
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

static RenderData s_RenderData;

GameRenderer::GameRenderer()
{
	s_RenderData.maxTileCount = 1000;
	s_RenderData.maxVertexCount = s_RenderData.maxTileCount * 4;
	s_RenderData.maxIndexCount = s_RenderData.maxTileCount * 6;
}

GameRenderer::~GameRenderer()
{

}

void GameRenderer::Initialize()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_Shader.Initialize("res/shaders/vertex_scene.glsl", "res/shaders/fragment_scene.glsl");
	m_ScreenShader.Initialize("res/shaders/vertex_screen.glsl", "res/shaders/fragment_screen.glsl");

	m_FrameBuffer.Create(800, 800);

	LoadFontMap("res/fonts/Roboto-Regular.ttf");

	TextureData texData;
	texData.minFilter = GL_LINEAR;
	texData.magFilter = GL_LINEAR;
	texData.wrapS = GL_CLAMP_TO_EDGE;
	texData.wrapT = GL_CLAMP_TO_EDGE;
	texData.slotIndex = 2;

	m_Textures["background"].Load("res/textures/background.png", texData);

	glBindTextureUnit(1, m_FontTextureAtlasID);
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
	float offsetSgader = 1.0f / 300.0f;
	float offsets[9][2] = {
	{ -offsetSgader, offsetSgader }, // top-left
	{ 0.0f, offsetSgader }, // top-center
	{ offsetSgader, offsetSgader }, // top-right
	{ -offsetSgader, 0.0f }, // center-left
	{ 0.0f, 0.0f }, // center-center
	{ offsetSgader, 0.0f }, // center-right
	{ -offsetSgader, -offsetSgader }, // bottom-left
	{ 0.0f, -offsetSgader }, // bottom-center
	{ offsetSgader, -offsetSgader } // bottom-right
	};
	glUniform2fv(glGetUniformLocation(m_ScreenShader.GetId(), "offsets"), 9, (float*)offsets);
	
	float edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1fv(glGetUniformLocation(m_ScreenShader.GetId(), "edge_kernel"), 9, edge_kernel);

	m_Shader.Bind();
}


void GameRenderer::BeginRender()
{
	s_RenderData.indicesCount = 0;

	s_RenderData.vertexIndexPtr = s_RenderData.vertices;
}

void GameRenderer::Draw(const Camera2D& cam)
{
	glEnable(GL_BLEND);
	m_FrameBuffer.Bind();
	m_Shader.Bind();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLsizeiptr size = (uint8_t*)s_RenderData.vertexIndexPtr - (uint8_t*)s_RenderData.vertices;

	m_Shader.SetMat4("viewproj", cam.GetViewProjMatrix());
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_RenderData.vertices);

	glBindVertexArray(s_RenderData.VAO);
	glDrawElements(GL_TRIANGLES, s_RenderData.indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	m_FrameBuffer.Unbind();
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_BLEND);
	m_ScreenShader.Bind();
	m_ScreenShader.SetBool("shake", m_IsShaking);
	m_ScreenShader.SetInt("colorMode", m_ColorMode);
	m_ScreenShader.SetFloat("time", Game::GetWindow()->GetTime());
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

void GameRenderer::LoadFontMap(const char* path)
{
	const int atlasWidth = 512, atlasHeight = 512;
	unsigned char* atlasData = new unsigned char[atlasWidth * atlasHeight];
	memset(atlasData, 0, atlasWidth * atlasHeight);

	FT_Library ft;
	FT_Init_FreeType(&ft);
	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
	{
		std::cerr << "Failed to load font atlas!\n";
		exit(1);
	}
	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int xOffset = 0;
	int yOffset = 0;
	int rowHeight = 0;

	for (unsigned char c = 32; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;

		FT_GlyphSlot g = face->glyph;

		// Check if we need to move to next row
		if (xOffset + g->bitmap.width >= atlasWidth)
		{
			xOffset = 0;
			yOffset += rowHeight;
			rowHeight = 0;
		}

		for (int y = 0; y < g->bitmap.rows; y++)
		{
			for (int x = 0; x < g->bitmap.width; x++)
			{
				int flippedY = g->bitmap.rows - 1 - y; // Flip Y axis
				int atlasIndex = (xOffset + x) + (yOffset + flippedY) * atlasWidth;
				int glyphIndex = x + y * g->bitmap.width;
				atlasData[atlasIndex] = g->bitmap.buffer[glyphIndex];
			}
		}

		Character character = {
			glm::vec2(g->bitmap.width, g->bitmap.rows),
			glm::vec2(g->bitmap_left, g->bitmap_top),
			(uint32_t)(g->advance.x),
			glm::vec2((float)xOffset / atlasWidth, (float)yOffset / atlasHeight),
			glm::vec2((float)g->bitmap.width / atlasWidth, (float)g->bitmap.rows / atlasHeight)
		};

		m_Characters[c] = character;
		xOffset += g->bitmap.width + 1;
		rowHeight = std::max(rowHeight, (int)g->bitmap.rows);
	}

	glGenTextures(1, &m_FontTextureAtlasID);
	glBindTexture(GL_TEXTURE_2D, m_FontTextureAtlasID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	delete[] atlasData;
}

void GameRenderer::Text(const std::string& text, const glm::vec2& pos, const glm::vec4& color, float height)
{
	std::string::const_iterator c;
	float offset = 0.0f;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_Characters[*c];

		float xPos = pos.x + offset + ch.Bearing.x * height;
		float yPos = pos.y - (ch.Size.y - ch.Bearing.y) * height;

		float w = ch.Size.x * height;
		float h = ch.Size.y * height;
		s_RenderData.vertexIndexPtr->pos = glm::vec3(xPos, yPos, 0.0f);
		s_RenderData.vertexIndexPtr->color = color;
		s_RenderData.vertexIndexPtr->texCoords = ch.TexPos;
		s_RenderData.vertexIndexPtr->texIndex = 1.0f;
		s_RenderData.vertexIndexPtr++;

		s_RenderData.vertexIndexPtr->pos = glm::vec3(xPos + w, yPos, 0.0f);
		s_RenderData.vertexIndexPtr->color = color;
		s_RenderData.vertexIndexPtr->texCoords = { ch.TexPos.x + ch.TexSize.x, ch.TexPos.y }; 
		s_RenderData.vertexIndexPtr->texIndex = 1.0f;
		s_RenderData.vertexIndexPtr++;

		s_RenderData.vertexIndexPtr->pos = glm::vec3(xPos + w, yPos + h, 0.0f);
		s_RenderData.vertexIndexPtr->color = color;
		s_RenderData.vertexIndexPtr->texCoords = { ch.TexPos.x + ch.TexSize.x, ch.TexPos.y + ch.TexSize.y };
		s_RenderData.vertexIndexPtr->texIndex = 1.0f;
		s_RenderData.vertexIndexPtr++;

		s_RenderData.vertexIndexPtr->pos = glm::vec3(xPos, yPos + h, 0.0f);
		s_RenderData.vertexIndexPtr->color = color;
		s_RenderData.vertexIndexPtr->texCoords = { ch.TexPos.x, ch.TexPos.y + ch.TexSize.y };
		s_RenderData.vertexIndexPtr->texIndex = 1.0f;
		s_RenderData.vertexIndexPtr++;

		s_RenderData.indicesCount += 6;
		offset += (ch.Advance >> 6) * height;
	}
}