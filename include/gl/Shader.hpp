//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <cstddef>

namespace gl
{
  class Shader
  {
    unsigned shader_;
  public:
    Shader(unsigned type);
    ~Shader();

    void source(const char* source);
    void compile();
    unsigned get() const { return shader_; }

    static size_t getShadingLanguageVersion();
  };
} // namespace gl
