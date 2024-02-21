#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "linalg.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

class GraphicsApplication {
    
public:
    GraphicsApplication(const char* name) : name(name), width(320), height(180) {}
    GraphicsApplication(const char* name, const unsigned int width, const unsigned int height) : name(name), width(width), height(height) {}

    void start() {
        setup();

        GLfloat cubeVerts[3*36] = { 
            -1,-1,-1,
            -1,-1, 1,
            -1, 1, 1,

            1, 1,-1,
            -1,-1,-1,
            -1, 1,-1,
            
            1,-1, 1,
            -1,-1,-1,
            1,-1,-1,
            
            1, 1,-1,
            1,-1,-1,
            -1,-1,-1,
            
            -1,-1,-1,
            -1, 1, 1,
            -1, 1,-1,
            1,-1, 1,
            -1,-1, 1,
            -1,-1,-1,
            -1, 1, 1,
            -1,-1, 1,
            1,-1, 1,
            1, 1, 1,
            1,-1,-1,
            1, 1,-1,
            1,-1,-1,
            1, 1, 1,
            1,-1, 1,
            1, 1, 1,
            1, 1,-1,
            -1, 1,-1,
            1, 1, 1,
            -1, 1,-1,
            -1, 1, 1,
            1, 1, 1,
            -1, 1, 1,
            1,-1, 1
        };

        const char* vertexShaderText = 
            "#version 330 \n"
            "uniform mat4 MVP; \n"
            "layout (location = 0) in vec3 vertexPosition; \n"
            "void main() { \n"
            "   gl_Position = MVP * vec4(vertexPosition, 1.0); \n"
            "} \n";

        const char* fragmentShaderText = 
            "#version 330 \n"
            "out mediump vec4 fragmentColour; \n"
            "void main() { \n"
            "   fragmentColour = vec4(1.0, 1.0, 1.0, 1.0); \n"
            "} \n";

        // opengl 

        GLuint vertexShader, fragmentShader;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
        glCompileShader(vertexShader);

        GLint success;
        char log[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
        glCompileShader(fragmentShader);
        
        GLuint shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);  

        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);

        GLuint cubeVBO;
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        double distance = 10.0;
        double rotation = 0.0;


        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow(); // Show demo window! :)

            // TODO calculate dt
            update(1.0);
            rotation += 0.01;

            glClearColor( 0.0, 0.0, 0.0, 0.0 );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear depth buffer

            mat4 MVP
                = perspective( 30.0*M_PI/180.0, WINDOW_WIDTH/(float)WINDOW_HEIGHT, distance-2, distance+2 )
                * translate( 0, 0, -distance )
                * scale(1.0, 1.0, 1.0)
                * rotate( rotation, vec3( 0, 1, 1 ) );

            glUseProgram(shaderProgram);

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_TRUE, &MVP[0][0]);

            glBindVertexArray( cubeVAO );
            glDrawArrays( GL_TRIANGLES, 0, 36 );
            glBindVertexArray( 0 );

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }


        glDeleteProgram(shaderProgram);

        terminate();
    }



private:

    const char* name;
    const unsigned int width;
    const unsigned int height;

    GLFWwindow* window;
    GLuint cubeVAO;

    void setup() {
        if(!glfwInit())
            throw;

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

        window = glfwCreateWindow( width, height, name, NULL, NULL);

        if(!window)
            throw;

        glfwMakeContextCurrent( window );
        glfwSwapInterval( 1 );
        gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }

    void update(double dt) {

    }

    void terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

};

int main(int argc, char *argv[]) {
    std::cout << argv[0] << std::endl;

    GraphicsApplication app("My First Window", WINDOW_WIDTH, WINDOW_HEIGHT);
    app.start();

    return 0;
}