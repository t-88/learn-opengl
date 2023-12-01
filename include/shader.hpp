#pragma once
#include <stdint.h>
#include <string>

class Shader
{
private:
    uint32_t ID;
public:
    Shader(std::string,std::string);
    ~Shader();

    void enable();
};

