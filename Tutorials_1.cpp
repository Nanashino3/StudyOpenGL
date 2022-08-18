
#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

int main()
{
	// GLFWの初期化
	if(!glfwInit()){
		std::cerr << "GLFWの初期化に失敗しました" << std::endl;
		return -1;
	}

	// ウィンドウの作成
	GLFWwindow* const window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	if(window == NULL){
		std::cerr << "ウィンドウの作成に失敗しました" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	const int version = gladLoadGL(glfwGetProcAddress);
	std::cout << "OpenGL version " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}