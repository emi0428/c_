#include "scientist.hpp"
#include <iostream>

Scientist::Scientist()
{
    // ���J�Ϲ�
    Image tempImage = LoadImage("Graphics/scientist.png");

    // �վ�Ϲ��j�p
    ImageResize(&tempImage, 100, 100);

    // �N�վ�᪺�Ϲ��ഫ�����z
    image = LoadTextureFromImage(tempImage);

    // ���������� Image ���
    UnloadImage(tempImage);

    // ��Ǯa����l��m
    position.x = (GetScreenWidth() - image.width)/2;
    position.y = GetScreenHeight() - image.height - 50;
    lastFireAlcoholTime  = 0.0;
    alcoholSpraySound = LoadSound("Sounds/alcohol.ogg");
};

Scientist::~Scientist()
{
    UnloadTexture(image);
    UnloadSound(alcoholSpraySound);
};

void Scientist::Draw()
{
    DrawTextureV(image, position, WHITE);
};

void Scientist::MoveLeft()
{
    position.x -= 5;
    //�קK�ڭ̪���Ǯa�W�X�C���خإ~
    if(position.x < 0)
    {
        position.x = 0 ;
    }
}

void Scientist::MoveRight()
{
    position.x += 5;
    //�קK�ڭ̪���Ǯa�W�X�C���خإ~
    if(position.x > GetScreenWidth() - image.width)
    {
        position.x = GetScreenWidth() - image.width ;
    }
}

void Scientist::FireAlcohol()
{
    //��alcohol �g�X�Ӫ����פ��n�Ӫ�
    if(GetTime() - lastFireAlcoholTime >= 0.35)
    {
        lasers.push_back(Laser({position.x + image.width/2 -2,position.y},-6));
        lastFireAlcoholTime = GetTime();
        PlaySound(alcoholSpraySound);
    }
}

Rectangle Scientist::getRect()
{
    return {position.x , position.y , static_cast<float>(image.width) , static_cast<float>(image.height)};
}

void Scientist::Reset()
{
    position.x = (GetScreenWidth()- image.width) /2.0f;
    position.y = GetScreenHeight() - image.height -50;
    lasers.clear();
}
