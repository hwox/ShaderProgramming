#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SimpleVelShader = CompileShaders("./Shaders/SimpleVal.vs", "./Shaders/SimpleVal.fs");
	m_SinTrailShader = CompileShaders("./Shaders/SimpleVal.vs", "./Shaders/SimpleVal.fs");
	m_ffSandBox = CompileShaders("./Shaders/ffSandBox.vs", "./Shaders/ffSandBox.fs");
	//m_FillAllShader = CompileShaders("./Shaders/FillAll.vs", "./Shaders/FillAll.fs");
	m_TextureShader = CompileShaders("./Shaders/TextureShader.vs", "./Shaders/TextureShader.fs");
	m_DrawNumberShader = CompileShaders("./Shaders/DrawNumber.vs", "./Shaders/DrawNumber.fs");
	m_SpriteAnim = CompileShaders("./Shaders/sprite.vs", "./Shaders/sprite.fs");
	m_VSSandboxShader = CompileShaders("./Shaders/VSSandbox.vs", "./Shaders/VSSandbox.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	// Load Textures
	m_ParticleTexture = CreatePngTexture("./Textures/rgb.png");
	m_ParticleTexture1 = CreatePngTexture("./Textures/particle.png");
	m_ParticleTexture2 = CreatePngTexture("./Textures/particle.png");
	m_KirbyTexture[1] = CreatePngTexture("./Textures/kirby1.png");
	m_KirbyTexture[2] = CreatePngTexture("./Textures/kirby2.png");
	

	m_KirbysTexture = CreatePngTexture("./Textures/kirby2.png");
	m_NumbersTexture = CreatePngTexture("./Textures/Numbers.png");
	BindNumberTexture();

	m_SpriteAnimWalk = CreatePngTexture("./Textures/dog.png");

	//GenQuadsVBO(1000, false, &m_VBOQuads, &m_VBOQuadsCount);
	//GenQuadsVBO(1000, false, &m_VBOQuads, &m_VBOQuadsCount);
	CreateProxyGeometry();
	CreateTextures();
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.01f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5, //x,y,z, value
		-size,size, 0.f, 0.5,
		size, size, 0.f, 0.5,//Triangle1

		-size, -size, 0.f,  0.5,
		size, size, 0.f, 0.5,
		size, -size, 0.f, 0.5//Triangle2
	};

	glGenBuffers(1, &m_VBORect); //���ؽ� ���� ������Ʈ�� ������ ������ 1���� ū ���� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);


	size = 0.5f;
	float texRect[]
		=
	{
		-size, -size, 0.f, 0.0f, 0.0f, // x y z u v
		-size,size, 0.f, 0.f, 1.0f,
		size, size, 0.f, 1.0f, 1.0f,

		-size, -size, 0.f, 0.0f, 0.0f,
		size, size, 0.f, 1.0f, 1.0f,
		size, -size, 0.f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_VBOTextureRect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRect), texRect, GL_STATIC_DRAW);

	//lecture2
	//float tri[]
	//	=
	//{
	//	-1.0, -0.0, 0.f, 1.0, 0.0, 0.f, 0.0, 0.5, 0.f //Triangle1
	//};

	//glGenBuffers(1, &m_VBOTri); //���ؽ� ���� ������Ʈ�� ������ ������ 1���� ū ���� ����
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);


	float color[]
		=
	{
		1,0,0,1, //r,g,b,a
		1,0,0,1,
		1,0,0,1,//Triangle1

		1,0,0,1,
		1,0,0,1,
		1,0,0,1//Triangle2
	};
	glGenBuffers(1, &m_VBORectColor); //���ؽ� ���� ������Ʈ�� ������ ������ 1���� ū ���� ����
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);


}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��

	// gl_������Ʈ_���̴��� ���̴� Ÿ�� ���� 
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

float g_Time = 0.f;

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);



	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	glUniform1f(uTime, g_Time);// 1.f);

	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	//GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect); //�� 18���� float point�� ������.
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); //�׸��� vbo�� �� �ִ� ���� 3���� ������ sizeof(float)*3�� �پ �о��
	//�� �ڸ��� 0���� ������ �˾Ƽ� 3ĭ�� ���� �ϴ±���! �˰� �������� �۵���
	//�ι�° ���ڰ� 4��°� 4���� �а�, sizeof(float)*4 ĭ�� �پ��


	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(0);

}
void Renderer::Lecture2()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri); //�� 18���� float point�� ������.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); //�� 18���� float point�� ������.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); //�׸��� vbo�� �� �ִ� ���� 3���� ������ sizeof(float)*3�� �پ �о��
	//�� �ڸ��� 0���� ������ �˾Ƽ� 3ĭ�� ���� �ϴ±��� �˰� �������� �۵���

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);

	glDisableVertexAttribArray(0);
}
void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri); //�� 18���� float point�� ������.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh); //�� 18���� float point�� ������.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); //�׸��� vbo�� �� �ִ� ���� 3���� ������ sizeof(float)*3�� �پ �о��
	//�� �ڸ��� 0���� ������ �˾Ƽ� 3ĭ�� ���� �ϴ±��� �˰� �������� �۵���

	//glDrawArrays(GL_TRIANGLES, 0, m_GridMesh_count);
	glDrawArrays(GL_LINE_STRIP, 0, m_GridMesh_count);

	glDisableVertexAttribArray(0);
}
void Renderer::Lecture4()
{

	glUseProgram(m_SimpleVelShader);

	// utime�� �Ϸ��� �ϴ� �������� �޾ƿ;� ��
	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time"); //

	// �������� �̿��ؼ� �ܺο��� �� ����
	// ���� float����Ʈ �ϳ����� ���߿��� ���Ͷ���� �׷��� ����������
	// ppt�� ���� 2������ �÷�����Ʈ glUniform2f �̷�����
	glUniform1f(uTime, g_Time);

	g_Time += 0.0001;


	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");

	// ���� ���°� ����? ��ӿ� ������ �򰥸��� ����
	// get�ؼ� �޾ƿ� id�� ����ٰ� �־��ִ� �� 
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// �� �����͵� ��������. aPos �־��ִ°�

	//glVertexAttribPointer(aPos , 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 
	// �׸��� ���� ���� sizeof(float)*3 ���
	// x y z x1 y1 z1 ... ������ �̷��� �ƴµ� �ӵ��� �߰��ϴϱ� 
	// x y z vx vy vz x1 y1 z1 vx vy vz... �̷��� ���� 
	// ���� 3�̸� �״��� ���� ���� x y z �̱��� ��� �����ϰ� �ӵ��� ���������� �о���� ��
	// ( x1, y1, z1�� �������������� �д� �� �ƴϰ�)
	// �׷��� x y z ������ sizeof(float)*3 �긦 vz������ �ٲ�߸� x y z �� ���� �а� �� ���� x1 y1 z1 �� ���� �а� �Ǵ� ��
	// �׷��� 6���� �ٲ��� �Ѵ�.
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);


	// glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); ������ 6���� �ٲٰ� �긦 �߰��߾�
	// �ٵ� �̻��·� �׳� apos ���� �ٲٸ� �߸��� ���� �о��.
	// �׷��� ���� ���������� x y z �������� �ٲ���� ��
	// ���������� sizeof(float)*6 ���� 3��ŭ �� �о���� �ϴϱ� (GLvoid*)(sizeof(float)*3) glvoid �÷μ� 3��°���� �����Ѵٰ�
	// �˷���� ��
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);

}
void Renderer::Lecture5()
{
	// 8�� ¥��
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time"); //
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");
	glUniform1f(uTime, g_Time);
	g_Time += 0.0001;


	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimpleVelShader, "a_StartLife");


	// 

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (x,y,z,vx,vy,vz,s,l,     x,y,z,vx,vy,vz,s,l...)
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);

	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);

}
void Renderer::Lecture6()
{

	GLuint shader = m_SinTrailShader;


	// ���İ� �Ѵ� �κ�
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time"); //
	glUniform1f(uTime, g_Time);

	g_Time += 0.0001f;

	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	//GLuint aTemp = glGetAttribLocation(shader, "a_Temp");
	GLuint aRatio = glGetAttribLocation(shader, "a_StartLifeRatioAmp");
	GLuint aValue = glGetAttribLocation(shader, "a_value");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	//glEnableVertexAttribArray(aTemp);
	glEnableVertexAttribArray(aRatio);
	glEnableVertexAttribArray(aValue);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (x,y,z,vx,vy,vz,s,l,     x,y,z,vx,vy,vz,s,l...)
	// �� �Ƚᵵ 

	// �ټ���° ���ڴ� �갡 ������ ���� ���� �� �갡 ������ ��ġ��
	//sizeof(float) * 11 ��� �� �ϳ��� ������ 11�� �ִٴ°���
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aRatio, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aRatio);
	glDisableVertexAttribArray(aValue);
	glDisableVertexAttribArray(aColor);


}

void Renderer::Lecture7()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	GLfloat points[] = { 0,0,0,0.5,0.5,0.3,0.3,-0.2,-0.2, 0.3, 0.3 };
	GLuint shader = m_ffSandBox;

	GLuint uPoints = glGetUniformLocation(shader, "u_Points");
	glUniform2fv(uPoints, 5, points);


	// utime�� �Ϸ��� �ϴ� �������� �޾ƿ;� ��
	GLuint uTime = glGetUniformLocation(shader, "u_Time"); //
	glUniform1f(uTime, g_Time);
	g_Time += 0.0001f;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");
	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);



}
void Renderer::BindNumberTexture() {
	m_NumberTexture[0] = CreatePngTexture("./Textures/0.png");
	m_NumberTexture[1] = CreatePngTexture("./Textures/1.png");
	m_NumberTexture[2] = CreatePngTexture("./Textures/2.png");
	m_NumberTexture[3] = CreatePngTexture("./Textures/3.png");
	m_NumberTexture[4] = CreatePngTexture("./Textures/4.png");
	m_NumberTexture[5] = CreatePngTexture("./Textures/5.png");
	m_NumberTexture[6] = CreatePngTexture("./Textures/6.png");
	m_NumberTexture[7] = CreatePngTexture("./Textures/7.png");
	m_NumberTexture[8] = CreatePngTexture("./Textures/8.png");
	m_NumberTexture[9] = CreatePngTexture("./Textures/9.png");
}
void Renderer::DrawNumber(int *number) {


	GLuint shader = m_DrawNumberShader;

	glUseProgram(shader);

	// Uniform inputs
	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1iv(uNumber, 3, number);



	// vertex settings
	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_TexPos");
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);


	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float)*3));

	// Texture settings
	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);
	glActiveTexture(GL_TEXTURE0); // 0��° �ؽ��İű�ٰ� m_NumberTexture�� �����ְڴ�
	glBindTexture(GL_TEXTURE_2D, m_NumbersTexture);
	

	// Draw here
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Restore to default
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);

}
void Renderer::DrawTextureRect(GLuint tex)
{


	GLuint shader = m_TextureShader;
	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_Time);


	g_Time += 0.001f;

	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_KirbyTexture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_KirbyTexture2);
*/
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_ParticleTexture2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_Checkerboard);

	int texture_temp;

	switch ((int)g_Time % 4) {
	case 0:
		texture_temp = 0;
		break;
	case 1:
		texture_temp = 1;
		break;
	case 2:
		texture_temp = 2;
		break;
	case 3:
		texture_temp = 3;
		break;
	}


	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0);

	GLuint uTex1 = glGetUniformLocation(shader, "u_Texture1");
	glUniform1i(uTex1, 1);

	GLuint uTex2 = glGetUniformLocation(shader, "u_Texture2");
	glUniform1i(uTex2, 2);

	GLuint uTex3 = glGetUniformLocation(shader, "u_Texture3");
	glUniform1i(uTex3, 3);



	GLuint aPos = glGetAttribLocation(shader, "a_Position");

	GLuint aTex = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);

}
void Renderer::GenQuadsVBO(int count, bool bRandPos, GLuint *id, GLuint *vcount)
{

	float size = 0.01f;
	int countQuad = count;
	float verticesPerQuad = 6; //���� �ϳ� ����� ���� ���ؽ� 6�� ������
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4; // ���ؽ��� �÷�Ʈ�� �r�� �ʿ�? ����Ʈ3+�ӵ�3+2(Ÿ�ӵΰ�) + 2(ratio,amp) + 1(value) + 4(color)
	float arraySize = countQuad * verticesPerQuad * floatsPerVertex;
	float *vertices = new float[arraySize];

	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY, randZ;
		float randVelX, randVelY, randVelZ;
		float StartT, LifeT;
		float StartTMax = 6;
		float LifeTMax = 3.0f;
		float ratio, amp;
		float ratioMin = 2.f;
		float ratioThres = 4.f;
		float ampMin = -0.1f;
		float ampThres = 0.4f;
		float value = 0.0f;
		float randThres = 1.0f;

		float r, g, b, a;

		int index = i * verticesPerQuad * floatsPerVertex; //4�ιٲٸ� 24�� ��

		if (bRandPos) {

			randX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else {
			randX = 0.f;
			randY = 0.f;
		}

		int temp = rand();

		randVelX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);

		StartT = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		LifeT = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);

		ratio = ratioMin + ((float)rand() / (float)RAND_MAX)*ratioThres;
		amp = ampMin + ((float)rand() / (float)RAND_MAX)*ampThres;

		value = (((float)rand() / (float)RAND_MAX))*randThres;

		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = 1.0f;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;


		vertices[index] = randX - size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = StartT; index++;
		vertices[index] = LifeT; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
	}

	// �� ���� gpu�� ���� ������� ���� �׷��ϱ�
	// gpu�� �����ֱ� ���ؼ� vboID��� ���̵� �ϳ� �������ְ�
	// 

	GLuint vboID = 0;

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // �����ְ� 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW); // arraysize��ŭ �����ֶ�� �ϴ°�

	*vcount = countQuad * verticesPerQuad;
	*id = vboID;
	delete(vertices);
}

void Renderer::CreateProxyGeometry()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_GridMesh_count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBO_GridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::FillAll(float _in) {


	GLuint shader = m_FillAllShader;


	// ���İ� �Ѵ� �κ�
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glUseProgram(shader);


	GLuint aPos = glGetAttribLocation(shader, "a_Position");

	glEnableVertexAttribArray(aPos);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (x,y,z,vx,vy,vz,s,l,     x,y,z,vx,vy,vz,s,l...)
	// �� �Ƚᵵ 

	// �ټ���° ���ڴ� �갡 ������ ���� ���� �� �갡 ������ ��ġ��
	//sizeof(float) * 11 ��� �� �ϳ��� ������ 11�� �ִٴ°���
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);
	glDisableVertexAttribArray(aPos);


}

void Renderer::CreateTextures() {
	static const GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};


	glGenTextures(1, &m_Checkerboard);
	glBindTexture(GL_TEXTURE_2D, m_Checkerboard);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void Renderer::CreateBmpTexture() {

}

void Renderer::DrawSpriteSequence(int number) {


	GLuint shader = m_SpriteAnim;
	glUseProgram(shader);

	GLuint uNumber = glGetUniformLocation(shader, "u_Number");
	glUniform1f(uNumber, float(number));
	GLuint ux = glGetUniformLocation(shader, "u_Resolx");
	glUniform1f(ux, float(4));
	GLuint uy = glGetUniformLocation(shader, "u_Resoly");
	glUniform1f(uy, float(9));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SpriteAnimWalk);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTextureRect);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);

}

void Renderer::VSSandbox()
{
	GLuint shader = m_VSSandboxShader;
	glUseProgram(shader);

	static float gTime = 0; gTime += 0.05f;
	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, gTime);

	//GLuint aPos = glGetAttribLocation(shader, "a_Position");
	//glEnableVertexAttribArray(aPos);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//glDrawArrays(GL_TRIANGLES, 0, m_GridMesh_count);
	glDrawArrays(GL_LINE_STRIP, 0, m_GridMesh_count);
	glDisableVertexAttribArray(0);

}