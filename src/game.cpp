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

//�o��O��scientist �򥻪����k����AEnter�O�o�g��
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

//�гy���ӭˤU����l���@
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

//�s�@�ڭ̷Q�n���f�r
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

//���ʯf�r
void Game::MoveViruses() 
{
    for(auto& virus : viruses)
    {
        //�קK�f�r���X�����~
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

//�p�G�f�r�쩳�]�O�f�r�ӧQ
void Game::MoveDownViruses(int distance) {
    for(auto& virus : viruses)
    {
        virus.position.y += distance;
    }
}

//�f�r�g�X�f�r�ĲG���ϷP�V��Ǯa
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

//�p�G����f�r�N�����A�S���N�~��
//�p�G�f�r�����l�A��l�|�C�C�Q�I�k
void Game::CheckForCollions() 
{
    // ��Ǯa���s��M�f�r�������I���˴�
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

        //�̤j�����f�r
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

    // �f�r�ĲG�M��l�������I���˴�
    for (auto& castle : castles)
    {
        auto shieldIt = castle.shields.begin();
        while (shieldIt != castle.shields.end())
        {
            bool collisionDetected = false;

            // �� virusSaps �����C�@�� Juice �i��I���˴�
            for (auto& sap : virusSaps)
            {
                if (CheckCollisionRecs(shieldIt->getRect(), sap.getRect()))
                {
                    collisionDetected = true;
                    sap.active = false; // �ϳo�ӥĲG����
                    break; // �@���˴���I���N���ݭn�A�ˬd�o�� sap
                }
            }

            // �p�G���I���o�͡A�N�����޵P
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

    //�f�r�ĲG�g���Ǯa���W
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

    //�f�r�I��ڪ���ê����l
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
    //��e��freeze��
   run = false;
}

void Game::Reset()
{
    Scientist.Reset();
    viruses.clear();
    virusSaps.clear();
    castles.clear();
}

//��l�ƹC��
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
        // ���ե��}���üg�J�ƾ�
        std::ofstream highscoreFile("highscore.txt");
        if (!highscoreFile.is_open())
        {
            // �p�G���L�k���}�A�h�ߥX���`
            throw std::ios_base::failure("Fail to open highscore file");
        }

        // ���ռg�J����
        highscoreFile << highscore;
        if (highscoreFile.fail())
        {
            throw std::ios_base::failure("Fail to write highscore to file");
        }

        highscoreFile.close();
    }
    catch (const std::ios_base::failure& e)
    {
        // ������󥴶}�μg�J���Ѫ����`
        std::cerr << "I/O Exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        // ������L�зǮw�������`
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        // ���������L���������`
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
