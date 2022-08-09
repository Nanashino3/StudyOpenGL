#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shape.h"

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
	const bool vstat(ReadShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(ReadShaderSource(frag, fsrc));

	// プログラムオブジェクトを作成する
	return vstat && fstat ? CreateProgram(vsrc.data(), fsrc.data()) : 0;
}

// 矩形の頂点の位置
constexpr Object::VERTEX rectangle[] = 
{
	{-0.5f, -0.5f},
	{ 0.5f, -0.5f},
	{ 0.5f,  0.5f},
	{-0.5f,  0.5f}
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

	// プログラムオブジェクトを作成する
	const GLuint program(LoadProgram("point.vert", "point.frag"));

	// uniform変数の場所を取得する
	const GLint sizeLoc = glGetUniformLocation(program, "size");
	const GLint scaleLoc = glGetUniformLocation(program, "scale");
	const GLint locationLoc = glGetUniformLocation(program, "location");

	// 図形データを作成する
	std::unique_ptr<const Shape> shape = std::make_unique<const Shape>(2, 4, rectangle);

	// ウィンドウが開いている間繰り返す
	while(window)
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用開始
		glUseProgram(program);

		// uniform変数に値を設定する
		glUniform2fv(sizeLoc, 1, window.GetSize());
		glUniform1f(scaleLoc, window.GetScale());
		glUniform2fv(locationLoc, 1, window.GetLocation());

		// 図形を描画する
		shape->Draw();

		// カラーバッファを入れ替える
		window.SwapBuffers();
	}
}