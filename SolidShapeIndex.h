#pragma once

#include "ShapeIndex.h"

class SolidShapeIndex : public ShapeIndex
{
public:
	// コンストラクタ
	//	 size：頂点の位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	//	 indexCount：頂点のインデックスの要素数
	//	 index：頂点のインデックスを格納した配列
	SolidShapeIndex(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex,
		GLsizei indexCount, const GLuint* index)
	: ShapeIndex(size, vertexCount, vertex, indexCount, index)
	{}

	// 描画実行
	virtual void Execute() const
	{
		// 三角形で描画する
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
};