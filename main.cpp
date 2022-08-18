#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"

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
	glBindAttribLocation(program, 1, "normal");
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
	const bool vstat = ReadShaderSource(vert, vsrc);
	std::vector<GLchar> fsrc;
	const bool fstat = ReadShaderSource(frag, fsrc);

	// �v���O�����I�u�W�F�N�g���쐬����
	return vstat && fstat ? CreateProgram(vsrc.data(), fsrc.data()) : 0;
}

// �ʂ��ƂɐF��ς����Z�ʑ̂̒��_����
constexpr Object::VERTEX solidCubeVertex[] = 
{
	// ��
	{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f },

	// ��
	{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	
	// ��
	{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	
	// �E
	{  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f },

	// ��
	{ -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },

	// �O
	{ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f }
};

// �Z�ʑ̖̂ʂ�h��Ԃ��O�p�`�̒��_�̃C���f�b�N�X
constexpr GLuint solidCubeIndex[] = 
{
	 0,  1,  2,  3,  4,  5, // ��
	 6,  7,  8,  9, 10, 11, // ��
	12, 13, 14, 15, 16, 17, // ��
	18, 19, 20, 21, 22, 23, // �E
	24, 25, 26, 27, 28, 29, // ��
	30, 31, 32, 33, 34, 35, // �O
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

	// �w�ʃJ�����O��L���ɂ���
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// �f�v�X�o�b�t�@��L���ɂ���
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// �v���O�����I�u�W�F�N�g���쐬����
	const GLuint program = LoadProgram("point.vert", "point.frag");

	// uniform�ϐ��̏ꏊ���擾����
	const GLint modelViewLoc = glGetUniformLocation(program, "modelView");
	const GLint projectionLoc = glGetUniformLocation(program, "projection");
	const GLint normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");

	// �}�`�f�[�^���쐬����
	std::unique_ptr<const Shape> shape = std::make_unique<const SolidShapeIndex>(3, 36, solidCubeVertex, 36, solidCubeIndex);

	glfwSetTime(0.0);

	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while(window)
	{
		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �V�F�[�_�v���O�����̎g�p�J�n
		glUseProgram(program);

		// ���s���e�ϊ��s������߂�
		const GLfloat* const size = window.GetSize();
		const GLfloat fovy = window.GetScale() * 0.01f;
		const GLfloat aspect = size[0] / size[1];
		const Matrix projection = Matrix::Perspective(fovy, aspect, 1.0f, 10.0f);
		
		// ���f���ϊ��s������߂�
		const GLfloat* const location = window.GetLocation();
		const Matrix rotate = Matrix::Rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f);
		const Matrix model = Matrix::Translate(location[0], location[1], 0.0f) * rotate;

		// �r���[�ϊ��s������߂�
		const Matrix view = Matrix::LookAt(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0f);

		// �@���x�N�g���̕ϊ��s��̊i�[��
		GLfloat normalMatrix[9];

		// ���f���r���[�ϊ��s������߂�
		const Matrix modelView = view * model;

		// �@���x�N�g���̕ϊ��s������߂�
		modelView.GetNormalMatrix(normalMatrix);

		// uniform�ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.Data());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView.Data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

		// �}�`��`�悷��
		shape->Draw();

		// 2�ڂ̃��f���r���[�ϊ��s������߂�
		const Matrix modelView1 = modelView * Matrix::Translate(0.0f, 0.0f, 3.0f);

		// 2�ڂ̖@���x�N�g�������߂�
		modelView1.GetNormalMatrix(normalMatrix);

		// uniform�ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView1.Data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

		// 2�ڂ̐}�`��`�悷��
		shape->Draw();

		// �J���[�o�b�t�@�����ւ���
		window.SwapBuffers();
	}
}