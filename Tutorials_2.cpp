
#include <iostream>
#include <fstream>
#include <vector>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

bool PrintShaderInfoLog(GLuint shader, const char* msg)
{
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		std::cerr << "Compile Error in " << msg << std::endl;
	}

	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		std::vector<GLchar> infoLog(infoLength);
		glGetShaderInfoLog(shader, infoLength, NULL, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<bool>(status);
}

bool ReadShaderFile(const char* fileName, std::vector<GLchar>& code)
{
	std::ifstream file(fileName, std::ios::binary);
	if(file.fail()){
		std::cerr << "Error�FCan't open source file�F" << fileName << std::endl;
		return false;
	}

	file.seekg(0, std::ios::end);
	int length = file.tellg();

	code.resize(length + 1);

	file.seekg(0, std::ios::beg);
	file.read(code.data(), length);
	code[length] = '\0';

	if(file.fail()){
		std::cerr << "Error�FCould not read source file�F" << fileName << std::endl;
		return false;
	}

	file.close();
	return true;
}

GLuint CreateShader(const char* vertexSource, const char* fragSource)
{
	const GLuint program = glCreateProgram();
	if(vertexSource != NULL){
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		if(PrintShaderInfoLog(vertexShader, "vertex shader")){ 
			glAttachShader(program, vertexShader);
		}
		glDeleteShader(vertexShader);
	}

	if(fragSource != NULL){
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragSource, NULL);
		glCompileShader(fragShader);

		if(PrintShaderInfoLog(fragShader, "fragment shader")){
			glAttachShader(program, fragShader);
		}
		glDeleteShader(fragShader);
	}

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		std::cerr << "Program Link Error." << std::endl;
	}

	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if(infoLength > 1){
		std::vector<GLchar> infoLog(infoLength);
		glGetProgramInfoLog(program, infoLength, NULL, &infoLog[0]);
		std::cout << &infoLog[0] << std::endl;

		// �v���O�����I�u�W�F�N�g���쐬�ł��Ȃ����0�ŕԂ�
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

GLuint LoadShader(const char* vertexFile, const char* fragFile)
{
	// ���_�V�F�[�_
	std::vector<GLchar> vertexSource;
	const bool vertexRet = ReadShaderFile(vertexFile, vertexSource);

	// ��f�V�F�[�_
	std::vector<GLchar> fragSource;
	const bool fragRet = ReadShaderFile(fragFile, fragSource);

	return (vertexRet && fragRet) ? CreateShader(vertexSource.data(), fragSource.data()) : 0;
}

int main()
{
	// GLFW�̏�����
	if (!glfwInit()) {
		std::cerr << "GLFW�̏������Ɏ��s���܂���" << std::endl;
		return -1;
	}

	// �E�B���h�E�̍쐬
	GLFWwindow* const window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	if (window == NULL) {
		std::cerr << "�E�B���h�E�̍쐬�Ɏ��s���܂���" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	const int version = gladLoadGL(glfwGetProcAddress);
	std::cout << "OpenGL version " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

	static const GLfloat vertexData[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// ���_�z��I�u�W�F�N�g
	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// ���_�o�b�t�@�I�u�W�F�N�g
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// ���_��OpenGL�ɓn��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	GLuint program = LoadShader("Sample.vert", "Sample.frag");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}