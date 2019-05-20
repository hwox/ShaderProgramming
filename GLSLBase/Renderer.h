#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();

	void Lecture2(); //삼각형
	void Lecture3();
	void Lecture4();

	void Lecture5();
	void Lecture6();
	void Lecture7();
	void DrawTextureRect(GLuint tex);
	void FillAll(float);
	void CreateTextures();
	void CreateBmpTexture();
	void DrawNumber(int* number);

	// 이 숫자는 전체 sprite 의 index의미 
	void DrawSpriteSequence(int number);

	void VSSandbox();
private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	void GenQuadsVBO(int count, bool bRandPos, GLuint *id, GLuint *vcount);//랜덤한 위치에 삼각형 1000개
	void CreateProxyGeometry();

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;
	void BindNumberTexture();


	GLuint m_VBOTri = 0;
	GLuint m_VBOQuads = 0;
	GLuint m_VBOQuads1 = 0;
	GLuint m_VBOQuadsCount = 0;
	GLuint m_VBOQuadsCount1 = 0;
	
	
	GLuint m_VBO_GridMesh = 0; 
	int m_GridMesh_count = 0;

	GLuint m_VBORectColor = 0;

	GLuint m_VBOTextureRect = 0;

	//shader
	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_SinTrailShader = 0;
	GLuint m_ffSandBox = 0;
	GLuint m_FillAllShader = 0;
	GLuint m_DrawNumberShader = 0;
	GLuint m_SpriteAnim = 0;
	GLuint m_VSSandboxShader = 0;


	//textures
	GLuint m_ParticleTexture = 0;
	GLuint m_ParticleTexture1 = 0;
	GLuint m_ParticleTexture2 = 0;
	
	GLuint m_KirbyTexture[10];
	GLuint m_KirbysTexture;

	GLuint m_NumbersTexture;
	GLuint m_NumberTexture[10];

	GLuint m_SpriteAnimWalk = 0;
	//GLuint m_KirbyTexture2 = 0;

	//GLuint Textures[];



	GLuint m_TextureShader = 0;
	
	GLuint m_Checkerboard = 0;

}

;

