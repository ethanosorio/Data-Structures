#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include <iostream>
#include <cstdlib>

#include "MyDisjointSets_e259o067.h"

class MyMaze 
{
  public:
	
    // constructor
	explicit MyMaze(const size_t r, const size_t c) : 
        dSets(r*c),
        numRows(r),
        numCols(c)
	{
        // code begins
        leftRightWalls = new bool*[r];
        topDownWalls = new bool*[r];
        for(int i = 0; i < r; i++){
            leftRightWalls[i] = new bool[c];
            topDownWalls[i] = new bool[c];
            for(int j = 0; j < c; j++){
                leftRightWalls[i][j] = true;
                topDownWalls[i][j] = true;
            }
        }
        topDownWalls[r-1][c-1] = false;
        // code ends
	}

    // destructor
    ~MyMaze(void)
    {
        // code begins
        for(int i = 0; i < numRows; i++){
            delete[] leftRightWalls[i];
            delete[] topDownWalls[i];
        }
        delete[] leftRightWalls;
        delete[] topDownWalls;
        // code ends
    }

    // builds a random maze
    // the top wall of the first cell (the top-left corner) should be open as the entrance
    // the bottom wall of the last cell (the bottom-right corner) should be open as the exit
    // algorithm hint: keep breaking walls until the entrance and exit become connected
	void buildMaze(void)
	{
        // code begins
        while(dSets.numDisjointSets() > 1){
            int cell1 = rand()%(numRows*numCols);
            int cell2 = cell1;
            char wall = 'n';
            while(wall == 'n'){
                switch(rand()%4) {
                    case 0: //top
                        if(cell1 >= numCols){ cell2 = cell1 - numCols; wall = 'b'; }
                        break;
                    case 1: //right
                        if((cell1 + 1)/numCols == cell1/numCols){ cell2++; wall = 'r'; }
                        break;
                    case 2: //bottom
                        if(cell1 + numCols < numRows*numCols){ cell2 = cell1 + numCols; wall = 'b'; }
                        break;
                    case 3: //left
                        if((cell1 - 1)/numCols == cell1/numCols){ cell2--; wall = 'r'; }
                        break;
                }
            }
            if(cell2 < cell1) std::swap(cell2, cell1);
            if(dSets.find(cell1) != dSets.find(cell2)){
                if(wall == 'b')
                    topDownWalls[cell1/numCols][cell1%numCols] = false;
                if(wall == 'r')
                    leftRightWalls[cell1/numCols][cell1%numCols] = false;
                dSets.unionSets(cell1, cell2);
            }
        }
        // code ends
	}

    // prints the maze
    // for top-down walls you can use '-' (ASCII code 45)
    // for left-right walls you can use '|' (ASCII code 124)
    // feel free to try out other symbols if your system supports extended ASCII and/or UTF-8
    void printMaze(std::ostream& out = std::cout)
    {
        // code begins
        std::cout << "\nDISJOINT SETS MAZE GENERATOR\nSIZE: " << numRows << "x" << numCols << "\n\n  |\n  v";
        std::cout << "\n+   +";
        for(int i=0; i<numCols-1; i++) std::cout << "---+";
        std::cout << std::endl;
        for(int i = 0; i < numRows; i++) {
            std::cout << "|";
            for (int j = 0; j < numCols; j++){
                if(leftRightWalls[i][j]) std::cout << "   |";
                else std::cout << "    ";
            }
            std::cout << std::endl;
            std::cout << "+";
            for (int j = 0; j < numCols; j++){
                if(topDownWalls[i][j]) std::cout << "---+";
                else std::cout << "   +";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "IF THE MAZE IS IMPROPERLY DISPLAYED, INCREASE TERMINAL SIZE\n\n";
        // code ends
    }

  private:
	size_t numRows = 0;         // the number of rows of the maze
	size_t numCols = 0;         // the number of columns of the maze
    MyDisjointSets dSets;       // the disjoint set class that keeps track of maze cell connection
    bool** leftRightWalls;      // the 2D array that indicates whether walls between left-right neighbors present
    bool** topDownWalls;        // the 2D array that indicates whether walls between the top-down neighbors preset

};


#endif // __MYMAZE_H__