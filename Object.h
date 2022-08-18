#pragma once
#include <GL/glew.h>

class Object
{
public:
	// ���_����
	struct VERTEX
	{
		GLfloat position[3]; // �ʒu
		GLfloat normal[3];	 // �@��
	};

	// �R���X�g���N�^
	//	 size�F���_�̈ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	//	 indexCount�F���_�̃C���f�b�N�X�̗v�f��
	//	 index�F���_�̃C���f�b�N�X���i�[�����z��
	Object(GLint size, GLsizei vertexCount, const VERTEX* vertex, GLsizei indexCount = 0, const GLuint* index = NULL)
	{
		// ���_�z��I�u�W�F�N�g
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// ���_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VERTEX), vertex, GL_STATIC_DRAW);
		
		// ��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g��in�ϐ�����Q�Ƃł���悤�ɂ���
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(VERTEX), static_cast<VERTEX*>(0)->position);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), static_cast<VERTEX*>(0)->normal);
		glEnableVertexAttribArray(1);

		// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	// �f�X�g���N�^
	virtual ~Object()
	{
		// ���_�z��I�u�W�F�N�g���폜����
		glDeleteVertexArrays(1, &m_vao);
		// ���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &m_vbo);
		// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &m_ibo);
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
	GLuint m_ibo;	// �C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
};