#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int window_width = 640;
int window_heignt = 400;


GLint makeShader() {
    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec3 color;\n"
        "uniform mat4 projectionMatrix;"
        "out vec3 outColor;\n"
        "void main(void) {\n"
        "outColor = color;\n"
        "gl_Position = projectionMatrix * vec4(position, 0.0f, 1.0f);\n"
        "}\n";


    const char* fragment_shader =
        "#version 400\n"
        "in vec3 outColor; \n"
        "out vec4 outFragmentColor; \n"
        "void main(void) {\n"
        "outFragmentColor = vec4(outColor, 1.0f); \n"
        "}\n";


    GLuint vs, fs, shader_programme;

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    return shader_programme;
}



int main() {
    GLFWwindow* window = NULL;

    //ポリゴンの描画位置をピクセルで指定
    GLfloat points[] = 
    { 
        250.0f, 250.0f,
        250.0f,  0.0f,
        0.0f,  250.0f,
        0.0f,  0.0f 
    };


    GLfloat colors[] = 
    { 
        1.0f, 0.0f, 0.3f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.8f, 1.0f
    };


    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(window_width, window_heignt, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    GLint shader = makeShader();
    GLuint vao, vertex_vbo, color_vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //座標系の変換行列（上の節参照）
    //const float projectionMatrix[4][4] = {
    //  { 2.0f / float(window_width), 0.0f, 0.0f, 0.0f },
    //  { 0.0f, -2.0f / float(window_heignt), 0.0f, 0.0f },
    //  { 0.0f, 0.0f, 1.0f, 0.0f },
    //  { -1.0f, 1.0f, 0.0f, 1.0f }
    //};
    float projectionMatrix[4][4] = {
        { 2.0f / window_width, 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / window_heignt, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    //変換行列をシェーダに渡すためのバッファ作成
    GLint projectionMatLocation = glGetUniformLocation(shader, "projectionMatrix");

    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        //バッファにデータを送信
        glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}