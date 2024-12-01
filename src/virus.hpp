#pragma once
#include <raylib.h>

class Virus
{
    public:
        Virus(int type , Vector2 position);
        void Update(int direction);
        void Draw();
        int GetType();
        Rectangle getRect();
        Texture2D image;
        int type;
        Vector2 position;
    private:


};