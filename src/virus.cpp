#include "virus.hpp"



Virus::Virus(int type, Vector2 position)
{
    this->type = type;
    this->position = position;

    Image tempImage;

    // 設定三種不同等級的病毒並調整大小
    switch(type)
    {
    case 1:
        tempImage = LoadImage("Graphics/smallvirus.png");
        ImageResize(&tempImage, 30, 30);  
        break;
    case 2:
        tempImage = LoadImage("Graphics/mediumvirus.png");
        ImageResize(&tempImage, 35, 35);  
        break;
    case 3:
        tempImage = LoadImage("Graphics/bigvirus.png");
        ImageResize(&tempImage, 40, 40);  
        break;
    default:
        tempImage = LoadImage("Graphics/smallvirus.png");
        ImageResize(&tempImage, 30, 30);  
        break;
    }

    // 將調整過大小的圖像轉換為Image
    image = LoadTextureFromImage(tempImage);

    // 釋放圖像資源以節省記憶體空間
    UnloadImage(tempImage);
};

void Virus::Draw()
{
    DrawTextureV(image, position, WHITE);
}

int Virus::GetType()
{
    return type;
}

Rectangle Virus::getRect()
{
    return {position.x , position.y , float(image.width) , float(image.height)};
}

void Virus::Update(int direction){
    position.x += direction;
}