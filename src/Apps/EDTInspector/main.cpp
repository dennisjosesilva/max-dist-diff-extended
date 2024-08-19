#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Vis/EDTInspector/Textute.hpp>

#include <iostream>

int main()
{
  GLFWwindow *window;

  // Initialise the library
  if (!glfwInit())
    return -1;

  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  
  // Make the window's context current
  glfwMakeContextCurrent(window);
   glfwSwapInterval(1); // Enable vsync

  // Load OpenGL Functions using GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize OpenGL Context";
    return -1;
  }  

  // Initialise ImGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);


  Texture textures[] = {
    Texture{"../dat/Nedved.png"},
    // Texture{"../dat/synthetic.png"},
    Texture{"../dat/lena.pgm"}  
  };

  int currIndex = 0;

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window))
  {    
    // Pool for and process events 
    glfwPollEvents();
    
    // ImGUI Init Code
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGUI Body (where things are create)
    {
      ImGui::Begin("Loading image");
      ImGui::Text("Loading Image from a texture");
      
      if (ImGui::Button("Swap Image")) {
        currIndex = (currIndex + 1) % 2;        
        textures[currIndex].bind();        
      }


      ImGui::Image((void*)(intptr_t)textures[currIndex].id(), 
        ImVec2(textures[currIndex].width(), textures[currIndex].height()),
        ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

      ImGui::End();
    }

    // ImGUI Render in the screen.
    ImGui::Render();
    
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    
    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    // ImGUI finalise
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap front and back buffers
    glfwSwapBuffers(window);
    
  }

  // Clean up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}