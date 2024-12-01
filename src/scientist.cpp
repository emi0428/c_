#include "scientist.hpp"
#include <iostream>

Scientist::Scientist()
{
    // 載入圖像
    Image tempImage = LoadImage("Graphics/scientist.png");

    // 調整圖像大小
    ImageResize(&tempImage, 100, 100);

    // 將調整後的圖像轉換為紋理
    image = LoadTextureFromImage(tempImage);

    // 卸載中間的 Image 資料
    UnloadImage(tempImage);

    // 科學家的原始位置
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
    //避免我們的科學家超出遊戲框框外
    if(position.x < 0)
    {
        position.x = 0 ;
    }
}

void Scientist::MoveRight()
{
    position.x += 5;
    //避免我們的科學家超出遊戲框框外
    if(position.x > GetScreenWidth() - image.width)
    {
        position.x = GetScreenWidth() - image.width ;
    }
}

void Scientist::FireAlcohol()
{
    //讓alcohol 射出來的長度不要太長
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
