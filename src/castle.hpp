#pragma once
#include <vector>
#include "shield.hpp"

class Castle
{
    public:
        Castle(Vector2 position);
        void Draw();
        Vector2 position;
        std::vector<Shield> shields;
        static std::vector<std::vector<int>>grid;
    private:

};