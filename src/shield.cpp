#include "shield.hpp"

Shield::Shield(Vector2 position)
{
    this -> position = position;
}

//�e�X�ڭ̪��B���� �p�G�����B���� �N�ܦ��Ǧ⪺
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
