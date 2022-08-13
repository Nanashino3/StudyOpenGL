#pragma once

#include "Shape.h"

class SolidShape : public Shape
{
public:
	// コンストラクタ
	//	 size：頂点位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	SolidShape(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex)
	: Shape(size, vertexCount, vertex)
	{}

	virtual void Execute() const
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	}
};