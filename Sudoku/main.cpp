#include <algorithm>
#include <iostream>
#include <vector>
#include "other.h"
#include <map>
#include <thread>
#include <mutex>
#include <set>
#include <bitset>
#include <stack>
using namespace std;


const unsigned sodokuSize = 9;

//unsigned sodoku [9][9] = {
//    {3, 0, 6, 5, 0, 8, 4, 0, 0},
//    {5, 2, 0, 0, 0, 0, 0, 0, 0},
//    {0, 8, 7, 0, 0, 0, 0, 3, 1},
//    {0, 0, 3, 0, 1, 0, 0, 8, 0},
//    {9, 0, 0, 8, 6, 3, 0, 0, 5},
//    {0, 5, 0, 0, 9, 0, 6, 0, 0},
//    {1, 3, 0, 0, 0, 0, 2, 5, 0},
//    {0, 0, 0, 0, 0, 0, 0, 7, 4},
//    {0, 0, 5, 2, 0, 6, 3, 0, 0}
//};

//unsigned sodoku [9][9] = {
//    {9, 8, 0, 0, 5, 0, 0, 6, 3},
//    {0, 3, 0, 0, 0, 0, 0, 4, 0},
//    {4, 0, 0, 9, 0, 3, 0, 0, 1},
//    {0, 7, 0, 1, 0, 5, 0, 3, 0},
//    {0, 0, 6, 0, 0, 0, 5, 0, 0},
//    {0, 9, 0, 6, 0, 2, 0, 8, 0},
//    {6, 0, 0, 8, 0, 9, 0, 0, 2},
//    {0, 4, 0, 0, 0, 0, 0, 9, 0},
//    {3, 2, 0, 0, 7, 0, 0, 1, 8}
//};

//unsigned sodoku [9][9] = {
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0},
//    {0, 0, 0, 0, 0, 0, 0, 0, 0}
//};

unsigned sodoku [9][9] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 5, 0, 0, 9, 0, 0, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 2, 0, 0, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 4, 0, 0, 7, 0, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};


struct SodokuItem
{
    std::pair<unsigned, unsigned> _coord{0,0};
    unsigned _value{0};
};

bool validateSmallSodoku(const SodokuItem& item, int SubVertical, int subHorizontal)
{
    int startHorizontal =subHorizontal*3;
    int startVertical =SubVertical*3;

    for(int vertical = startVertical; vertical < startVertical +3; ++vertical)
    {
        for(int horizontal = startHorizontal; horizontal< startHorizontal + 3; ++horizontal)
        {
            //check if we have same number in sodoku as the one we try to add
            if(item._coord.first != vertical || item._coord.second != horizontal)
            {
                if(sodoku[vertical][horizontal] ==  item._value)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool validateHorizontal(const SodokuItem& item)
{
    for(int horizontal = 0; horizontal < sodokuSize; ++horizontal)
    {
        //make sure item is not the same as the one we compare
        if(horizontal != item._coord.second)
        {
            //check if we have same number on horizontal
            if(sodoku[item._coord.first][horizontal] == item._value)
            {
                return false;
            }
        }
    }

    return true;
}

bool validateVertical(const SodokuItem& item)
{
    for(int vertical = 0; vertical < sodokuSize; ++vertical)
    {
        //make sure item is not the same as the one we compare
        if(vertical != item._coord.first)
        {
            if(sodoku[vertical][item._coord.second] == item._value)
            {
                return false;
            }
        }
    }

    return true;
}

bool validateSodoku(const SodokuItem& item)
{
    //get item small sodoku location

    int subVertical = item._coord.first / 3;
    int subHorizontal = item._coord.second / 3;

    if(!validateSmallSodoku(item, subVertical, subHorizontal))
    {
        return false;
    }

    if(!validateHorizontal(item))
    {
        return false;
    }

    if(!validateVertical(item))
    {
        return false;
    }


    return true;
}

void PrintSodoku()
{
    for(unsigned i = 0; i< sodokuSize; ++i)
    {
        for(unsigned j = 0; j< sodokuSize; ++j)
        {
            cout<<sodoku[i][j]<<", ";
        }
        cout<<"\n";
    }
}


void TrySodoku(vector<SodokuItem>& items)
{
//    //we found solution print it
//    if(items.size() == 0)
//    {
//        cout<<" solution found \n\n";
//        PrintSodoku();
//        return;
//    }

    //get first item
    SodokuItem item = items[0];
    items.erase(items.begin());

    //try number
    for(unsigned numberToTry = 1; numberToTry<=9; ++numberToTry)
    {
        item._value = numberToTry;
        sodoku[item._coord.first][item._coord.second] = numberToTry;

        //if valid position go on
        if(validateSodoku(item))
        {
            //if guess for last item is valid we have solution
            if(items.size() == 0)
            {
                cout<<" solution found \n\n";
                PrintSodoku();
               //return;
            }
            else
            {
                //go deeper till exaust all empty items
                TrySodoku(items);
            }
        }
        else
        {
            //not valid try next one
            continue;
        }
    }

    //backtrack
    sodoku[item._coord.first][item._coord.second] = 0;
    item._value = 0;
    items.insert(items.begin(), item);
}


int main()
{
    //todo make sure we can input different sodokus

    vector<SodokuItem> positonsToFill;

    for(int vertical =0; vertical<sodokuSize; ++vertical)
    {
        for(int horizontal=0; horizontal< sodokuSize; ++horizontal)
        {
            //find positions that are not yet filled
            if(sodoku[vertical][horizontal] == 0)
            {
                //keep position as empty item
                //SodokuItem item{{i,j},0};
                //store items in vector
                positonsToFill.push_back({{vertical,horizontal},0});
            }
        }
    }


    TrySodoku(positonsToFill);

    return 0;
}
