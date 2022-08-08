#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

	// ウィンドウを作成する
	GLFWwindow* const window = glfwCreateWindow(640, 480, "Hello!", NULL, NULL);
	if(window == NULL)
	{
		std::cerr << "Can't Create GLFW Window" << std::endl;
		return -1;
	}

	// 作成したウィンドウをOpenGLの処理対象にする
	glfwMakeContextCurrent(window);

	// GLEWを初期化する
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Can't Initialize GLEW" << std::endl;
		return -1;
	}

	// 垂直同期のタイミングを待つ
	glfwSwapInterval(1);

	// 背景色を指定
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// ウィンドウが開いている間繰り返す
	while(!glfwWindowShouldClose(window))
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//
		// ここで描画処理を行う
		// 

		// カラーバッファを入れ替える
		glfwSwapBuffers(window);

		// イベントを取り出す
		glfwWaitEvents();
	}
}