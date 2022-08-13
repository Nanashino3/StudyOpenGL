#pragma once

#include "Shape.h"

class ShapeIndex : public Shape
{
public:
	// �R���X�g���N�^
	//	 size�F���_�̈ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	//	 indexCount�F���_�̃C���f�b�N�X�̗v�f��
	//	 index�F���_�̃C���f�b�N�X���i�[�����z��
	ShapeIndex(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex,
		GLsizei indexCount, const GLuint* index)
	: Shape(size, vertexCount, vertex, indexCount, index)
	, m_indexCount(indexCount)
	{}

	// �`����s
	virtual void Execute() const
	{
		// �����Q�ŕ`�悷��
		glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
	}

protected:
	const GLsizei m_indexCount;	// �`��Ɏg�����_�̐�
};