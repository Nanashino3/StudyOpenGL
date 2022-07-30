#include <GLFW/glfw3.h>
#include <iostream>

const int g_width = 640;
const int g_height = 480;

int main(void)
{
    // GLFWの初期化
    if(!glfwInit()){ return -1; }
    
    // ウィンドウの作成
    GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Simple", NULL, NULL);
    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // ゲームループ
    while(!glfwWindowShouldClose(window)){
        // バッファのクリア
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
    }

    // GLFWの終了処理
    glfwTerminate();

    return 0;
}