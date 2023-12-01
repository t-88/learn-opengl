#pragma once
#include <stdint.h>
#include <string>

class Shader
{
private:
    uint32_t ID;
    uint32_t vertex_shader;
    uint32_t fragment_shader;

public:
    Shader(std::string,std::string);
    ~Shader();

    void enable();
    void free();

    void set_bool(std::string name, bool val);
    void set_int(std::string name, bool val);
    void set_float(std::string name, bool val);
};

