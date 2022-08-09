#pragma once
#include <memory>
#include "Object.h"

class Shape
{
public:
	// �R���X�g���N�^
	//	 size�F���_�̈ʒu�̎���
	//	 vertexCount�F���_�̐�
	//	 vertex�F���_�������i�[�����z��
	Shape(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex)
	: m_object(std::make_shared<Object>(size, vertexCount, vertex))
	, m_vertexCount(vertexCount)
	{}

	// �`����s
	virtual void Execute() const
	{
		// �܂���ŕ`�悷��
		glDrawArrays(GL_LINE_LOOP, 0, m_vertexCount);
	}

	// �`��
	void Draw() const
	{
		// ���_�z��I�u�W�F�N�g����������
		m_object->Bind();
		// �`������s����
		Execute();
	}

private:
	std::shared_ptr<const Object> m_object;	// �}�`�f�[�^

protected:
	const GLsizei m_vertexCount;			// �`��Ɏg�����_�̐�
};