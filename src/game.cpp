#include "game.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
Game::Game()
{
    music = LoadMusicStream("Sounds/tension.ogg");
    eliminateSound = LoadSound("Sounds/eliminate.ogg");
    PlayMusicStream(music);
    InitGame();
}

Game::~Game()
{
    UnloadMusicStream(music);
    UnloadSound(eliminateSound);
}

void Game::Update()
{
    if (run)
    {
    double currentTime = GetTime();
    if(currentTime - timeLastSpawn > bigbossSpawnInterval)
    {
        bigboss.Spawn();
        timeLastSpawn = GetTime();
        bigbossSpawnInterval = GetRandomValue(10,20);
    }
    for(auto& laser : Scientist.lasers)
    {
        laser.Update();
    }

    MoveViruses();

    VirusesShootSap();
    for(auto& sap: virusSaps)
    {
        sap.Update();
    }

    DeleteInactiveAlcohol();
    bigboss.Update();
    CheckForCollions();
    }
    else
    {
        if(IsKeyDown(KEY_ENTER))
        {
            Reset();
            InitGame();
        }

    }

}

void Game::Draw()
{
    Scientist.Draw();

    for(auto& laser : Scientist.lasers)
    {
        laser.Draw();
    }

    for(auto& castle : castles)
    {
        castle.Draw();
    }

    for(auto& virus : viruses)
    {
        virus.Draw();
    }

    for(auto& sap: virusSaps)
    {
        sap.Draw();
    }

    bigboss.Draw();
    
};

//這邊是做scientist 基本的左右控制，Enter是發射鍵
void Game::HandleInput()
{
    if(run)
    {
    if(IsKeyDown(KEY_LEFT))
    {
        Scientist.MoveLeft();
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        Scientist.MoveRight();
    }
    else if(IsKeyDown(KEY_UP))
    {
        Scientist.FireAlcohol();
    }
    }

}

void Game::DeleteInactiveAlcohol() 
{
    for(auto it = Scientist.lasers.begin(); it != Scientist.lasers.end();)
    {
        if(!it ->active)
        {
            it = Scientist.lasers.erase(it);
        }
        else
        {
            ++ it;
        }
    }
    for(auto it = virusSaps.begin(); it != virusSaps.end();)
    {
        if(!it ->active)
        {
            it = virusSaps.erase(it);
        }
        else
        {
            ++ it;
        }
    }
}

//創造六個倒下的桌子當掩護
std::vector<Castle> Game::CreateCastles()
{
    int CastleWidth = Castle::grid[0].size()*3;
    float gap = (GetScreenWidth()-(6*CastleWidth))/7;

    for(int i = 0 ; i < 6 ; i++)
    {
        float offsetX = (i+1)*gap + i * CastleWidth;
        castles.push_back(Castle({offsetX,float(GetScreenHeight()-150)}));
    }
    return castles;

}

//製作我們想要的病毒
std::vector<Virus> Game::CreateViruses()
{
    std::vector<Virus> viruses;
    for(int row = 0 ; row < 5 ; row++)
    {
        for(int column = 0 ; column < 11 ; column++)
        {
            int virusType;
            if(row == 0)
            {
                virusType = 3;
            }
            else if (row == 1 || row == 2)
            {
                virusType = 2;
            }
            else{
                virusType = 1;
            }
            float x = 75 + column * 55;
            float y = 110 + row * 55;
            viruses.push_back(Virus(virusType,{x,y}));
        }
    }
    return viruses;

}

//移動病毒
void Game::MoveViruses() 
{
    for(auto& virus : viruses)
    {
        //避免病毒移出視窗外
        if(virus.position.x + virus.image.width > GetScreenWidth())
        {
            virusesDirection = -1;
            MoveDownViruses(4);
        }
        if(virus.position.x < 0)
        {
            virusesDirection = 1;
            MoveDownViruses(4);
        }
        virus.Update(virusesDirection);
    }
    


}

//如果病毒到底也是病毒勝利
void Game::MoveDownViruses(int distance) {
    for(auto& virus : viruses)
    {
        virus.position.y += distance;
    }
}

//病毒射出病毒汁液企圖感染科學家
void Game::VirusesShootSap() 
{
    double currentTime = GetTime();
    if(currentTime - timeLastVirusAttack >= virusSapShootInterval && !viruses.empty()) 
    {
        int randomIndex = GetRandomValue(0, viruses.size()-1);
        Virus& virus = viruses[randomIndex];
        virusSaps.push_back(Juice({virus.position.x + virus.image.width/2,virus.position.y+virus.image.height},6));
        timeLastVirusAttack = GetTime();
    }
   
}

//如果打到病毒就消滅，沒有就繼續
//如果病毒打到桌子，桌子會慢慢被侵蝕
void Game::CheckForCollions() 
{
    // 科學家的酒精和病毒之間的碰撞檢測
    for (auto& alcohol : Scientist.lasers)
    {
        auto it = viruses.begin();
        while (it != viruses.end())
        {
            if (CheckCollisionRecs(it->getRect(), alcohol.getRect()))
            {
                PlaySound(eliminateSound);
                if(it ->type == 1)
                {
                    score += 100;
                }
                else if(it -> type == 2)
                {
                    score += 200;
                }
                else if(it -> type == 3)
                {
                    score += 300;
                }
                checkForHighscore();
                it = viruses.erase(it);
                alcohol.active = false;
            }
            else
            {
                ++it;
            }
        }

        //最大隻的病毒
        if(CheckCollisionRecs(bigboss.getRect() , alcohol.getRect()))
        {
            bigboss.alive = false;
            alcohol.active = false;
            score += 500;
            checkForHighscore();
            SetSoundVolume(eliminateSound, 0.05f);
            PlaySound(eliminateSound);
        }
    }

    // 病毒汁液和桌子之間的碰撞檢測
    for (auto& castle : castles)
    {
        auto shieldIt = castle.shields.begin();
        while (shieldIt != castle.shields.end())
        {
            bool collisionDetected = false;

            // 對 virusSaps 中的每一個 Juice 進行碰撞檢測
            for (auto& sap : virusSaps)
            {
                if (CheckCollisionRecs(shieldIt->getRect(), sap.getRect()))
                {
                    collisionDetected = true;
                    sap.active = false; // 使這個汁液失效
                    break; // 一旦檢測到碰撞就不需要再檢查這個 sap
                }
            }

            // 如果有碰撞發生，就移除盾牌
            if (collisionDetected)
            {
                shieldIt = castle.shields.erase(shieldIt);
            }
            else
            {
                ++shieldIt;
            }
        }
    }

    //病毒汁液射到科學家身上
    for(auto& sap : virusSaps)
    {
        if(CheckCollisionRecs(sap.getRect() , Scientist.getRect()))
        {
            sap.active = false;
            lives --;
            if(lives == 0)
            {
                GameOver();
            }
        
        }
    }

    //病毒碰到我的障礙物桌子
    for(auto& virus : viruses)
    {
        for(auto& castle : castles)
        {
            auto it = castle.shields.begin();
            while(it !=  castle.shields.end())
            {
                if(CheckCollisionRecs(it -> getRect() , virus.getRect()))
                {
                    it = castle.shields.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }
        if(CheckCollisionRecs(virus.getRect(),Scientist.getRect()))
        {
            GameOver();
        }
    }

}

void Game::GameOver()
{
    //把畫面freeze住
   run = false;
}

void Game::Reset()
{
    Scientist.Reset();
    viruses.clear();
    virusSaps.clear();
    castles.clear();
}

//初始化遊戲
void Game::InitGame()
{
    castles = CreateCastles();
    viruses = CreateViruses();
    virusesDirection = 1;
    timeLastVirusAttack = 0.0;
    timeLastSpawn = 0.0;
    lives = 5;
    score = 0 ;
    run = true;
    highScore = loadHighscoreFromFile();
    bigbossSpawnInterval = GetRandomValue(10,20);
}

void Game::checkForHighscore()
{
    if(score > highScore)
    {
        highScore = score;
        saveHighScoreToFile(highScore);
    }
}

//Read I/O file
#include <iostream>
#include <fstream>
#include <exception>

void Game::saveHighScoreToFile(int highscore)
{
    try
    {
        // 嘗試打開文件並寫入數據
        std::ofstream highscoreFile("highscore.txt");
        if (!highscoreFile.is_open())
        {
            // 如果文件無法打開，則拋出異常
            throw std::ios_base::failure("Fail to open highscore file");
        }

        // 嘗試寫入分數
        highscoreFile << highscore;
        if (highscoreFile.fail())
        {
            throw std::ios_base::failure("Fail to write highscore to file");
        }

        highscoreFile.close();
    }
    catch (const std::ios_base::failure& e)
    {
        // 捕捉文件打開或寫入失敗的異常
        std::cerr << "I/O Exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        // 捕捉其他標準庫中的異常
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        // 捕捉任何其他未知的異常
        std::cerr << "Unknown exception occurred." << std::endl;
    }
}



int Game::loadHighscoreFromFile()
{
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open())
    {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "Fail to save highscore to file" << std::endl;
    }
    return loadedHighscore;
}
