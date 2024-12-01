#pragma once
#include <raylib.h>

class Laser 
{
    public:
        Laser(Vector2 position , int speed);
        virtual void Update();
        virtual void Draw();
        virtual Rectangle getRect();
        bool active;
    private:
        protected : Vector2 position;
        protected : int speed;

};