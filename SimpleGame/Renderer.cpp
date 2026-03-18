#include "stdafx.h"
#include "Renderer.h"
#include <vector>
#include <cmath>

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

	glEnable(GL_PROGRAM_POINT_SIZE); // 크기 설정

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	//Load shaders
	m_TriangleShader = CompileShaders("./Shaders/triangle.vs", "./Shaders/triangle.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::GenParticles(int Num) {
	VertexCount = Num;

	std::vector<float> particles;
	particles.reserve(Num * 7);

	float minRadius = 0.7f;
	float maxRadius = 0.9f;
	float centerX = 0.0f;
	float centerY = 0.0f;

	for (int i = 0; i < Num; ++i) {
		float rv = static_cast<float>(rand()) / RAND_MAX;
		float angle = rv * 2.0f * 3.14159265f;

		float r = minRadius + (static_cast<float>(rand()) / RAND_MAX) * (maxRadius - minRadius);

		float x = centerX + r * cos(angle);
		float y = centerY + r * sin(angle);
		float z = 0.0f;

		float mass = 0.7f + (static_cast<float>(rand()) / RAND_MAX) * 0.6f;

		float vx = -0.08f + (static_cast<float>(rand()) / RAND_MAX) * 0.16f;
		float vy = -0.05f + (static_cast<float>(rand()) / RAND_MAX) * 0.10f;

		particles.push_back(x);
		particles.push_back(y);
		particles.push_back(z);
		particles.push_back(mass);
		particles.push_back(vx);
		particles.push_back(vy);
		particles.push_back(rv);
	}
	if (m_ParticleVBO == 0) {
		glGenBuffers(1, &m_ParticleVBO);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particles.size(), particles.data(), GL_STATIC_DRAW);
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float centerX = 0;
	float centerY = 0;
	float size = 0.1;
	float mass = 1.f;
	float vx = 1;
	float vy = 1;
	float triangle[] =
	{
		centerX - size / 2,centerY - size / 2,0 ,
		mass,vx,vy, //v0
		centerX + size / 2,centerY - size / 2,0,  
		mass,vx,vy,//v1
		centerX + size / 2,centerY + size / 2,0,  
		mass,vx,vy,//v2

		centerX - size / 2,centerY - size / 2,0 ,	
		mass,vx,vy,//v0
		centerX + size / 2,centerY + size / 2,0,  
		mass,vx,vy,//v1
		centerX - size / 2,centerY + size / 2,0,
		mass,vx,vy//v2
	};
	glGenBuffers(1, &m_TriangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
	// glBufferData - 동기함수 (데이터를 올려야 하기때문) 그리는건 비동기 - GPU가 알아서 그려줌

	GenParticles(1000);
	
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done." << std::endl;

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float gTime = 0;

void Renderer::DrawTriangle()
{
	gTime += 0.001f;
	//Program select
	glUseProgram(m_TriangleShader);

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);

	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));

	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::DrawParticles()
{
	gTime += 0.001f;
	//Program select
	glUseProgram(m_TriangleShader); //같은 쉐이더 사용

	int uTime = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(uTime, gTime);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	int attribRV = glGetAttribLocation(m_TriangleShader, "a_RV");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);
	glEnableVertexAttribArray(attribRV);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float) * 3));

	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(attribRV, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLvoid*)(sizeof(float) * 6));

	glDrawArrays(GL_POINTS, 0, VertexCount);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRV);
}


void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

