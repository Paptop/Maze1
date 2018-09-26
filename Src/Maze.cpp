#include <limits>
#include <algorithm>
#include <cassert>
#include "Maze.h"


Maze::Maze(std::string fileName)
: m_fileName(fileName)
, m_trial(0)
, m_L(2)
, recursionCount(0)
, lineCount(0)
{
    m_prod = { {-1,0}, {0,-1}, {1,0}, {0,1} };
    InitMaze();

    //Part 2
    bool isFound = Try();

    std::cout << "PART 3. Results" << std::endl << std::endl;

    if(isFound)
    {
        std::cout << " 3.1. Path is found." << std::endl << std::endl;
        std::cout << " 3.2. Path graphically." << std::endl << std::endl;
    }
    else
    {
        std::cout << " 3.1. Path was not found." << std::endl << std::endl;
    }
    PrintMazeToConsole();
    std::cout << std::endl;


    if(isFound)
    {
        std::cout << " Rules : ";
        std::cout << "R" << m_nodes.front().Rule + 1;
        for (auto iter = std::next(m_nodes.begin(), 1); iter != m_nodes.end(); ++iter) {

            std::cout << ", R" << iter->Rule + 1;
        }
        std::cout << std::endl;

        std::cout << " Nodes : ";
        std::cout << "[" << m_agentX + 1 << ", " << m_agentY + 1 << "], ";
        std::cout << "[" << m_nodes.front().U + 1 << ", " << m_nodes.front().V + 1 << "]";
        for (auto iter = std::next(m_nodes.begin(), 1); iter != m_nodes.end(); ++iter) {

            std::cout << ", [" << iter->U + 1 << ", " << iter->V + 1 << "]";
        }
        std::cout << std::endl;
    }
    m_output.close();
}

void Maze::PrintMazeToConsole()
{
    std::cout << "Y, V ^" << std::endl;
    int size = (int)m_maze[0].size();

    for(int i = (int)m_maze.size() - 1; i >= 0 ; --i)
    {
        std::cout << " " << std::setfill(' ') << std::setw(3) << i + 1  << " | ";
        for(int c : m_maze[i])
        {
            std::cout << std::setfill(' ') << std::setw(4) << c;
        }

        std::cout << std::endl;
    }

    std::cout << "       ";
    for(int i = 0; i < size; ++i)
    {
        std::cout << "----";
    }
    std::cout << "> X, U";
    std::cout << std::endl;

    std::cout << "       ";
    for(int i = 0; i < size; ++i)
    {
        std::cout << std::setfill(' ') << std::setw(4) << i+1;
    }
    std::cout << std::endl;
}

bool Maze::Try()
{
   return Try(m_agentX,m_agentY);
}

bool Maze::Try(int x, int y)
{
    ++recursionCount;
    bool TERM = false;

    if( x == 0 || x == m_col - 1 || y == 0 || y == m_row - 1)
    {
        return true;
    }
    else
    {
       for(int k = 0; k < m_prod.size(); ++k)
       {
           int U = x + m_prod[k].first;
           int V = y + m_prod[k].second;

           m_output<< std::setfill(' ') << std::setw(5) << ++lineCount << ") ";
           for(int i  = 0 ; i < recursionCount - 1; ++i)
           {
               m_output << "-";
           }
           m_output << "R" << std::to_string(k+1) << ". U=" << U + 1 <<", "<< "V=" << V + 1 << ". ";


           if(m_maze[V][U] == 0)
           {
               // Free
               m_L++;
               m_trial++;
               m_maze[V][U]=m_L;
               m_nodes.push_back(Data(U , V, k));
               m_output << "Free. " << "L=L+1=" << m_L << " LAB[" << U + 1 <<","<< V + 1 << "]:=" << m_L << "." << std::endl;
               TERM = Try(U,V);

               if(!TERM)
               {
                   recursionCount--;
                   m_maze[V][U] = -1;
                   m_nodes.pop_back();
                   m_L--;
                   m_output<< std::setfill(' ') << std::setw(5 + (int)std::to_string(lineCount).size() + 1);
                   for(int i  = 0 ; i < recursionCount; ++i)
                   {
                       m_output << "-";
                   }
                   m_output << "Backtrack from X=" << U + 1 << ", Y=" << V + 1 <<", L="<< m_L + 1 << ". LAB[" << U + 1 <<","<< V + 1 << "]:=" << -1 << ". L:=L-1=" << m_L << "." << std::endl;
               }
           }
           else if(m_maze[V][U] == 1)
           {
               m_output << "Wall. " << std::endl;
           }
           else if(m_maze[V][U] > 1 || m_maze[V][U] == -1)
           {
               m_output << "Thread. " << std::endl;
           }
           if (TERM)
           {
               break;
           }
       }

       if(TERM)
       {
           return true;
       }
       else
       {
           return false;
       }
    }
}

void Maze::InitMaze()
{
    m_input.open(m_fileName);
    m_output.open("OUTPUT.TXT");
    std::string line;
    bool isPositionDefined = false;
    if(m_input.is_open()) {

        while (getline(m_input, line)) {
            std::vector<int> row;

            for (int i = 0; i < line.length(); ++i) {
                if (line[i] == '2')
                {
                    isPositionDefined = true;
                }

                row.push_back(line[i] - '0');
            }

            m_maze.push_back(row);
        }
    }
    assert(m_maze.size() > 0);

    std::reverse(m_maze.begin(), m_maze.end());
    m_row = static_cast<int>(m_maze.size());
    m_col = static_cast<int>(m_maze[0].size());
    std::cout << std::endl;
    std::cout << "PART 1. Data" << std::endl << std::endl;
    std::cout << "   Labyrinth" << std::endl;
    PrintMazeToConsole();
    std::cout << std::endl;

    if(!isPositionDefined)
    {
        bool isValid = true;
        while(isValid)
        {
            m_agentX = GetInt("Enter agent position X : ", 1, m_col);
            m_agentY = GetInt("Enter agent position Y : ", 1, m_col);
            m_agentX--; m_agentY--;
            if(m_maze[m_agentY][m_agentX] == 0)
            {
                m_maze[m_agentY][m_agentX] = 2;
                isValid = false;
            }
            else
            {
                std::cout << " Invalid position " << std::endl;
            }
        }
    }else
    {
        for(int i = 0; i < m_maze.size(); ++i)
        {
            for(int j = 0; j < m_maze[i].size(); ++j)
            {
                if(m_maze[i][j] == 2)
                {
                    m_agentX = j;
                    m_agentY = i;
                }
            }
        }
    }
    std::cout << std::endl;
    std::cout << " 1.2 Initial position X=" << m_agentX+1 << " Y=" << m_agentY+1 << ". L=" << 2 << "." << std::endl << std::endl;
    std::cout << std::endl;
    std::cout << "PART 2 is located in OUTPUT.TXT" << std::endl << std::endl;
    m_input.close();
}

int Maze::GetInt(std::string str, int lowerBound, int upperBound)
{
    int value = 0;
    std::cout << str;
    std::cin >> value;

    while(!std::cin || value < lowerBound || value > upperBound)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cout << " Invalid value\n";
        std::cout << str;
        std::cin >> value;
    }

    return value;
}
