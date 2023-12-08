#pragma once
#include <stdint.h>

class Texture
{
private:
    uint32_t ID;
    int width, height, comps ; 
public:
    Texture(const char* fp,int out_comps = 3);
    ~Texture();

    void bind(int placement = 0);
};

