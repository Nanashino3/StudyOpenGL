#pragma once
#include <memory>
#include "Object.h"

class Shape
{
public:
	// コンストラクタ
	//	 size：頂点の位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	Shape(GLint size, GLsizei vertexCount, const Object::VERTEX* vertex)
	: m_object(std::make_shared<Object>(size, vertexCount, vertex))
	, m_vertexCount(vertexCount)
	{}

	// 描画実行
	virtual void Execute() const
	{
		// 折れ線で描画する
		glDrawArrays(GL_LINE_LOOP, 0, m_vertexCount);
	}

	// 描画
	void Draw() const
	{
		// 頂点配列オブジェクトを結合する
		m_object->Bind();
		// 描画を実行する
		Execute();
	}

private:
	std::shared_ptr<const Object> m_object;	// 図形データ

protected:
	const GLsizei m_vertexCount;			// 描画に使う頂点の数
};