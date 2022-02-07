#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>
#include <locale.h>

using namespace std;

struct PCBpin
{
  int pinNum;
  int x;
  int y;
};

int getMenuItemNumber(int count);
void readFileInVector(vector<PCBpin> &PCBpins);
void printVector(vector<PCBpin> &Vector);
vector<vector<int>> resize2dVector(int n);
void printMatrix(vector<vector<int>> adjMat);
void getAdjacencyMatrix(vector<PCBpin> &PCBpins, vector<vector<int>> &adjMat);
void findPath(int startPin,
              int endPin,
              int numOfPins,
              int j,
              int &maxPathLength,
              vector<int> &currPath,
              vector<bool> &visited,
              vector<vector<int>> &adjMat);

int main()
{
  setlocale(LC_ALL, "Russian");
  system("cls");
  vector<bool> visited;
  vector<int> currentPath;
  vector<PCBpin> PCBpins;
  int numberOfPCBpins = 0;
  vector<vector<int>> adjMat;
  int startPoint = 0;
  int endPoint = 0;
  int counter = 0;
  int maxPathLength = 0;
  int tmpMaxPathLength = 0;
  int choice;
  bool exit = true;
  while (exit)
  {
    std::cout << "Choose an action" << std::endl;
    std::cout << "1. Read the file with PCB pin coordinates" << std::endl;
    std::cout << "2. Read file with PCB pin connections" << std::endl;
    std::cout << "3. Find paths" << std::endl;
    std::cout << "4. Exit" << std::endl;
    choice = getMenuItemNumber(6);
    switch (choice)
    {
    case 1:
      PCBpins.clear();
      readFileInVector(PCBpins);
      numberOfPCBpins = PCBpins.size();
      cout << numberOfPCBpins << "\n";
      printVector(PCBpins);
      break;
    case 2:
      cout << numberOfPCBpins << "\n";
      adjMat = resize2dVector(numberOfPCBpins);
      getAdjacencyMatrix(PCBpins, adjMat);
      printMatrix(adjMat);
      break;
    case 3:
      visited.resize(numberOfPCBpins);
      currentPath.resize(numberOfPCBpins);
      for (int i = 0; i < adjMat.size(); i++)
      {
        for (int j = adjMat[i].size() - 1; j > i; j--)
          {
            findPath(i, j, numberOfPCBpins, counter, tmpMaxPathLength, currentPath, visited, adjMat);
            if (tmpMaxPathLength > maxPathLength)
            {
                maxPathLength = tmpMaxPathLength;
                startPoint = i;
                endPoint = j;
            }
          }
      }
   //   findPath(startPoint, endPoint, numberOfPCBpins, counter, maxPathLength, currentPath, visited, adjMat);
      cout << "PCB pin from which you can build a path of maximum length = "
           << startPoint
           << " to "
           << endPoint
           << "\n";
      cout << "Maximum path length = " << maxPathLength << "\n";
      break;
    case 4:
      exit = false;
      break;
    }
  }
  return 0;
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

vector<vector<int>> resize2dVector(int n)
{
  vector<vector<int>> adjMat(n);
  for (int i = 0; i < n; i++)
    adjMat[i].resize(n);
  return adjMat;
}

void printVector(vector<PCBpin> &Vector)
{
  std::cout << "Print vector" << std::endl;
  for (size_t i = 0; i < Vector.size(); i++)
    std::cout << Vector[i].pinNum << " " << Vector[i].x << " " << Vector[i].y << "\n";
}

void readFileInVector(vector<PCBpin> &Vector)
{
  string pathIn;
  cout << "Enter file name: ";
  cin >> pathIn;
  ifstream fileIn(pathIn);
  if (!fileIn.is_open())
  {
    std::cout << "file open error" << std::endl;
  }
  else
  {
    PCBpin PCBpin;
    while (!fileIn.eof())
    {
      fileIn >> PCBpin.pinNum >> PCBpin.x >> PCBpin.y;
      Vector.push_back(PCBpin);
    }
    fileIn.close();
  }
}

void getAdjacencyMatrix(vector<PCBpin> &PCBpins, vector<vector<int>> &adjMat)
{
  string pathIn;
  cout << "Enter file name: ";
  cin >> pathIn;
  ifstream fileIn(pathIn);
  if (!fileIn.is_open())
  {
    std::cout << "file open error" << std::endl;
  }
  else
  {
    int startPin;
    int endPin;
    int lengthX;
    int lengthY;
    int edgeLength;
    while (!fileIn.eof())
    {
      fileIn >> startPin >> endPin;
      lengthX = PCBpins[startPin].x - PCBpins[endPin].x;
      lengthY = PCBpins[startPin].y - PCBpins[endPin].y;
      edgeLength = int(sqrt(pow(lengthX, 2) + pow(lengthY, 2)));
      adjMat[startPin][endPin] = edgeLength;
      adjMat[endPin][startPin] = edgeLength;
    }
    fileIn.close();
  }
}

void findPath(int startPin,
              int endPin,
              int numOfPins,
              int j,
              int &maxPathLength,
              vector<int> &currPath,
              vector<bool> &visited,
              vector<vector<int>> &adjMat)
{
  int i;
  currPath[j] = startPin;
  visited[startPin] = true;
  j = j + 1;
  if (startPin == endPin)
  {
    int counter = 0;
    cout << "Found the path: j = "
         << j
         << "\n";
    for (i = 0; i < j; i++)
    {
        cout << currPath[i] << "\n";
        if ((i + 1) < j)
        {
          cout << "adjMat " << currPath[i] << ", " << currPath[i+1] << " = " << adjMat[currPath[i]][currPath[i+1]] << " \n";
          counter += adjMat[currPath[i]][currPath[i+1]];
        }
    }
    cout << "\n"
         << counter << "\n";
    if (counter > maxPathLength)
      maxPathLength = counter;
  }
  else
  {
    for (i = 0; i < numOfPins; i++)
      if (!visited[i] && (adjMat[startPin][i] > 0))
      {
        findPath(i, endPin, numOfPins, j, maxPathLength, currPath, visited, adjMat);
      }
  }
  j = j - 1;
  visited[startPin] = false;
}

void printMatrix(vector<vector<int>> adjMat)
{
  for (int i = 0; i < adjMat.size(); i++)
  {
    for (int j = 0; j < adjMat[i].size(); j++)
      printf("%4d", adjMat[i][j]);
    cout << "\n";
  }
}
