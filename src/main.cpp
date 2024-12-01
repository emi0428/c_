#include <raylib.h>
#include "game.hpp"
#include <string>

std::string FormatWithLeadingZeros(int number , int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    return numberText = std::string(leadingZeros,'0') + numberText;
}

int main() 
{
    //創建視窗
    Color white = {245,255,255,255};
    Color black = {0,0,0,255};
    int windowWidth = 750;
    int windowHeight = 700;

     // 使用 SetConfigFlags 設定支持 Unicode 字符
    SetConfigFlags(FLAG_WINDOW_HIGHDPI); // 確保高 DPI 支持

    InitWindow(windowWidth ,windowHeight ,"SAY GOODBYE TO COVID") ;
    InitAudioDevice();
    
    Font font = LoadFontEx("Font/monogram.ttf" , 64 ,0,0);
    Image heartImageData = LoadImage("Graphics/lives.png");
    ImageResize(&heartImageData, 30, 30);
    Texture2D heartImage = LoadTextureFromImage(heartImageData);
    UnloadImage(heartImageData);

    SetTargetFPS(60);

    Game Game;

    //主要遊戲的迴圈
    while(WindowShouldClose() == false)
    {
        UpdateMusicStream(Game.music);
        Game.HandleInput();
        Game.Update();
        BeginDrawing();
        ClearBackground(white);
        DrawRectangleRoundedLines({10,10,730,680},0.18f,3,black);
        DrawLineEx({10,628},{740,628},1,black);
        if(Game.run)
        {
            DrawTextEx(font,"GOOD LUCK!", {530,640},34,2,black);
        }
        else
        {
            DrawTextEx(font,"GAME OVER...", {515,640},34,2,black);
        }

        float x = 50.0;
        for(int i = 0 ; i < Game.lives ; i++)
        {
            DrawTextureV(heartImage,{x,640},WHITE);
            x += 50;
        }

        DrawTextEx(font,"SCORE",{50,10},34,2,black);
        std::string scoreText = FormatWithLeadingZeros(Game.score,5);
        DrawTextEx(font,scoreText.c_str(),{50,30},34,2,black);
        DrawTextEx(font,"HIGH-SCORE",{530,10},34,2,black);
        std::string highscoreText = FormatWithLeadingZeros(Game.highScore,5);
        DrawTextEx(font,highscoreText.c_str(),{530,30},34,2,black);
        Game.Draw();
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();
}