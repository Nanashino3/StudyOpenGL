#pragma once
#include <GL/glew.h>

class Object
{
public:
	// 頂点属性
	struct VERTEX
	{
		// 位置
		GLfloat position[2];
	};

	// コンストラクタ
	//	 size：頂点の位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	Object(GLint size, GLsizei vertexCount, const VERTEX* vertex)
	{
		// 頂点配列オブジェクト
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// 頂点バッファオブジェクト
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VERTEX), vertex, GL_STATIC_DRAW);
		
		// 結合されている頂点バッファオブジェクトをin変数から参照できるようにする
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	// デストラクタ
	virtual ~Object()
	{
		// 頂点配列オブジェクトを削除する
		glDeleteVertexArrays(1, &m_vao);
		// 頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &m_vbo);
	}

	// 頂点配列オブジェクトの結合
	void Bind() const
	{
		// 描画する頂点配列オブジェクトを指定する
		glBindVertexArray(m_vao);
	}

private:
	// コピーコンストラクタによるコピー禁止
	Object(const Object&);
	// 代入によるコピー禁止
	Object& operator=(const Object&);

private:
	GLuint m_vao;	// 頂点配列オブジェクト名
	GLuint m_vbo;	// 頂点バッファオブジェクト名
};