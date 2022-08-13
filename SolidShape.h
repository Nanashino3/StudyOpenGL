#pragma once

#include "Shape.h"

class SolidShape : public Shape
{
public:
	// �R���X�g���N�^
	//	 size�F���_�ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	SolidShape(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex)
	: Shape(size, vertexCount, vertex)
	{}

	virtual void Execute() const
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	}
};