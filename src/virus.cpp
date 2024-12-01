#include "virus.hpp"



Virus::Virus(int type, Vector2 position)
{
    this->type = type;
    this->position = position;

    Image tempImage;

    // �]�w�T�ؤ��P���Ū��f�r�ýվ�j�p
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

    // �N�վ�L�j�p���Ϲ��ഫ��Image
    image = LoadTextureFromImage(tempImage);

    // ����Ϲ��귽�H�`�ٰO����Ŷ�
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