#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	// GLFW������������
	if(!glfwInit())
	{
		std::cerr << "Can't Initialize GLFW" << std::endl;
		return -1;
	}

	// �v���O�����I�����̏�����o�^����
	atexit(glfwTerminate);

	// OpenGL Version 4.6 Core Profile ��I������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// �E�B���h�E���쐬����
	GLFWwindow* const window = glfwCreateWindow(640, 480, "Hello!", NULL, NULL);
	if(window == NULL)
	{
		std::cerr << "Can't Create GLFW Window" << std::endl;
		return -1;
	}

	// �쐬�����E�B���h�E��OpenGL�̏����Ώۂɂ���
	glfwMakeContextCurrent(window);

	// GLEW������������
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Can't Initialize GLEW" << std::endl;
		return -1;
	}

	// ���������̃^�C�~���O��҂�
	glfwSwapInterval(1);

	// �w�i�F���w��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// �E�B���h�E���J���Ă���ԌJ��Ԃ�
	while(!glfwWindowShouldClose(window))
	{
		// �E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		//
		// �����ŕ`�揈�����s��
		// 

		// �J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);

		// �C�x���g�����o��
		glfwWaitEvents();
	}
}