/*Дана шахматная доска, состоящая из NxN клеток, несколько из них занято другими фигурами.
Провести ходом коня через незанятые клетки путь минимальной длины из начальной клетки в конечную.
Ограничения : 2 <= N <= 500, время 1 с.
Ввод из файла input.txt.В первой строке задано число N.
В следующих N строках содержится по N символов.
Символом # обозначена занятая клетка, точкой - незанятая клетка, @-исходное и конечное положение коня(таких символов два).Вывод в файл output.txt.
Если путь построить невозможно, вывести No, в противном случае вывести такую же карту, как и на входе, 
но пометить символом @ все промежуточные положения коня(11) 
Майшанов И.Н. VS Code*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <conio.h>

using namespace std;

static const char BUSY_CELL_SYMBOL = '#';
static const char FREE_CELL_SYMBOL = '.';
static const char STARTING_POSITION_SYMBOL = '@';
static const char END_POSITION_SYMBOL = 'X';
const int numberOfDirections = 8;
const int directionsX[numberOfDirections] = {1, -1, 2, 2, 1, -1, -2, -2};
const int directionsY[numberOfDirections] = {-2, -2, 1, -1, 2, 2, 1, -1};

void findPath(int fieldSize, int x, int y, vector<vector<char>> playingField, int **visited, int **path, queue<int> &plan);

void restorePath(int fieldSize, int x_start, int y_start, int x_end, int y_end, vector<vector<char>> &playingField, int **path);

void printVector2(int fieldSize, vector<vector<char>> playingField);

int main()
{
    int fieldSize;
    scanf("%d", &fieldSize);
    cout << fieldSize << endl;
    vector<vector<char>> playingField(fieldSize, vector<char>(fieldSize));
    int x_start, y_start, x_end, y_end, x, y;
    queue<int> cellVisitQueue;
    int **visited = new int *[fieldSize];
    int **path = new int *[fieldSize];
    for (int y = 0; y < fieldSize; y++)
    {
        visited[y] = new int[fieldSize]; /* массив для хранения информации о посещении клеток*/
        path[y] = new int[fieldSize];    /* массив для хранения найденных путей */
        for (int x = 0; x < fieldSize; x++)
        {
            visited[y][x] = 0;
            path[y][x] = -1;
            scanf(" %c", &playingField[y][x]);
            if (playingField[y][x] == STARTING_POSITION_SYMBOL)
            {
                x_start = x;
                y_start = y;
                cellVisitQueue.push(x);
                cellVisitQueue.push(y);
                path[y][x] = 1;
            }
            else if (playingField[y][x] == END_POSITION_SYMBOL)
            {
                x_end = x;
                y_end = y;
            }
        }
    }
    while (!cellVisitQueue.empty())
    {
        x = cellVisitQueue.front();
        cellVisitQueue.pop();
        y = cellVisitQueue.front();
        cellVisitQueue.pop();
        findPath(fieldSize, x, y, playingField, visited, path, cellVisitQueue);
    }
    if (!visited[y_end][x_end])
    {
        cout << "No" << endl;
    }
    else
    {
        cout << "Yes" << endl;
        restorePath(fieldSize, x_start, y_start, x_end, y_end, playingField, path);
        printVector2(fieldSize, playingField);
    }
    return 0;
}

bool checkOutOfBorder(int x, int y, int fieldSize)
{
    if (x >= 0 && x < fieldSize && y >= 0 && y < fieldSize)
        return true;
    else
        return false;
}

void findPath(int fieldSize, int x, int y, vector<vector<char>> playingField, int **visited, int **path, queue<int> &plan)
{
    int offset = 0;
    int ix, iy;
    if (!visited[y][x])
    {
        for (int i = 0; i < numberOfDirections; i++)
        {
            ix = x + directionsX[i];
            iy = y + directionsY[i];
            if (checkOutOfBorder(ix, iy, fieldSize) && !visited[iy][ix] &&
                (playingField[iy][ix] == FREE_CELL_SYMBOL || playingField[iy][ix] == END_POSITION_SYMBOL))
            {
                path[iy][ix] = path[y][x] + 1;
                plan.push(ix);
                plan.push(iy);
            }
        }
        visited[y][x] = 1;
    }
}

void restorePath(int fieldSize, int x_start, int y_start, int x_end, int y_end, vector<vector<char>> &playingField, int **path)
{
    int x = x_end;
    int y = y_end;
    char pathMarker = '@';
    int ix, iy;
    while (path[y][x] != path[y_start][x_start] + 1)
    {
        for (int i = 0; i < numberOfDirections; i++)
        {
            ix = x + directionsX[i];
            iy = y + directionsY[i];
            if (checkOutOfBorder(ix, iy, fieldSize) && (path[y][x] != path[y_start][x_start] + 1) && (path[iy][ix] == path[y][x] - 1))
            {
                x = ix;
                y = iy;
                playingField[y][x] = pathMarker;
            }
        }
    }
}

void printVector2(int fieldSize, vector<vector<char>> playingField)
{
    for (int i = 0; i < fieldSize; i++)
    {
        for (int j = 0; j < fieldSize; j++)
        {
            printf("%c", playingField[i][j]);
        }
        printf("\n");
    }
}