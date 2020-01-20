#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "iostream"

int main()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    unsigned int indices[] = { // 索引从0开始
        0, 1, 3,
        1, 2 ,3
    };

    // 初始化glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 创建索引缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 顶点着色器对象的ID
    unsigned int VBO;
    // 生成顶点缓冲对象
    glGenBuffers(1, &VBO);
    // 把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点数据复制到缓冲内存中
    // GL_STATIC_DRAW 数据不会或几乎不会改变
    // GL_DYNAMAMIC_DRAW 数据被改变很多
    // GL_STREAM_DRAW 数据每次绘制的时候都会被改变
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 使用着色器语言编写顶点着色器
    const char* shader_source = " \
       \n #version 330 core   \
       \n layout (location = 0) in vec3 aPos; \
       \n void main() \
       \n {            \
        \n gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \
       \n }";
    // 创建着色器对象
    unsigned int  vertexShader;
    // 创建一个顶点着色器，传入着色器类型，返回着色器ID
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shader_source, nullptr);
    glCompileShader(vertexShader);
    // 检测顶点着色器是否被编译成功
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    std::cout << success << std::endl;
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 使用着色器语言编写片段着色器
    // 输出一个4分量向量，表示最终输出的颜色
    // 传给vec4的四个参数分别是 红绿蓝和透明度
    const char* fragment_shader = " \
        \n #version 330 core \
        \n out vec4 FragColor; \
        \n \
        \n void main()       \
        \n{ \
        \n    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \
        \n}";
    unsigned int fragment_shader_id;
    // 创建一个片段着色器
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader, nullptr);
    // 编译着色器
    glCompileShader(fragment_shader_id);
    // 创建一个着色器程序对象
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // 将着色器附加到程序对象上并连接
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragment_shader_id);
    glLinkProgram(shaderProgram);
    // 检测链接着色器程序是否失败
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        // 如果失败， 获取错误信息并输出
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << infoLog << std::endl;
    }
    // 使用刚刚创建的程序对象作为它的参数，以激活这个对象
    glUseProgram(shaderProgram);
    // 着色器对象链接到程序对象后，就不需要着色器对象了，可以删除了
    glDeleteShader(vertexShader);
    glDeleteShader(fragment_shader_id);
    // 使用glVertexAttribPointer告知OpenGL如何解析顶点数据
    // 第一个参数表明从0开始索引数据
    // 第二个参数是顶点属性的大小，因为顶点属性是vec3，由3个值组成，所以大小是3
    // 第三个参数是数据类型
    // 不将数据标准化
    // 步长
    // 偏移量
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    //// 复制顶点数组到缓冲区供OpenGL使用 （先绑定再复制）
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //// 1.设置顶点属性指针
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// 2.使用着色器程序
    //glUseProgram(shaderProgram);
    //// 3.绘制物体


    // 创建一个VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲区供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 复制我们的顶点数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {
        // 第一个参数是图元的类型
        // 第二个参数顶点数组起始索引
        // 第三个参数是绘制的顶点个数
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 4, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}