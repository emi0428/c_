#pragma once
#include <raylib.h>

class Bigboss {
    public:
        Bigboss();
        ~Bigboss();
        void Update();
        void Draw();
        void Spawn();
        Rectangle getRect();
        bool alive;
    private:
        Vector2 position;
        Texture2D image;
        int speed;
};