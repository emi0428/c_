#include "shield.hpp"

Shield::Shield(Vector2 position)
{
    this -> position = position;
}

//畫出我們的遮蔽物 如果打中遮蔽物 就變成灰色的
void Shield::Draw()
{
    DrawRectangle(position.x , position.y ,3,3,{210,200,50,255});
}

Rectangle Shield::getRect()
{
    Rectangle rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.width = 3;
    rect.height = 3;
    return rect;
}
