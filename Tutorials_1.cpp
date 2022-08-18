
#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

int main()
{
	// GLFW�̏�����
	if(!glfwInit()){
		std::cerr << "GLFW�̏������Ɏ��s���܂���" << std::endl;
		return -1;
	}

	// �E�B���h�E�̍쐬
	GLFWwindow* const window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	if(window == NULL){
		std::cerr << "�E�B���h�E�̍쐬�Ɏ��s���܂���" << std::endl;
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