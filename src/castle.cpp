#include "castle.hpp"

//創建出一個遮蔽物
std::vector<std::vector<int>>Castle::grid ={
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
        {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1}
    };

Castle::Castle(Vector2 position)
{
    this -> position = position;


    for(unsigned int row = 0 ; row < grid.size(); ++row )
    {
        for(unsigned int column = 0 ; column < grid[0].size(); ++column)
        {
            if(grid[row][column] == 1)
            {
                float pos_x = position.x + column*3;
                float pos_y = position.y + row*3 - 10;
                Shield shield = Shield({pos_x , pos_y});
                shields.push_back(shield);
            }
        }
    }
}

void Castle::Draw(){
    for(auto& block : shields)
    {
        block.Draw();
    }
}