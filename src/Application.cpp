#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>

/// <summary>
///     This function will compile shader
/// </summary>
/// 
/// <param name="shaderType">
///     The type of shader to be compiled could be vertex
///     or fragment shader.
/// </param>
/// <param name="source">
///     Here the source is shader code written strings
///     (this will modified in later)
/// </param>
/// 
/// <returns>
///     unsigned int
/// </returns>
static unsigned int compileShader(
    unsigned int shaderType,
    const std::string &source
) {
    /*First create a empty shader object with shaderID*/
    unsigned int shaderID = glCreateShader(shaderType);

    /* When providing the source convert it to const char* */
    const char* shaderSrc = &source[0];

    /*
     * Then replace or the set the shader code in shader object
     * with the ID of shaderID
     * NOTE:- When you provide the nullptr for length 
     *        opengl will automatically determine the length
     *        of the string provided by checking for null ptr ('\0')
    */
    glShaderSource(shaderID, 1, &shaderSrc, nullptr);
    glCompileShader(shaderID);

    // TODO: Add some error checking

    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

        /*
        * NOTE: We are using stack allocated char array
        *       Allocating length*sizeof(char) on stack.
        *       message is pointer pointing to some memory space
        *       on the stack.
        * 
        * **NOTE: Might lead to stackover flow error is length is
        *         message is greater than stack memory
        */
        //char* message = (char*)alloca(length * sizeof(char));

        /*
        * Create memory in the heap for char array aka string
        * NOTE: Delete this array after use to avoid heap memory overflow.
        */
        char* message = new char[length];

        glGetShaderInfoLog(shaderID, length, &length, message);
        std::cout << "Failed to compile " 
                  << (shaderType == GL_VERTEX_SHADER ? "Vertex shader\n" : "Fragment shader\n")
                  << message << std::endl;

        delete[] message;
    }

    return shaderID;
}

/// <summary>
///     This function creates shaders.
/// </summary>
/// 
/// <param name="vertexShader">
///     Takes in the vertex shader in the form of string reference
/// </param>
/// <param name="fragmentShader">
///     Takes in the fragment shader in the form of string reference
/// </param>
/// 
/// <returns>
///     static int
/// </returns>
static unsigned int createShader(
    const std::string& vertexShader, 
    const std::string& fragmentShader
) {

    /*
    * Creates new program object.
    * A program object holds reference
    * to different types of shaders
    */
    unsigned int programID = glCreateProgram();
    unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /*
    * Attach or add shaders to the program object container
    */
    glAttachShader(programID, vShader);
    glAttachShader(programID, fShader);

    /*
    * Link and validate all the shaders to program object container
    */
    glLinkProgram(programID);
    glValidateProgram(programID);

    return programID;
}


int main(){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Glew not initialized" << std::endl;
    }

    const int POSITION_SIZE = 6;
    float positions[POSITION_SIZE] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
    

    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // Selects the created buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Defines size of the buffer will be
    glBufferData(GL_ARRAY_BUFFER, 
        POSITION_SIZE*sizeof(float), 
        positions, GL_STATIC_DRAW
    ); 

    // Defines the layout of the buffer
    glVertexAttribPointer(
        0,
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        2*sizeof(float), 
        0
    );

    //Finally enable the buffer layout
    glEnableVertexAttribArray(0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "   gl_Position=position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color=vec4(1.0, 0.0, 1.0, 1.0);\n"
        "}\n";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        /*
        * This function is used when we don't use 
        * have a index buffer
        */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    // Delete the program object container
    glDeleteProgram(shader);

    // Clear the buffer
    return 0;
}