#ifndef LABIRINTH_MAZE_H
#define LABIRINTH_MAZE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iomanip>

class Maze
{
    struct Data
    {
       int U;
       int V;
       int Rule;

       Data(int U, int V, int Rule)
       : U(U)
       , V(V)
       , Rule(Rule)
       {
       }

    };

public:
    Maze(std::string fileName);
    void PrintMazeToConsole();
    bool Try();

private:
    void InitMaze();
    bool Try(int x, int y);
    int  GetInt(std::string str, int lowerBound, int upperBound);

private:
    std::vector< std::vector<int> > m_maze;
    std::vector<std::pair<int,int> > m_prod;
    std::list<Data> m_nodes;
    std::string  m_fileName;
    std::ifstream m_input;
    std::ofstream m_output;
    int m_row;
    int m_col;
    int m_agentX;
    int m_agentY;
    int m_trial;
    int m_L;
    unsigned int recursionCount;
    unsigned int lineCount;


};


#endif //LABIRINTH_MAZE_H
