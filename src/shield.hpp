#pragma once 
#include <raylib.h>

class Shield{
    public:
        Shield(Vector2 position);
        void Draw();
        Rectangle getRect();
    private:
        Vector2 position;

};