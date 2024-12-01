#pragma once
#include "laser.hpp"

class Juice : public Laser
{
public:
    Juice(Vector2 position, int speed);
    void Draw() override;  // ÂÐ»\ Draw ¨ç¼Æ
};
