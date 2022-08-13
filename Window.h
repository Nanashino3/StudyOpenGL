#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	// �R���X�g���N�^
	Window(int width = 640, int height = 480, const char* title = "Hello!")
	: m_window(glfwCreateWindow(width, height, title, NULL, NULL))
		, m_scale(100.0f), m_location{0.0f, 0.0f}, m_keyStatus(GLFW_RELEASE)
	{
		if(m_window == NULL)
		{
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		// ���݂̃E�B���h�E�������Ώۂɂ���
		glfwMakeContextCurrent(m_window);

		// GLEW��������
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK)
		{
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		// ���������̃^�C�~���O��҂�
		glfwSwapInterval(1);

		// �E�B���h�E�̃T�C�Y�ύX���ɌĂяo�������̓o�^
		glfwSetWindowSizeCallback(m_window, Resize);

		// �}�E�X�z�C�[�����쎞�ɌĂяo�������̓o�^
		glfwSetScrollCallback(m_window, Wheel);

		glfwSetKeyCallback(m_window, Keyboard);

		// ���̃C���X�^���X��this�|�C���^���L�^���Ă���
		glfwSetWindowUserPointer(m_window, this);

		// �J�����E�B���h�E�̏����ݒ�
		Resize(m_window, width, height);
	}

	// �f�X�g���N�^
	virtual ~Window()
	{
		glfwDestroyWindow(m_window);
	}

	// �`�惋�[�v�p������
	explicit operator bool()
	{
		// �C�x���g�����o��
		glfwPollEvents();

		// �L�[�{�[�h�̏�Ԃ𒲂ׂ�
		if(glfwGetKey(m_window, GLFW_KEY_LEFT) != GLFW_RELEASE)
		{
			m_location[0] -= 2.0f / m_size[0];
		}
		else if(glfwGetKey(m_window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
		{
			m_location[0] += 2.0f / m_size[0];
		}
		if(glfwGetKey(m_window, GLFW_KEY_DOWN) != GLFW_RELEASE)
		{
			m_location[1] -= 2.0f / m_size[1];
		}
		else if(glfwGetKey(m_window, GLFW_KEY_UP) != GLFW_RELEASE)
		{
			m_location[1] += 2.0f / m_size[1];
		}

		// �}�E�X�̍��{�^���̏�Ԃ𒲂ׂ�
		if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			// ���{�^����������Ă�����}�E�X�J�[�\���̈ʒu���擾����
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);

			// �}�E�X�J�[�\���̐��K���f�o�C�X���W�`��ł̈ʒu�����߂�
			m_location[0] = static_cast<GLfloat>(x) * 2.0f / m_size[0] - 1.0f;
			m_location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / m_size[1];
		}

		// �E�B���h�E�����K�v���Ȃ����true��Ԃ�
		return !glfwWindowShouldClose(m_window) && !glfwGetKey(m_window, GLFW_KEY_ESCAPE);
	}

	// �_�u���o�b�t�@�����O
	void SwapBuffers() const
	{
		// �J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(m_window);
	}

	// �E�B���h�E�̃T�C�Y�ύX���̏���
	static void Resize(GLFWwindow* const window, int width, int height)
	{
		// �t���[���o�b�t�@�̃T�C�Y�𒲂ׂ�
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// �t���[���o�b�t�@�S�̂��r���[�|�[�g�ɐݒ肷��
		glViewport(0, 0, fbWidth, fbHeight);

		// ���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// ���̃C���X�^���X���ێ�����c������X�V����
			instance->m_size[0] = static_cast<GLfloat>(width);
			instance->m_size[1] = static_cast<GLfloat>(height);
		}
	}

	// �}�E�X�z�C�[�����쎞�̏���
	static void Wheel(GLFWwindow* window, double x, double y)
	{
		// ���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦���X�V����
			instance->m_scale += static_cast<GLfloat>(y);
		}
	}

	// �L�[�{�[�h���쎞�̏���
	static void Keyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// �L�[��Ԃ̕ۑ�
			instance->m_keyStatus = action;
		}
	}
	
	// �E�B���h�E�T�C�Y�����o��
	const GLfloat* GetSize() const { return m_size; }
	// ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦�����o��
	GLfloat GetScale() const { return m_scale; }
	// �ʒu�����o��
	const GLfloat* GetLocation() const { return m_location; }

private:
	GLFWwindow* const m_window;	// �E�B���h�E�n���h��
	GLfloat m_size[2];			// �E�B���h�E�T�C�Y
	GLfloat m_scale;			// ���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦
	GLfloat m_location[2];		// �}�`�̐��K���f�o�C�X���W�`��̈ʒu
	int m_keyStatus;			// �L�[�{�[�h���
};