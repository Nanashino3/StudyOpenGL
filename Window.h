#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	// コンストラクタ
	Window(int width = 640, int height = 480, const char* title = "Hello!")
	: m_window(glfwCreateWindow(width, height, title, NULL, NULL))
		, m_scale(100.0f), m_location{0.0f, 0.0f}, m_keyStatus(GLFW_RELEASE)
	{
		if(m_window == NULL)
		{
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		// 現在のウィンドウを処理対象にする
		glfwMakeContextCurrent(m_window);

		// GLEWを初期化
		glewExperimental = GL_TRUE;
		if(glewInit() != GLEW_OK)
		{
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		// 垂直同期のタイミングを待つ
		glfwSwapInterval(1);

		// ウィンドウのサイズ変更時に呼び出す処理の登録
		glfwSetWindowSizeCallback(m_window, Resize);

		// マウスホイール操作時に呼び出す処理の登録
		glfwSetScrollCallback(m_window, Wheel);

		glfwSetKeyCallback(m_window, Keyboard);

		// このインスタンスのthisポインタを記録しておく
		glfwSetWindowUserPointer(m_window, this);

		// 開いたウィンドウの初期設定
		Resize(m_window, width, height);
	}

	// デストラクタ
	virtual ~Window()
	{
		glfwDestroyWindow(m_window);
	}

	// 描画ループ継続判定
	explicit operator bool()
	{
		// イベントを取り出す
		glfwPollEvents();

		// キーボードの状態を調べる
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

		// マウスの左ボタンの状態を調べる
		if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			// 左ボタンが押されていたらマウスカーソルの位置を取得する
			double x, y;
			glfwGetCursorPos(m_window, &x, &y);

			// マウスカーソルの正規化デバイス座標形状での位置を求める
			m_location[0] = static_cast<GLfloat>(x) * 2.0f / m_size[0] - 1.0f;
			m_location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / m_size[1];
		}

		// ウィンドウを閉じる必要がなければtrueを返す
		return !glfwWindowShouldClose(m_window) && !glfwGetKey(m_window, GLFW_KEY_ESCAPE);
	}

	// ダブルバッファリング
	void SwapBuffers() const
	{
		// カラーバッファを入れ替える
		glfwSwapBuffers(m_window);
	}

	// ウィンドウのサイズ変更時の処理
	static void Resize(GLFWwindow* const window, int width, int height)
	{
		// フレームバッファのサイズを調べる
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		// フレームバッファ全体をビューポートに設定する
		glViewport(0, 0, fbWidth, fbHeight);

		// このインスタンスのthisポインタを得る
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// このインスタンスが保持する縦横比を更新する
			instance->m_size[0] = static_cast<GLfloat>(width);
			instance->m_size[1] = static_cast<GLfloat>(height);
		}
	}

	// マウスホイール操作時の処理
	static void Wheel(GLFWwindow* window, double x, double y)
	{
		// このインスタンスのthisポインタを得る
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// ワールド座標系に対するデバイス座標系の拡大率を更新する
			instance->m_scale += static_cast<GLfloat>(y);
		}
	}

	// キーボード操作時の処理
	static void Keyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			// キー状態の保存
			instance->m_keyStatus = action;
		}
	}
	
	// ウィンドウサイズを取り出す
	const GLfloat* GetSize() const { return m_size; }
	// ワールド座標系に対するデバイス座標系の拡大率を取り出す
	GLfloat GetScale() const { return m_scale; }
	// 位置を取り出す
	const GLfloat* GetLocation() const { return m_location; }

private:
	GLFWwindow* const m_window;	// ウィンドウハンドル
	GLfloat m_size[2];			// ウィンドウサイズ
	GLfloat m_scale;			// ワールド座標系に対するデバイス座標系の拡大率
	GLfloat m_location[2];		// 図形の正規化デバイス座標形上の位置
	int m_keyStatus;			// キーボード状態
};