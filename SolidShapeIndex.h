#pragma once

#include "ShapeIndex.h"

class SolidShapeIndex : public ShapeIndex
{
public:
	// �R���X�g���N�^
	//	 size�F���_�̈ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	//	 indexCount�F���_�̃C���f�b�N�X�̗v�f��
	//	 index�F���_�̃C���f�b�N�X���i�[�����z��
	SolidShapeIndex(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex,
		GLsizei indexCount, const GLuint* index)
	: ShapeIndex(size, vertexCount, vertex, indexCount, index)
	{}

	// �`����s
	virtual void Execute() const
	{
		// �O�p�`�ŕ`�悷��
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
};