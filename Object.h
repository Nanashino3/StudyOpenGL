#pragma once
#include <GL/glew.h>

class Object
{
public:
	// ���_����
	struct VERTEX
	{
		// �ʒu
		GLfloat position[2];
	};

	// �R���X�g���N�^
	//	 size�F���_�̈ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	Object(GLint size, GLsizei vertexCount, const VERTEX* vertex)
	{
		// ���_�z��I�u�W�F�N�g
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// ���_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VERTEX), vertex, GL_STATIC_DRAW);
		
		// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g��in�ϐ�����Q�Ƃł���悤�ɂ���
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	// �f�X�g���N�^
	virtual ~Object()
	{
		// ���_�z��I�u�W�F�N�g���폜����
		glDeleteVertexArrays(1, &m_vao);
		// ���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &m_vbo);
	}

	// ���_�z��I�u�W�F�N�g�̌���
	void Bind() const
	{
		// �`�悷�钸�_�z��I�u�W�F�N�g���w�肷��
		glBindVertexArray(m_vao);
	}

private:
	// �R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~
	Object(const Object&);
	// ����ɂ��R�s�[�֎~
	Object& operator=(const Object&);

private:
	GLuint m_vao;	// ���_�z��I�u�W�F�N�g��
	GLuint m_vbo;	// ���_�o�b�t�@�I�u�W�F�N�g��
};