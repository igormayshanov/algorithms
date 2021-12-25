/*Имеются   N   линейных   списков,   имеющих  одинаковую
структуру и упорядоченных по возрастанию  некоторого ключевого
поля. Создать общий упорядоченный список, используя  имеющуюся
частичную сортировку (8) Майшанов И.Н. VS Code*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct list
{
    int field;
    list *next;
};

list *CreateNewNode(int NewField);

list *FindPlaceToInsert(list *First, int NewField);

void AddFirst(list *&First, list *NewNode);

void AddAfter(list *CurrNode, list *NewNode);

void AddBefore(list *&First, list *Prev, list *NewNode);

void AddLast(list *&First, list *NewNode);

void PrintList(list *First);

void PrintVector(vector<list *> VectorOfList);

void ReadLineFromFile(ifstream &FileIn, vector<list *> &VectorOfList);

vector<list *> ReadFileInList();

list *MergeLists(list *First1, list *First2);

list *CopyList(list *curr);

void ClearList(list *&First);

int main()
{
    vector<list *> VectorOfList;
    list *ResultList = NULL;
    int choice;
    bool exit = true;
    while (exit)
    {
        std::cout << "Choose an action" << std::endl;
        std::cout << "1. Read numbers from file to the lists" << std::endl;
        std::cout << "2. Print lists" << std::endl;
        std::cout << "3. Merge lists and print common list" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            VectorOfList = ReadFileInList();
            break;
        case 2:
            for (int i = 0; i < VectorOfList.size(); i++)
            {
                PrintList(VectorOfList[i]);
                std::cout << std::endl;
            }
            break;
        case 3:
            for (int i = 0; i < VectorOfList.size(); i++)
            {
                ResultList = MergeLists(VectorOfList[i], ResultList);
            }
            std::cout << "Common list" << std::endl;
            PrintList(ResultList);
            ClearList(ResultList);
            break;
        case 4:
            exit = false;
            break;
        }
    }
}

list *CreateNewNode(int NewField)
{
    list *NewNode = new list;
    NewNode->field = NewField;
    NewNode->next = NULL;
    return NewNode;
}

void AddFirst(list *&First, list *NewNode)
{
    NewNode->next = First;
    First = NewNode;
}

void AddAfter(list *CurrNode, list *NewNode)
{
    NewNode->next = CurrNode->next;
    CurrNode->next = NewNode;
}

void AddLast(list *&First, list *NewNode)
{
    list *curr;
    curr = First;
    if (First == NULL)
    {
        AddFirst(First, NewNode);
        return;
    }
    while (curr->next)
        curr = curr->next;
    AddAfter(curr, NewNode);
}

list *FindPlaceToInsert(list *First, int NewField)
{
    list *Curr, *Prev;
    Prev = NULL;
    Curr = First;
    while (Curr && (NewField > Curr->field))
    {
        Prev = Curr;
        Curr = Curr->next;
    }
    return Prev;
}

void AddBefore(list *&First, list *Prev, list *NewNode)
{
    (Prev == NULL) ? AddFirst(First, NewNode) : AddAfter(Prev, NewNode);
}

void PrintList(list *First)
{
    list *curr = First;
    while (curr != NULL)
    {
        std::cout << curr->field << " ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

void PrintVector(vector<list *> VectorOfList)
{
    std::cout << "Print vector" << std::endl;
    for (int i = 0; i < VectorOfList.size(); i++)
        std::cout << VectorOfList[i]->field << std::endl;
}

vector<list *> ReadFileInList()
{
    setlocale(LC_ALL, "ru");
    string pathIn = "input.txt";
    vector<list *> VectorOfList;

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
            ReadLineFromFile(FileIn, VectorOfList);
        }
    }
    FileIn.close();
    return VectorOfList;
}

void ReadLineFromFile(ifstream &FileIn, vector<list *> &VectorOfList)
{
    string s;
    getline(FileIn, s);
    std::cout << s << " ";
    std::cout << std::endl;
    std::stringstream ss;
    ss.str(s);

    list *First = NULL;
    list *NewNode, *CurrNode, *Prev;
    string s2;
    int Number;
    while (ss >> s2)
    {
        Number = stoi(s2);
        NewNode = CreateNewNode(Number);
        Prev = FindPlaceToInsert(First, Number);
        AddBefore(First, Prev, NewNode);
    }
    VectorOfList.push_back(First);
}

list *MergeLists(list *First1, list *First2)
{
    list *resultFirst = new list;
    list *curr1 = First1;
    list *curr2 = First2;
    list *temp = new list;
    if (First2 == NULL)
    {
        resultFirst = CopyList(First1);
    }
    else
    {
        if (curr1->field < curr2->field)
        {
            temp->next = NULL;
            temp->field = curr1->field;
            resultFirst = temp;
            curr1 = curr1->next;
        }
        else
        {
            temp->next = NULL;
            temp->field = curr2->field;
            resultFirst = temp;
            curr2 = curr2->next;
        }
        while ((curr1 != NULL) && (curr2 != NULL))
        {
            if (curr1->field < curr2->field)
            {
                temp->next = new list;
                temp->next->field = curr1->field;
                temp->next->next = NULL;
                temp = temp->next;
                curr1 = curr1->next;
            }
            else
            {
                temp->next = new list;
                temp->next->field = curr2->field;
                temp->next->next = NULL;
                temp = temp->next;
                curr2 = curr2->next;
            }
        }
        while (curr1) //дописываем остатки из списка 1
        {
            temp->next = new list;
            temp->next->field = curr1->field;
            temp->next->next = NULL;
            temp = temp->next;
            curr1 = curr1->next;
        }
        while (curr2) //дописываем остатки из списка 2
        {
            temp->next = new list;
            temp->next->field = curr2->field;
            temp->next->next = NULL;
            temp = temp->next;
            curr2 = curr2->next;
        }
    }
    return resultFirst;
}

list *CopyList(list *First)
{
    list *curr = First;
    list *temp = new list;
    temp->next = NULL;
    temp->field = curr->field;
    list *result = temp;
    curr = curr->next;
    while (curr != NULL)
    {
        temp->next = new list;
        temp->next->field = curr->field;
        temp->next->next = NULL;
        temp = temp->next;
        curr = curr->next;
    }
    return result;
}

void ClearList(list *&First)
{
    if (First == 0)
        return;
    list *curr = First;
    list *temp;
    while (curr != NULL)
    {
        temp = curr;
        curr = curr->next;
        delete temp;
    }
    First = 0;
}