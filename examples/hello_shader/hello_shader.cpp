//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gl/gl.h>
#include <gl/FrameBuffer.hpp>
#include <gl/Program.hpp>
#include <gui/gui.hpp>
#include <timer/Timer.hpp>

#include <cassert>
#include <cmath>
#include <memory>
#include <iostream>

class TopFrame : public gui::ChildFrame
{
  std::unique_ptr<gl::FrameBuffer> frameBuffer_;
  std::unique_ptr<gl::Program> program_;
  GLuint VBO, VAO;
  bool initialized_;
  std::vector<float> vertices_;
  timer::Timer timer_;
  float angle_;
public:
  TopFrame() :
    frameBuffer_{ nullptr },
    program_{ nullptr },
    initialized_{ false },
    vertices_{},
    timer_{},
    angle_{ 0.0f }
  {
    timer_.start(100, [this]()
    {
      angle_ += 7.2f;
      if (angle_ >= 360.0f)
      {
        angle_ -= 360.0f;
      }
    });
  }

  void initGL()
  {
    if (initialized_)
    { // Already initialized
      return;
    }

    if (!frameBuffer_)
    {
      frameBuffer_ = std::make_unique<gl::FrameBuffer>();
    }

    if (!program_)
    {
      const size_t glslVersion{ gl::Shader::getShadingLanguageVersion() };
      //std::cout << "GLSL Version: " << glslVersion << std::endl;
      std::string vertexShader, fragmentShader;
      if (glslVersion == 140)
      {
        vertexShader =
          "#version 140\n"
          "attribute vec3 aPos;\n"
          "attribute vec3 aColor;\n"
          "\n"
          "varying vec3 vColor;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  gl_Position = vec4(aPos, 1.0);\n"
          "  vColor = aColor;\n"
          "}\n"
          ;
        fragmentShader =
          "#version 140\n"
          "varying vec3 vColor;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  gl_FragColor = vec4(vColor, 1.0);\n"
          "}\n"
          ;
      }
      else if (glslVersion > 140)
      {
        vertexShader =
          "#version 150\n"
          "in vec3 aPos;\n"
          "in vec3 aColor;\n"
          "\n"
          "out vec3 vColor;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  gl_Position = vec4(aPos, 1.0);\n"
          "  vColor = aColor;\n"
          "}\n"
          ;
        fragmentShader =
          "#version 150\n"
          "in vec3 vColor;\n"
          "out vec4 FragColor;\n"
          "\n"
          "void main()\n"
          "{\n"
          "  FragColor = vec4(vColor, 1.0);\n"
          "}\n"
          ;
      }
      else
      {
        throw std::runtime_error("Unsupported GLSL version");
      }
      program_ = std::make_unique<gl::Program>(vertexShader, fragmentShader);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Define the vertex data
    const float factor{ std::cos(angle_ * 3.14159f / 180.0f) };
    vertices_ = std::vector<float>{
        // Positions         // Colors
        -0.6f * factor, -0.75f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
         0.6f * factor, -0.75f, 0.0f,  0.0f, 1.0f, 0.0f, // Green
         0.0f * factor,  0.75f, 0.0f,  0.0f, 0.0f, 1.0f  // Blue
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
      sizeof(float)*vertices_.size(), vertices_.data(),
      GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set the initialized flag
    initialized_ = true;
  }

  void render() override
  {
    ImGui::Text("Top Frame");

    initGL();

    frameBuffer_->setSize(math::make<gui::Vec2i>(ImGui::GetContentRegionAvail()));

    ImGui::Image(
      (ImTextureID)(intptr_t)frameBuffer_->getTexture(),
      frameBuffer_->getSize().to<float>(),
      ImVec2(0, 1),
      ImVec2(1, 0)
    );

    drawGL();
  }

  void drawGL()
  {
    if (frameBuffer_ != nullptr)
    {
      configureViewport(-1, 1, -1 , 1);

      frameBuffer_->bind();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      drawTriangle();

      frameBuffer_->unbind();
    }
  }

  void configureViewport(int x1, int y1, int x2, int y2)
  {
    assert(frameBuffer_ != nullptr);

    //make the viewport occupy the whole canvas
    const gui::Vec2i& canvasSize{ frameBuffer_->getSize() };
    glViewport(0, 0, canvasSize.x, canvasSize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the projection transformation
    glOrtho(x1, x2, y1, y2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void drawTriangle()
  {
    // update the vertex data
    const float factor_{ std::cos(angle_ * 3.14159f / 180.0f) };
    vertices_ = std::vector<float>{
        // Positions         // Colors
        -0.6f * factor_, -0.75f, 0.0f,  1.0f, 0.0f, 0.0f, // Red
         0.6f * factor_, -0.75f, 0.0f,  0.0f, 1.0f, 0.0f, // Green
         0.0f * factor_,  0.75f, 0.0f,  0.0f, 0.0f, 1.0f  // Blue
    };

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Update the vertex data
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*vertices_.size(), vertices_.data());

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Use the shader program
    assert(program_ != nullptr);
    program_->use();

    // Draw the triangle
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Unuse the shader program
    program_->unuse();
  }
};

class BottomFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Bottom Frame");
  }
};

class LeftFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Left Frame");
  }
};

class MainWindow : public gui::Frame
{
  TopFrame topFrame_;
  BottomFrame bottomFrame_;
  LeftFrame leftFrame_;
public:
  void render() override
  {
    gui::VerticalSizer verticalSizer;

    verticalSizer.addChild(&topFrame_, 3);
    verticalSizer.addChild(&bottomFrame_);

    gui::HorizontalSizer horizontalSizer;
    horizontalSizer.addWithFixedSize(&leftFrame_, 200);
    horizontalSizer.addChild(&verticalSizer, 3);

    horizontalSizer.setSize(getContentSize());
    horizontalSizer.setPosition(getContentMin());
    horizontalSizer.draw();
  }
};

int main(int argc, char** argv)
{
  // Create the application
  gui::Application app{ "GUI: Hello Shaders!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
