#pragma once
#include "scientist.hpp"
#include "castle.hpp"
#include "virus.hpp"
#include "juice.hpp"
#include "bigboss.hpp"
class Game
{
    public:
        Game();
        ~Game();
        void Draw();
        void Update();
        void HandleInput();
        bool run;
        int lives;
        int score;
        int highScore;
        Music music;
    private:
        void DeleteInactiveAlcohol();
        std::vector<Castle> CreateCastles();
        std::vector<Virus> CreateViruses();
        void MoveViruses();
        void MoveDownViruses(int distance);
        void VirusesShootSap();
        void CheckForCollions();
        void GameOver();
        void Reset();
        void InitGame();
        void checkForHighscore();
        void saveHighScoreToFile(int highscore);
        int loadHighscoreFromFile();
        Scientist Scientist;
        std::vector<Castle> castles;
        std::vector<Virus> viruses;
        int virusesDirection;
        std::vector<Juice> virusSaps;
        constexpr static float virusSapShootInterval = 0.35;
        float timeLastVirusAttack;
        Bigboss bigboss;
        float bigbossSpawnInterval;
        float timeLastSpawn;
        Sound eliminateSound;
        
};