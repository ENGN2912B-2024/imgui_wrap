
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(LINUX TRUE)
endif()

option(USE_GLFW_GL3 "Enable GLFW/OpenGL3 backend" ON)
option(USE_GUI_TEST_ENGINE "Enable Dear ImGui test engine" OFF)

if(MSVC)
  add_compile_options(/permissive-)
  if (NOT USE_GUI_TEST_ENGINE)
    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  endif()
endif()

if(XCODE)
  option(XCODE_UNIVERSAL_APP "Universal MacOS executable")
  if (XCODE_UNIVERSAL_APP)
    # Enable building of universal MacOS executable (works both on Intel
    # and ARM based Mac's). Requires Xcode 12.2 or newer.
    message(STATUS "Building MacOS universal executable")
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
  endif()
endif()

include(cmake/git_submodule.cmake)

checkout_submodules(${PROJECT_SOURCE_DIR}/3rd-party/imgui)
if(USE_GLFW_GL3)
  checkout_submodules(${PROJECT_SOURCE_DIR}/3rd-party/glfw)
endif()
if(USE_GUI_TEST_ENGINE)
  checkout_submodules(${PROJECT_SOURCE_DIR}/3rd-party/imgui_test_engine)
endif()