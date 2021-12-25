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

void printMatrix(int **matrix, int fieldSize)
{
for (int i = 0; i < fieldSize; i++)
    {
        for (int j = 0; j < fieldSize; j++)
        {
            printf("%3d", matrix[j][i]);
        }
        cout << endl;
    }
    cout << endl;
}

int getOffset(int x, int y, int fieldSize)
{
    return x + y * fieldSize;
}

void findPath(int fieldSize, int x, int y, vector<char> playingField, int **visited, int **path, queue<int> &plan)
{
    int numberOfDirections = 8;
    int directionsX[numberOfDirections] = {1, -1, 2, 2, 1, -1, -2, -2};
    int directionsY[numberOfDirections] = {-2, -2, 1, -1, 2, 2, 1, -1};
    int offset = 0;
    int ix, iy;
    if (!visited[x][y])
    {
        /* проверяем не вышли ли мы за границы игрового поля, есть ли клетка 
        в массиве посещенных и можно ли через нее пройти*/
        for (int i = 0; i < numberOfDirections; i++)
        {
            ix = x + directionsX[i];
            iy = y + directionsY[i];
            offset = getOffset(ix, iy, fieldSize);
            if (ix < fieldSize && ix >= 0 && !visited[ix][iy] &&
                (playingField[offset] == FREE_CELL_SYMBOL || playingField[offset] == END_POSITION_SYMBOL))
            {
                path[ix][iy] = path[x][y] + 1;
                plan.push(ix);
                plan.push(iy);
            }
        }
        visited[x][y] = 1; /* отмечаем клетку, в которой побывали */
    }
}

void restorePath(int fieldSize, int x_start, int y_start, int x_end, int y_end, vector<char> &playingField, int **path); /* восстановление пути*/

void PrintVector(vector<char> Vector);

void ReadLineFromFile(ifstream &FileIn, vector<char> &playingField);

vector<char> ReadFileInVector();

int main()
{
    vector<char> playingField;
    playingField = ReadFileInVector();
    cout<<playingField[0]<<endl;
    static const int fieldSize = playingField[0] - '0';
    auto begin = playingField.cbegin();
    playingField.erase(begin);
    PrintVector(playingField);
    cout << fieldSize << endl;

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
            int offset = getOffset(x, y, fieldSize);
            if (playingField[offset] == STARTING_POSITION_SYMBOL)
            { /* находим начало пути*/
                x_start = x;
                y_start = y;
                cellVisitQueue.push(x); /* заносим начальную клетку */
                cellVisitQueue.push(y); /* в план посещения */
                path[x][y] = 1;
            }
            else if (playingField[offset] == END_POSITION_SYMBOL)
            { /* находим конечную точку */
                x_end = x;
                y_end = y;
            }
        }
    }
    cout << "Path" << endl;
    printMatrix(path, fieldSize);
    cout << "Visited" << endl;
    printMatrix(visited, fieldSize);
    int i = 1;
    while (!cellVisitQueue.empty())
    { /* пока очередь посещения клеток непустая*/
        x = cellVisitQueue.front();
        cellVisitQueue.pop();
        y = cellVisitQueue.front();
        cellVisitQueue.pop();
        findPath(fieldSize, x, y, playingField, visited, path, cellVisitQueue); /* продолжаем поиск пути*/
        i++;
    }
    cout << "Path" << endl;
    printMatrix(path, fieldSize);
    cout << "Visited" << endl;
    printMatrix(visited, fieldSize);

    if (!visited[x_end][y_end])
    {
        cout << "No way" << endl;
    }
    else
    {
        cout << "Y" << endl;
        cout << "x_start= " << x_start << " y_start= " << y_start << endl;
        cout << "x_end= " << x_end << " y_end= " << y_end << endl;
        restorePath(fieldSize, x_start, y_start, x_end, y_end, playingField, path);

        for (int i = 0; i < fieldSize; i++)
        {
            for (int j = 0; j < fieldSize; j++)
            {
                int offset = getOffset(j, i, fieldSize);
                cout << playingField[offset];
            }
            cout << endl;
        }
    }
    getch();
    return 0;
}

bool checkOutOfBorder(int x, int y, int fieldSize)
{
    if (x >= 0 && x < fieldSize && y >= 0 && y < fieldSize)
        return true;
    else
        return false;
}

void restorePath(int fieldSize, int x_start, int y_start, int x_end, int y_end, vector<char> &playingField, int **path) /* восстановление пути*/
{
    int x = x_end;
    int y = y_end;
    int offset = getOffset(x, y, fieldSize);
    char pathMarker = '+';
    int numberOfDirections = 8;
    int directionsX[numberOfDirections] = {1, -1, 2, 2, 1, -1, -2, -2};
    int directionsY[numberOfDirections] = {-2, -2, 1, -1, 2, 2, 1, -1};
    int ix, iy;
    while (path[x][y] != path[x_start][y_start] + 1)
    {
        for (int i = 0; i < numberOfDirections; i++)
        {
            ix = x + directionsX[i];
            iy = y + directionsY[i];
            if (checkOutOfBorder(x, y, fieldSize) && (path[ix][iy] == path[x][y] - 1) && (path[x][y] != path[x_start][y_start] + 1))
            {
                x = ix;
                y = iy;
                cout << "x_= " << x << " y_= " << y << " " << path[ix][iy] << endl;
                offset = getOffset(x, y, fieldSize);
                playingField[offset] = pathMarker;
            }
        }
    }
}

void PrintVector(vector<char> Vector)
{
    std::cout << "Print vector" << std::endl;
    for (int i = 0; i < Vector.size(); i++)
        std::cout << Vector[i] << std::endl;
}

void ReadLineFromFile(ifstream &FileIn, vector<char> &playingField)
{
    string s;
    getline(FileIn, s);
    std::cout << s << " ";
    std::cout << std::endl;
    std::stringstream ss;
    ss.str(s);
    char c2;
    string s2;
    int Number;
    while (ss >> s2)
    {
        c2 = *s2.c_str();
        playingField.push_back(c2);
    }
}

vector<char> ReadFileInVector()
{
    setlocale(LC_ALL, "ru");
    string pathIn = "input.txt";
    vector<char> playingField;
    ifstream FileIn;
    FileIn.open(pathIn);
    if (!FileIn.is_open())
    {
        std::cout << "file open error" << std::endl;
    }
    else
    {
        std::cout << "file open" << std::endl;
        while (!FileIn.eof())
        {
            ReadLineFromFile(FileIn, playingField);
        }
    }
    FileIn.close();
    return playingField;
}