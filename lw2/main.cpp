/*
  13. Теннисный  турнир  проходит  по  олимпийской  системе с выбываниями. В турнире участвуют 2^n игроков.
  Известен рейтинг каждого игрока.  Результаты турнира записаны с помощью дерева.
  Первоначально  задается  только  список  участников,   которым соответствуют  листья  дерева.
  Известно,  что турнир прошел в полном соответствии с рейтингом игроков.
  Требуется предложить распределение  или посев сильнейших игроков так,  чтобы они не встречались в начале турнира.
  Показать в наглядном виде дерево проведенного турнира и выдать результаты по турам (12) Майшанов И.Н. VS Code.
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
#include <windows.h>
#include <queue>

const int DL = 20; // максимальная длина
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
void printLeafs(tree *ptr, int level);
void printTreeLevels(tree *ptr, int level);
void traverseDepthFirst(tree *root, int treeHeight);
int printTourResult(struct tree *ptr, int tour, vector<player> &Players, bool &outputParent);
void freeTree(tree *&tree);

int main()
{
  setlocale(LC_ALL, "RU");
  vector<player> Players;
  vector<player> seedPlayers;
  tree *root = NULL;
  int treeHeight=0;
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
            treeHeight = getTreeHeight(Players);
            break;
          case 2:
            printVector(Players);
            break;
          case 3:
            getTournamentTree(root, Players);
            break;
          case 4:
            printTreeInfix(root, 0);
            //printLeafs(root, 0);
            for (int i = treeHeight; i >= 0; i--)
              {
                printf("Level: %d\n", i);
                printTreeLevels(root, i);
              }
            cout << "Tree traversal: \n";
            traverseDepthFirst(root, treeHeight);
            break;
          case 5:
            if (!root)
              {
                printf("Empty tree \n");
                system("pause");
              }
            else
              {
                cout << "Enter tour number: ";
                int tour;
                bool outputParent = false;
                cin >> tour;
                tour = treeHeight - tour + 1;
                cout << "\n";
                printTourResult(root, tour, Players, outputParent);
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
  string pathIn;
  vector<player> Players;
  ifstream FileIn;
  string s;
  player rankingMember;
  cout << "Enter file name: ";
  cin >> pathIn;
  FileIn.open(pathIn);
  if (!FileIn.is_open())
    {
      std::cout << "file open error" << std::endl;
    }
  else
    {
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

int getTreeHeight(vector<player> &Players)
{
  return log(Players.size()) / log(2.0);
}

int getTournamentTree(tree *&root, vector<player> &Players)
{
  tree *tournament;
  int treeHeight = getTreeHeight(Players);
  int numberOfPlayers = Players.size();
  int numberOfNodes = numberOfPlayers * 2 - 1;
  if (!numberOfPlayers)
    {
      return 0;
    }
  else
    {
      tournament = new tree;
      tournament->rankingPlace = 1;
      tournament->level = 0;
      tournament->left = NULL;
      tournament->right = NULL;
      tournament->parent = NULL;
      root = tournament;
      int ext = 0;
      int counter = 1;
      while (ext != 1)
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
                  counter++;
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
                      tournament->rankingPlace = (int(pow(2.0, tournament->level))) + 1 - tournament->parent->rankingPlace; //(2^(уровень)/2)+место отца
                      counter++;
                      if (counter == numberOfNodes)
                        ext = 1;
                    }
                  else // Если и слева и справа есть значения, то подняться на 1 уровень вверх
                    {
                      tournament = tournament->parent;
                    }
                }
            }
          tournament = tournament->parent;
        }
      return 0;
    }
}

void printVector(vector<player> &Vector)
{
  std::cout << "Print vector" << std::endl;
  for (int i = 0; i < Vector.size(); i++)
    std::cout << Vector[i].name << " " << Vector[i].score << std::endl;
}

void printTreeLevels(tree *ptr, int level)
{
  if (ptr == NULL)
    {
      return;
    }
  else
    {
      printTreeLevels(ptr->left, level - 1);
      if (level == 0)
        {
          cout << ptr->rankingPlace << "\n\n";
        }
      printTreeLevels(ptr->right, level - 1);
    }
}

int printTreeLevels1(struct tree *ptr, int tour, vector<player> &Players, bool &outputParent)
{
  if (ptr)
    {
      if (ptr->level == tour)
        {
          outputParent = (outputParent == true) ? false : true;
          printf("%4d %-15s %5d\n", ptr->rankingPlace);
          if (outputParent && ptr->parent != NULL)
            printf("  %4d %-15s %5d\n", ptr->parent->rankingPlace);
        }
      printTourResult(ptr->left, tour, Players, outputParent);
      printTourResult(ptr->right, tour, Players, outputParent);
      if (ptr->level == tour - 1)
        printf("\n");
    }
  return 0;
}

void traverseDepthFirst(tree *root, int treeHeight)
{
  if(!root)
    {
      return;
    }
  else
    {
      queue<tree *> childrenQueue;
      childrenQueue.push(root);
      string stringBefor;
      string stringAfter;
      int counter = treeHeight;
      size_t num, num1;
      //cout << "counter = " << counter << "\n";
      num = (pow(2, counter))-1;
      //cout << "num = " << num << "\n";
      stringBefor.insert(0, num, ' ');
      int i = 0;
      while (!childrenQueue.empty())
        {
          tree *node = childrenQueue.front();
          childrenQueue.pop();
          cout << stringBefor << node->rankingPlace << stringAfter;
          i++;
          if (node->left != NULL)
            childrenQueue.push(node->left);
          if (node->right != NULL)
            childrenQueue.push(node->right);
          if ((i == (int(pow(2, treeHeight - counter)))) && (counter >= 0))
            {
              counter--;
              //cout << "counter in if = " << counter << "\n";
              i = 0;
              stringBefor = "";
              num = (pow(2, counter)) - 1;
              cout << "num in if =" << stringBefor << num << "\n";
              stringBefor.insert(0, num, ' ');
              cout << "\n" << stringBefor;
              stringAfter = "";
              num1 = (pow(2, counter));
            //  cout << "num1 in if = " << num1 << "\n";
              stringAfter.insert(0, num1, ' ');
            }
        }
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

void printLeafs(tree *ptr, int level)
{
  if (ptr)
    {
      printLeafs(ptr->left, level + 1);
      if (!ptr->left && !ptr->right)
        cout << ptr->rankingPlace << endl;
      printLeafs(ptr->right, level + 1);
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

int printTourResult(struct tree *ptr, int tour, vector<player> &Players, bool &outputParent)
{
  if (ptr)
    {
      if (ptr->level == tour)
        {
          outputParent = (outputParent == true) ? false : true;
          printf("%4d %-15s %5d\n", ptr->rankingPlace, Players[ptr->rankingPlace - 1].name, Players[ptr->rankingPlace - 1].score);
          if (outputParent && (ptr->parent != NULL))
            {
              printf("                         %4d %-15s %5d\n",
                     ptr->parent->rankingPlace,
                     Players[ptr->parent->rankingPlace - 1].name,
                     Players[ptr->parent->rankingPlace - 1].score);
            }
        }
      printTourResult(ptr->left, tour, Players, outputParent);
      printTourResult(ptr->right, tour, Players, outputParent);
      if (ptr->level == tour - 1)
        printf("\n");
    }
  return 0;
}
