#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shape.h"

// �v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
//   program�F�v���O�����I�u�W�F�N�g��
GLboolean PrintShaderInfoLog(GLuint program)
{
	// �����N���ʂ��擾����
	GLint status;
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cerr << "Link Error." << std::endl;
	}

	// �V�F�[�_�̃����N���̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if(bufSize > 1)
	{
		// �V�F�[�_�̃����N���̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

// �V�F�[�_�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
//   shader�F�V�F�[�_�I�u�W�F�N�g��
//   str�F�R���p�C���G���[�����������ꏊ�������ꏊ
GLboolean PrintShaderInfoLog(GLuint shader, const char* str)
{
	// �R���p�C�����ʂ��擾����
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cerr << "Compile Error in" << str << std::endl;
	}

	// �V�F�[�_�̃R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		// �V�F�[�_�̃R���p�C�����̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

// �v���O�����I�u�W�F�N�g���쐬����
//	  vsrc�F���_�V�F�[�_�̃\�[�X�v���O�����̕�����
//    fsrc�F��f�V�F�[�_�̃\�[�X�v���O�����̕�����
GLuint CreateProgram(const char* vsrc, const char* fsrc)
{
	// ��̃v���O�����I�u�W�F�N�g���쐬����
	const GLuint program = glCreateProgram();

	if(vsrc != NULL)
	{
		// ���_�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		const GLuint vobj = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		// ���_�V�F�[�_�̃I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if(PrintShaderInfoLog(vobj, "vertex shader"))
		{
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}

	if(fsrc != NULL)
	{
		// ��f�V�F�[�_�̃V�F�[�_�I�u�W�F�N�g���쐬����
		const GLuint fobj = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// ��f�V�F�[�_�̃I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if(PrintShaderInfoLog(fobj, "fragment shader"))
		{
			glAttachShader(program, fobj);
		}
		glDeleteShader(fobj);
	}

	// �v���O�����I�u�W�F�N�g�������N����
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	// �쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if(PrintShaderInfoLog(program))
	{
		return program;
	}

	// �v���O�����I�u�W�F�N�g���쐬�ł��Ȃ����0�ŕԂ�
	glDeleteProgram(program);
	return 0;
}

// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
//   name�F�V�F�[�_�̃\�[�X�t�@�C����
//   buffer�F�ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
bool ReadShaderSource(const char* name, std::vector<GLchar> &buffer)
{
	// �t�@�C������NULL������
	if(name == NULL){ return false; }

	// �\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if(file.fail())
	{
		std::cerr << "Error�FCan't open source file�F" << name << std::endl;
		return false;
	}

	// �t�@�C���̖����Ɉړ������݈ʒu(�t�@�C���T�C�Y)���擾����
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// �t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);

	// �t�@�C����擪����ǂݍ���
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if(file.fail())
	{
		std::cerr << "Error�FCould not read source file�F" << name << std::endl;
		file.close();
		return false;
	}

	// �ǂݍ��ݐ���
	file.close();
	return true;
}

// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬����
//   vert�F���_�V�F�[�_�̃\�[�X�t�@�C����
//   frag�F��f�V�F�[�_�̃\�[�X�t�@�C����
GLuint LoadProgram(const char* vert, const char* frag)
{
	// �V�F�[�_�̃\�[�X�t�@�C����ǂݍ���
	std::vector<GLchar> vsrc;
	const bool vstat(ReadShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(ReadShaderSource(frag, fsrc));

	// �v���O�����I�u�W�F�N�g���쐬����
	return vstat && fstat ? CreateProgram(vsrc.data(), fsrc.data()) : 0;
}

// ��`�̒��_�̈ʒu
constexpr Object::VERTEX rectangle[] = 
{
	{-0.5f, -0.5f},
	{ 0.5f, -0.5f},
	{ 0.5f,  0.5f},
	{-0.5f,  0.5f}
};

int main()
{
	// GLFW������������
	if(!glfwInit())
	{
		std::cerr << "Can't Initialize GLFW" << std::endl;
		return -1;
	}

	// �v���O�����I�����̏�����o�^����
	atexit(glfwTerminate);

	// OpenGL Version 4.6 Core Profile ��I������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window window;

	// �w�i�F���w��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// �v���O�����I�u�W�F�N�g���쐬����
	const GLuint program(LoadProgram("point.vert", "point.frag"));

	// uniform�ϐ��̏ꏊ���擾����
	const GLint sizeLoc = glGetUniformLocation(program, "size");
	const GLint scaleLoc = glGetUniformLocation(program, "scale");
	const GLint locationLoc = glGetUniformLocation(program, "location");

	// �}�`�f�[�^���쐬����
	std::unique_ptr<const Shape> shape = std::make_unique<const Shape>(2, 4, rectangle);

	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while(window)
	{
		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		// �V�F�[�_�v���O�����̎g�p�J�n
		glUseProgram(program);

		// uniform�ϐ��ɒl��ݒ肷��
		glUniform2fv(sizeLoc, 1, window.GetSize());
		glUniform1f(scaleLoc, window.GetScale());
		glUniform2fv(locationLoc, 1, window.GetLocation());

		// �}�`��`�悷��
		shape->Draw();

		// �J���[�o�b�t�@�����ւ���
		window.SwapBuffers();
	}
}