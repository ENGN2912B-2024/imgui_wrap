
# GLFW #######################################################################
if (USE_GLFW_GL3)
  if (LINUX)
    set(GLFW_BUILD_WAYLAND 0)
    set(GLFW_BUILD_X11 1)
  endif()
  add_subdirectory(glfw)
endif(USE_GLFW_GL3)

# Dear ImGui #################################################################

add_library(imgui STATIC
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(imgui PUBLIC imgui imgui/backends imgui/misc/cpp)
target_compile_definitions(imgui
  PUBLIC -DIMGUI_USER_CONFIG="${CMAKE_CURRENT_LIST_DIR}/imconfig.h")

if (USE_GLFW_GL3)
  target_sources(imgui PRIVATE
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
  )

  target_compile_definitions(imgui
    PUBLIC
      -DUSE_GLFW_GL3
      -DGLFW_HAS_PER_MONITOR_DPI=1
  )

  target_link_libraries(imgui PUBLIC glfw)

  if (WIN32)
    target_link_libraries(imgui PUBLIC "opengl32")
  elseif(APPLE)
    target_link_libraries(imgui PUBLIC "-framework OpenGL")
  elseif(LINUX)
    target_link_libraries(imgui PUBLIC "-lGL")
  endif()

endif(USE_GLFW_GL3)

# Dear ImGui Test Engine #####################################################
if (USE_GUI_TEST_ENGINE)

  add_library(imgui_test_engine STATIC
    imgui_test_engine/imgui_test_engine/imgui_capture_tool.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_context.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_coroutine.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_engine.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_exporters.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_perftool.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_ui.cpp
    imgui_test_engine/imgui_test_engine/imgui_te_utils.cpp
  )

  target_include_directories(imgui_test_engine
    PUBLIC imgui_test_engine/imgui_test_engine)

  target_link_libraries(imgui_test_engine PUBLIC imgui)

  # We set USE_GUI_TEST_ENGINE define in `imgui` instead of `imgui_test_engine`
  # because this define affects the values of `imconfig.h` which is included
  # in `imgui` and other places. In this way, anything that links to `imgui`
  # will have the same values for `imconfig.h` and will have this define set,
  # including `imgui_test_engine`.
  target_compile_definitions(imgui PUBLIC -DUSE_GUI_TEST_ENGINE)

  if (LINUX)
    target_compile_options(imgui_test_engine
      PRIVATE -Wno-unused-result -Wno-deprecated-enum-enum-conversion)
  endif()

endif(USE_GUI_TEST_ENGINE)
