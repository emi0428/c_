#pragma once
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class Scientist
{
    public:
        Scientist();
        ~Scientist();
        void Draw();
        void MoveLeft();
        void MoveRight();
        void FireAlcohol();
        Rectangle getRect();
        void Reset();
        std::vector<Laser> lasers;
    private:
        Texture2D image;
        Vector2 position;
        double lastFireAlcoholTime;
        Sound  alcoholSpraySound;
};