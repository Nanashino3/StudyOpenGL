#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"

// プログラムオブジェクトのリンク結果を表示する
//   program：プログラムオブジェクト名
GLboolean PrintShaderInfoLog(GLuint program)
{
	// リンク結果を取得する
	GLint status;
	glGetShaderiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cerr << "Link Error." << std::endl;
	}

	// シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if(bufSize > 1)
	{
		// シェーダのリンク時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

// シェーダオブジェクトのコンパイル結果を表示する
//   shader：シェーダオブジェクト名
//   str：コンパイルエラーが発生した場所を示す場所
GLboolean PrintShaderInfoLog(GLuint shader, const char* str)
{
	// コンパイル結果を取得する
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		std::cerr << "Compile Error in" << str << std::endl;
	}

	// シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		// シェーダのコンパイル時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

// プログラムオブジェクトを作成する
//	  vsrc：頂点シェーダのソースプログラムの文字列
//    fsrc：画素シェーダのソースプログラムの文字列
GLuint CreateProgram(const char* vsrc, const char* fsrc)
{
	// 空のプログラムオブジェクトを作成する
	const GLuint program = glCreateProgram();

	if(vsrc != NULL)
	{
		// 頂点シェーダのシェーダオブジェクトを作成する
		const GLuint vobj = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		// 頂点シェーダのオブジェクトをプログラムオブジェクトに組み込む
		if(PrintShaderInfoLog(vobj, "vertex shader"))
		{
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}

	if(fsrc != NULL)
	{
		// 画素シェーダのシェーダオブジェクトを作成する
		const GLuint fobj = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		// 画素シェーダのオブジェクトをプログラムオブジェクトに組み込む
		if(PrintShaderInfoLog(fobj, "fragment shader"))
		{
			glAttachShader(program, fobj);
		}
		glDeleteShader(fobj);
	}

	// プログラムオブジェクトをリンクする
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	// 作成したプログラムオブジェクトを返す
	if(PrintShaderInfoLog(program))
	{
		return program;
	}

	// プログラムオブジェクトが作成できなければ0で返す
	glDeleteProgram(program);
	return 0;
}

// シェーダのソースファイルを読み込んだメモリを返す
//   name：シェーダのソースファイル名
//   buffer：読み込んだソースファイルのテキスト
bool ReadShaderSource(const char* name, std::vector<GLchar> &buffer)
{
	// ファイル名がNULLだった
	if(name == NULL){ return false; }

	// ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if(file.fail())
	{
		std::cerr << "Error：Can't open source file：" << name << std::endl;
		return false;
	}

	// ファイルの末尾に移動し現在位置(ファイルサイズ)を取得する
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	// ファイルサイズのメモリを確保
	buffer.resize(length + 1);

	// ファイルを先頭から読み込む
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if(file.fail())
	{
		std::cerr << "Error：Could not read source file：" << name << std::endl;
		file.close();
		return false;
	}

	// 読み込み成功
	file.close();
	return true;
}

// シェーダのソースファイルを読み込んでプログラムオブジェクトを作成する
//   vert：頂点シェーダのソースファイル名
//   frag：画素シェーダのソースファイル名
GLuint LoadProgram(const char* vert, const char* frag)
{
	// シェーダのソースファイルを読み込む
	std::vector<GLchar> vsrc;
	const bool vstat = ReadShaderSource(vert, vsrc);
	std::vector<GLchar> fsrc;
	const bool fstat = ReadShaderSource(frag, fsrc);

	// プログラムオブジェクトを作成する
	return vstat && fstat ? CreateProgram(vsrc.data(), fsrc.data()) : 0;
}

// 面ごとに色を変えた六面体の頂点属性
constexpr Object::VERTEX solidCubeVertex[] = 
{
	// 左
	{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f },
	{ -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f },

	// 裏
	{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	{  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
	
	// 下
	{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f },
	
	// 右
	{  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f },

	// 上
	{ -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f },

	// 前
	{ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f }
};

// 六面体の面を塗りつぶす三角形の頂点のインデックス
constexpr GLuint solidCubeIndex[] = 
{
	 0,  1,  2,  3,  4,  5, // 左
	 6,  7,  8,  9, 10, 11, // 裏
	12, 13, 14, 15, 16, 17, // 下
	18, 19, 20, 21, 22, 23, // 右
	24, 25, 26, 27, 28, 29, // 上
	30, 31, 32, 33, 34, 35, // 前
};

int main()
{
	// GLFWを初期化する
	if(!glfwInit())
	{
		std::cerr << "Can't Initialize GLFW" << std::endl;
		return -1;
	}

	// プログラム終了時の処理を登録する
	atexit(glfwTerminate);

	// OpenGL Version 4.6 Core Profile を選択する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window window;

	// 背景色を指定
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// 背面カリングを有効にする
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// デプスバッファを有効にする
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// プログラムオブジェクトを作成する
	const GLuint program = LoadProgram("point.vert", "point.frag");

	// uniform変数の場所を取得する
	const GLint modelViewLoc = glGetUniformLocation(program, "modelView");
	const GLint projectionLoc = glGetUniformLocation(program, "projection");
	const GLint normalMatrixLoc = glGetUniformLocation(program, "normalMatrix");

	// 図形データを作成する
	std::unique_ptr<const Shape> shape = std::make_unique<const SolidShapeIndex>(3, 36, solidCubeVertex, 36, solidCubeIndex);

	glfwSetTime(0.0);

	// ウィンドウが開いている間繰り返す
	while(window)
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// シェーダプログラムの使用開始
		glUseProgram(program);

		// 直行投影変換行列を求める
		const GLfloat* const size = window.GetSize();
		const GLfloat fovy = window.GetScale() * 0.01f;
		const GLfloat aspect = size[0] / size[1];
		const Matrix projection = Matrix::Perspective(fovy, aspect, 1.0f, 10.0f);
		
		// モデル変換行列を求める
		const GLfloat* const location = window.GetLocation();
		const Matrix rotate = Matrix::Rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f);
		const Matrix model = Matrix::Translate(location[0], location[1], 0.0f) * rotate;

		// ビュー変換行列を求める
		const Matrix view = Matrix::LookAt(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0f);

		// 法線ベクトルの変換行列の格納先
		GLfloat normalMatrix[9];

		// モデルビュー変換行列を求める
		const Matrix modelView = view * model;

		// 法線ベクトルの変換行列を求める
		modelView.GetNormalMatrix(normalMatrix);

		// uniform変数に値を設定する
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.Data());
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView.Data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

		// 図形を描画する
		shape->Draw();

		// 2つ目のモデルビュー変換行列を求める
		const Matrix modelView1 = modelView * Matrix::Translate(0.0f, 0.0f, 3.0f);

		// 2つ目の法線ベクトルを求める
		modelView1.GetNormalMatrix(normalMatrix);

		// uniform変数に値を設定する
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelView1.Data());
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix);

		// 2つ目の図形を描画する
		shape->Draw();

		// カラーバッファを入れ替える
		window.SwapBuffers();
	}
}