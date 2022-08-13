#pragma once

#include "Shape.h"

class ShapeIndex : public Shape
{
public:
	// コンストラクタ
	//	 size：頂点の位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	//	 indexCount：頂点のインデックスの要素数
	//	 index：頂点のインデックスを格納した配列
	ShapeIndex(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex,
		GLsizei indexCount, const GLuint* index)
	: Shape(size, vertexCount, vertex, indexCount, index)
	, m_indexCount(indexCount)
	{}

	// 描画実行
	virtual void Execute() const
	{
		// 線分群で描画する
		glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
	}

protected:
	const GLsizei m_indexCount;	// 描画に使う頂点の数
};