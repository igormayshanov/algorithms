/* 
  13. Теннисный  турнир  проходит  по  олимпийской  системе с выбываниями. В турнире участвуют 2^n игроков. Известен рейтинг каждого игрока.  Результаты турнира записаны с помощью дерева. Первоначально  задается  только  список  участников,   которым соответствуют  листья  дерева.  Известно,  что турнир прошел в полном соответствии с рейтингом игроков.  Требуется предложить распределение  или посев сильнейших игроков так,  чтобы они не встречались в начале турнира. Показать в наглядном виде дерево проведенного турнира и выдать результаты по турам (12) Майшанов И.Н. VS Code.
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
const int DL = 20;
using namespace std;

struct player
{
  char name[DL];
  int score;
};

struct tree
{
  int level;
  int rankingPlace;
  tree *left;
  tree *right;
  tree *parent;
};

vector<player> readFileInVector();
int getTournamentTree(tree *&tournament, vector<player> &Players);
int getTreeHeight(vector<player> &Players);
int getMenuItemNumber(int count);
void printVector(vector<player> &Vector);
void printTreePostfix(tree *ptr, int level);
void printTreeInfix(tree *ptr, int level);
int printTourResult(struct tree *ptr, int tour, vector<player> &Players);
void freeTree(tree *&tree);

int main()
{
  setlocale(LC_ALL, "RU");
  vector<player> Players;
  tree *root = NULL;
  int choice;
  bool exit = true;
  while (exit)
  {
    std::cout << "Choose an action" << std::endl;
    std::cout << "1. Read rating from file" << std::endl;
    std::cout << "2. Print rating" << std::endl;
    std::cout << "3. Seeding players" << std::endl;
    std::cout << "4. Print tournament tree" << std::endl;
    std::cout << "5. Return the result of the tour" << std::endl;
    std::cout << "6. Exit" << std::endl;
    choice = getMenuItemNumber(6);
    switch (choice)
    {
    case 1:
      freeTree(root);
      Players = readFileInVector();
      break;
    case 2:
      printVector(Players);
      break;
    case 3:
      getTournamentTree(root, Players);
      break;
    case 4:
      printTreeInfix(root, 0);
      break;
    case 5:
      if (!root)
      {
        cout << "Tree is empty \n";
        system("pause");
      }
      else
      {
        cout << "Enter tour number: ";
        int tour;
        int treeHeight = getTreeHeight(Players);
        cin >> tour;
        tour = treeHeight - tour + 1;
        cout << "\n";
        printTourResult(root, tour, Players);
        system("pause");
      }
      break;
    case 6:
      freeTree(root);
      exit = false;
      break;
    }
  }
}

vector<player> readFileInVector()
{
  setlocale(LC_ALL, "RU");
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
    if (!FileIn.eof())
      getline(FileIn, s);
    while (!FileIn.eof())
    {
      std::stringstream strm;
      strm.str(s);
      strm >> rankingMember.name >> rankingMember.score;
      Players.push_back(rankingMember);
      getline(FileIn, s);
    }
  }
  FileIn.close();
  return Players;
}

int getTreeHeight(vector<player> &Players)
{
  return log(Players.size()) / log(2.0);
}

int getTournamentTree(tree *&root, vector<player> &Players)
{
  tree *tournament;
  int treeHeight = getTreeHeight(Players);
  int numberOfPlayers = Players.size();
  tournament = new tree;
  tournament->rankingPlace = 1;
  tournament->level = 0;
  tournament->left = NULL;
  tournament->right = NULL;
  root = tournament;
  int ext = 0;
  while (!ext)
  {
    while (tournament->level != treeHeight)
    {
      if (tournament->left == NULL)
      {
        tournament->left = new tree;
        tournament->left->parent = tournament;
        tournament->left->rankingPlace = tournament->rankingPlace;
        tournament = tournament->left;
        tournament->left = NULL;
        tournament->right = NULL;
        tournament->level = tournament->parent->level + 1;
      }
      else
      {
        if (tournament->right == NULL)
        {
          tournament->right = new tree;
          tournament->right->parent = tournament;
          tournament = tournament->right;
          tournament->left = NULL;
          tournament->right = NULL;
          tournament->level = tournament->parent->level + 1;
          tournament->rankingPlace = (int(pow(2.0, tournament->level))) / 2 + tournament->parent->rankingPlace; //(2^(level)/2)+parent place
          if (tournament->rankingPlace == numberOfPlayers)
            ext = 1;
        }
        else
        {
          tournament = tournament->parent;
        }
      }
    }
    tournament = tournament->parent;
  }
  return 0;
}

void printVector(vector<player> &Players)
{
  std::cout << "Print ranking list: \n";
  for (int i = 0; i < Players.size(); i++)
  {
    printf("%4d. %-15s %5d\n", i + 1, Players[i].name, Players[i].score);
  }
}

void printTreePostfix(tree *ptr, int level)
{
  if (ptr != NULL)
  {
    printTreePostfix(ptr->left, level + 1);
    printTreePostfix(ptr->right, level + 1);
    for (int i = 0; i < level; i++)
      cout << '.';
    cout << ptr->rankingPlace << "\n";
  }
}

void printTreeInfix(tree *ptr, int level)
{
  if (ptr != NULL)
  {
    printTreeInfix(ptr->left, level + 1);
    for (int i = 0; i < level; i++)
      cout << "  ";
    cout << ptr->rankingPlace << "\n";
    printTreeInfix(ptr->right, level + 1);
  }
}

void freeTree(tree *&tree)
{
  if (tree != NULL)
  {
    freeTree(tree->left);
    freeTree(tree->right);
    delete tree;
  }
}

int getMenuItemNumber(int count)
{
  int itemNumber;
  char s[100];
  scanf("%s", s);
  while (sscanf(s, "%d", &itemNumber) != 1 || itemNumber < 1 || itemNumber > count)
  {
    printf("Incorrect input. Try again: ");
    scanf("%s", s);
  }

  return itemNumber;
}

int printTourResult(struct tree *ptr, int tour, vector<player> &Players)
{
  if (ptr)
  {
    if (ptr->level == tour)
    {
      printf("%4d %-15s %5d\n", ptr->rankingPlace, Players[ptr->rankingPlace - 1].name, Players[ptr->rankingPlace - 1].score);
    }
    printTourResult(ptr->left, tour, Players);
    printTourResult(ptr->right, tour, Players);
    if (ptr->level == tour - 1)
      printf("\n");
  }
  return 0;
}