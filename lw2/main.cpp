
/* 
  13. Теннисный  турнир  проходит  по  олимпийской  системе с выбываниями. В турнире участвуют 2^n игроков. Известен рейтинг каждого игрока.  Результаты турнира записаны с помощью дерева. Первоначально  задается  только  список  участников,   которым соответствуют  листья  дерева.  Известно,  что турнир прошел в полном соответствии с рейтингом игроков.  Требуется предложить распределение  или посев сильнейших игроков так,  чтобы они не встречались в начале турнира. Показать в наглядном виде дерево проведенного турнира и выдать результаты по турам (12).
*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct player
{
    string name;
    int score;
};

struct tree
{
    player player;
    tree *left, *right, *parent;
};

tree *tournament;
tree *root; // Корень
FILE *fIn;  // Исходный файл

vector<player> seedingPlayers(vector<player> &Players);

void PrintVector(vector<player> &Vector)
{
    std::cout << "Print vector" << std::endl;
    for (int i = 0; i < Vector.size(); i++)
        std::cout << Vector[i].name << " " << Vector[i].score << std::endl;
}

vector<player> ReadFileInVector()
{
    setlocale(LC_ALL, "ru");
    string pathIn = "input.txt";
    vector<player> Players;
    ifstream FileIn;
    string s;
    player rankingMember;
    FileIn.open(pathIn);
    if (!FileIn.is_open())
    {
        std::cout << "file open error" << std::endl;
    }
    else
    {
        std::cout << "file open" << std::endl;
        getline(FileIn, s);
        int i = 1;
        while (!FileIn.eof())
        {
            std::stringstream strm;
            strm.str(s);
            strm >> rankingMember.name >> rankingMember.score;
            cout << i << ". " << rankingMember.name << " " << rankingMember.score << endl;
            i++;
            Players.push_back(rankingMember);
            getline(FileIn, s);
        }
    }
    FileIn.close();
    return Players;
}

int main()
{
    vector<player> Players;
    vector<player> seedPlayers;
    int choice;
    bool exit = true;
    while (exit)
    {
        std::cout << "Choose an action" << std::endl;
        std::cout << "1. Read rating from file" << std::endl;
        std::cout << "2. Print rating" << std::endl;
        std::cout << "3. Seeding players" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            Players = ReadFileInVector();
            break;
        case 2:
            PrintVector(Players);
            break;
        case 3:
            seedPlayers = seedingPlayers(Players);
            PrintVector(seedPlayers);
            break;
        case 4:
            exit = false;
            break;
        }
    }
}

vector<player> seedingPlayers(vector<player> &Players)
{
    vector<player> distributionOfPlayers;
    int vectorSize = Players.size();
    cout << vectorSize << endl;
    int N = vectorSize / 2 - 1;
    for (int i = 0; i < N - i; i++)
    {
        distributionOfPlayers.push_back(Players[i]);
        player gamer = distributionOfPlayers.back();
        cout << gamer.name << endl;
        int j = vectorSize - i - 1;
        distributionOfPlayers.push_back(Players[j]);
        gamer = distributionOfPlayers.back();
        cout << gamer.name << endl;
        j = vectorSize / 2 - 1 - i;
        distributionOfPlayers.push_back(Players[j]);
        gamer = distributionOfPlayers.back();
        cout << gamer.name << endl;
        j = vectorSize / 2 + i;
        distributionOfPlayers.push_back(Players[j]);
        gamer = distributionOfPlayers.back();
        cout << gamer.name << endl;
    }
    return distributionOfPlayers;
}