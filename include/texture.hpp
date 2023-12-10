#pragma once
#include <stdint.h>
#include <string>

class Texture
{
public:
    uint32_t ID;
    std::string type;

    Texture(const char* fp,int out_comps = 3);
    ~Texture();

    void bind(int placement = 0);
};

