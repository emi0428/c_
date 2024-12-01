#include "bigboss.hpp"

Bigboss::Bigboss()
{
    // ���J�Ϲ�
    Image tempImage = LoadImage("Graphics/bigboss.png");

    // �վ�Ϲ��j�p
    ImageResize(&tempImage, 100, 100);

    // �N�վ�᪺�Ϲ��ഫ�����z
    image = LoadTextureFromImage(tempImage);

    alive = false;
    // ���������� Image ���
    UnloadImage(tempImage);
}

Bigboss::~Bigboss()
{
    UnloadTexture(image);
}

void Bigboss::Spawn(){
    position.y = 45;
    int side = GetRandomValue(0,1);
    if(side == 0)
    {
        position.x = 0;
        speed = 3;
    }
    else
    {
        position.x = GetScreenWidth() - image.width;
        speed = -3;
    }
    alive = true;
}

Rectangle Bigboss::getRect()
{
    if(alive)
    {
        return {position.x , position.y , float(image.width) , float(image.height)};
    }
    else
    {
        return {position.x , position.y , 0, 0};
    }
}

void Bigboss::Update()
{
    if(alive)
    {
        position.x += speed;
        if(position.x > GetScreenWidth() - image.width || position.x < 0)
        {
            alive = false;
        }
    }
}

void Bigboss ::Draw(){
    if(alive)
    {
        DrawTextureV(image,position,WHITE);
    }
}