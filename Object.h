#pragma once
#include <GL/glew.h>

class Object
{
public:
	// 頂点属性
	struct VERTEX
	{
		GLfloat position[3]; // 位置
		GLfloat normal[3];	 // 法線
	};

	// コンストラクタ
	//	 size：頂点の位置の次元
	//	 vertexCount：頂点の数
	//	 vertex：頂点属性を格納した配列
	//	 indexCount：頂点のインデックスの要素数
	//	 index：頂点のインデックスを格納した配列
	Object(GLint size, GLsizei vertexCount, const VERTEX* vertex, GLsizei indexCount = 0, const GLuint* index = NULL)
	{
		// 頂点配列オブジェクト
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// 頂点バッファオブジェクト
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(VERTEX), vertex, GL_STATIC_DRAW);
		
		// 結合されている頂点バッファオブジェクトをin変数から参照できるようにする
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(VERTEX), static_cast<VERTEX*>(0)->position);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), static_cast<VERTEX*>(0)->normal);
		glEnableVertexAttribArray(1);

		// インデックスの頂点バッファオブジェクト
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	// デストラクタ
	virtual ~Object()
	{
		// 頂点配列オブジェクトを削除する
		glDeleteVertexArrays(1, &m_vao);
		// 頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &m_vbo);
		// インデックスの頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &m_ibo);
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
	GLuint m_ibo;	// インデックスの頂点バッファオブジェクト
};