
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "linalg.h"

#include "Mesh.h"
#include "ShaderProgram.h"


class GraphicsApplication {
    
public:
    GraphicsApplication(const char* name) : name(name), width(320), height(180) {}
    GraphicsApplication(const char* name, const unsigned int width, const unsigned int height) : name(name), width(width), height(height) {}

    void start() {
        setup();

        ShaderProgram shader = ShaderProgram::fromFiles("data/shaders/vertex.vs", "data/shaders/fragment.fs");
        Mesh cube = Mesh::fromWavefront("data/objects/cube.obj");
        vec3 extent(1.0, 1.0, 1.0);
        double distance = 30.0;
        
        while(!glfwWindowShouldClose(window)) { 
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);

                if(ImGui::CollapsingHeader("Extent")) {
                    ImGui::SliderFloat("x", &extent.x, 0.0, 3.0);
                    ImGui::SliderFloat("y", &extent.y, 0.0, 3.0);
                    ImGui::SliderFloat("z", &extent.z, 0.0, 3.0);
                }

            ImGui::End();

            // TODO calculate dt
            update(1.0);

            glClearColor( 0.0, 0.0, 0.0, 0.0 );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear depth buffer

            cube.setExtent(extent);
            cube.rot(0.005, vec3(1.0, 1.0, 0.0));

            mat4 P = perspective( 90.0*M_PI/180.0, width/(float)height, distance-10, distance+10);
            mat4 V = translate(0.0, 0.0, -distance);

            shader.use();
            shader.setMat4("o2w", cube.transform());
            shader.setMat4("w2c", P * V);
            shader.setVec3("lightPosition", vec3(10.0, 10.0, 10.0));
            cube.render();

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        terminate();
    }



private:

    const char* name;
    const unsigned int width;
    const unsigned int height;

    GLFWwindow* window;

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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

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