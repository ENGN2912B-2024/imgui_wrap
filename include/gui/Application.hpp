//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Window.hpp>

#include <memory>
#include <string>

namespace gui
{
  class Application
  {
    static Application* instance_;
    std::unique_ptr<Window> window_;
  public:
    Application(const std::string& title = "Application");
    virtual ~Application();

    Window& getWindow();
    const Window& getWindow() const;

    static void setInstancePtr(Application* instance) { instance_ = instance; }
    static Application* getInstancePtr() { return instance_;}

    void run();
  };

  // Declares `getApp()` function that returns a reference to the global
  //  application instance.
  #define DECLARE_APPLICATION(ApplicationClass) \
    ApplicationClass& getApp() { \
      return static_cast<ApplicationClass&>(*Application::getInstancePtr()) }

} // namespace gui