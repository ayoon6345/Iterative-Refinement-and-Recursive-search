#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <numeric>
using namespace std;
struct candy
{
    double ratio;
    double weight;
    double tasty;
};
struct CandyState
{
    bool inbag;
    int candy;
};
struct bag
{
    vector<int> bag;
    int totalYumminess;
    int totalWeight;
};
vector<candy> candies(16);
vector<CandyState> Candy(16);
bag bags[3];

int ExhaustiveSol = 0;
int RefinementSol = 0;
int bagweight;

bool compare(candy a, candy b)
{
    if (a.ratio > b.ratio)
        return 1;
    else
        return 0;
}

void insert(int a, int b)
{
    bags[a].bag.push_back(b);
    bags[a].totalYumminess+=candies[b].tasty;
    bags[a].totalWeight+=candies[b].weight;
    for (int i = 0; i < 16; i++)
    {
        if (Candy[i].candy == b)
        {
            Candy[i].inbag = true;
            break;
        }
    }
}
void remove(int a, int b)
{
    bags[a].totalYumminess -= candies[b].tasty;
    bags[a].totalWeight-=candies[b].weight;
    auto c = find(bags[a].bag.begin(), bags[a].bag.end(), b);
    int index = c - bags[a].bag.begin();
    bags[a].bag.erase(bags[a].bag.begin() + index);
    for (int i = 0; i < 16; i++)
    {
        if (Candy[i].candy == b)
        {
            Candy[i].inbag = false;
            break;
        }
    }
}
void FasterRemove(int a, int b)
{
    bags[a].totalYumminess -= candies[b].tasty;
    bags[a].totalWeight-=candies[b].weight;
    bags[a].bag.pop_back();
}
void move(int bag1, int bag2, int b)
{
    bags[bag2].bag.push_back(b);
    bags[bag2].totalWeight+=candies[b].weight;
    for (int i = 0; i < 16; i++)
    {
        if (Candy[i].candy == b)
        {
            Candy[i].inbag = true;
            break;
        }
    }
    bags[bag1].totalWeight-=candies[b].weight;
    auto c = find(bags[bag1].bag.begin(), bags[bag1].bag.end(), b);
    int index = c - bags[bag1].bag.begin();
    bags[bag1].bag.erase(bags[bag1].bag.begin() + index);
   
}
void dump()
{
    for (int i = 0; i < 3; i++)
    {
        bags[i].bag.clear();
        
        bags[i].totalYumminess = 0;
        bags[i].totalWeight = 0;

    }
}
void print()
{
    cout << "bag 0: " << bags[0].totalWeight << "\n";
    for (int i = 0; i < bags[0].bag.size(); i++)
    {
        cout << bags[0].bag[i] << " ";
    }
    cout << "\n";
    cout << "bag 1: " << bags[1].totalWeight << "\n";
    for (int i = 0; i < bags[1].bag.size(); i++)
    {
        cout << bags[1].bag[i] << " ";
    }
    cout << "\n";
    cout << "bag 2: " << bags[2].totalWeight << "\n";
    for (int i = 0; i < bags[2].bag.size(); i++)
    {
        cout << bags[2].bag[i] << " ";
    }
    cout << "\n";
    cout << "\n";
}
void fill_bags()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (((bags[i].totalWeight + candies[Candy[j].candy].weight) <= 2000) && (Candy[j].inbag == false))
            {
                insert(i, Candy[j].candy);
            }
        }
    }
}

int solve(int a)
{
    int bagCount;
    int sum = 0;
    if (a == 16)
    {
        if ((bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess) > ExhaustiveSol)
        {
            ExhaustiveSol =  (bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess);
        }
        return 0;
    }

    for (int i = a; i < 16; i++)
    {
        sum += candies[i].tasty;
    }

    if (((bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess) + sum) < ExhaustiveSol)
    {
        return 0;
    }
    if (a == 0)
    {
        bagCount = 1;
    }
    else
    {
        bagCount = 3;
    }
    for (int i = 0; i < bagCount; i++)
    {
        if ((bags[i].totalWeight + candies[Candy[a].candy].weight) < 2000)
        {
            bags[i].bag.push_back(Candy[a].candy);
            bags[i].totalYumminess+=candies[Candy[a].candy].tasty;
            bags[i].totalWeight+=candies[a].weight;
            solve(a + 1);
            FasterRemove(i, Candy[a].candy);           
        }
    }
    solve(a + 1);
    return 0;
}

int main(void)
{   
    double grams, taste = 0;
    int i = 0;
    ifstream candyfile("candy.txt");
    while (candyfile >> grams >> taste)
    {
        candies[i].ratio = taste / grams;
        candies[i].weight = grams;
        candies[i].tasty = taste;
        i++;
    }
    candyfile.close();
    for (int i = 0; i < 16; i++)
    {
        Candy[i].candy = i;
    }
    sort(candies.begin(), candies.end(), compare);
    fill_bags();
    cout << "Greedy: " << (bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess) << "\n";
    dump();

    //--------------------------------------------------------------------------------------------------------------------------

    for (int k = 0; k < 1000; k++)
    {
        //Shuffle and fill all 3 bags under weight limit
        for (int i = 0; i < 16; i++)
        {
            Candy[i].candy = i;
            Candy[i].inbag = false;
        }
        random_shuffle(Candy.begin(), Candy.end());
        fill_bags();
        int randbag = rand() % 3;
        int randbag2 = (randbag + 1) % 3;
        int randbag3 = (randbag + 2) % 3;

        //find the lightest bag of the 2 bags your moving candy into
        bool lowestWeight;
        (bags[randbag2].totalWeight) < (bags[randbag3].totalWeight) ? lowestWeight = true : lowestWeight = false;
        //Pick random candy your moving from bag
        int index = rand() % (bags[randbag].bag.size());

        if (lowestWeight == true)
        {
            move(randbag, randbag2, bags[randbag].bag[index]);
            //If weight of the other bag is overfull, remove the least tastiest candy till it's underfull
            while (bags[randbag2].totalWeight > 2000)
            {
                int value = 3000;
                int index = 0;
                //find least valuable candy in the bag and remove it
                for (int i = 0; i < bags[randbag2].bag.size(); i++)
                {
                    if (value > candies[bags[randbag2].bag[i]].ratio)
                    {
                        value = candies[bags[randbag2].bag[i]].ratio;
                        index = bags[randbag2].bag[i];
                    }
                }
                remove(randbag2, index);
            }
            fill_bags();
        }
        else
        {
            move(randbag, randbag3, (bags[randbag].bag[index]));
            //If weight of the other bag is overfull, remove the least tastiest candy till it's underfull
            while (bags[randbag3].totalWeight > 2000)
            {
                int value2 = 3000;
                int index = 0;
                //find least valuable candy in the bag and remove it
                for (int i = 0; i < bags[randbag3].bag.size(); i++)
                {
                    if (value2 > candies[bags[randbag3].bag[i]].ratio)
                    {
                        value2 = candies[bags[randbag3].bag[i]].ratio;
                        index = bags[randbag3].bag[i];
                    }
                }
                remove(randbag3, index);
            }
            fill_bags();
        }
        //If total tastiness of all 3 bags is better than best solution, make that the best solution
        if ((bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess) >= RefinementSol)
        {
            RefinementSol = bags[0].totalYumminess + bags[1].totalYumminess + bags[2].totalYumminess;
        }
        dump();
    }
    cout << "Refinement: " << RefinementSol << "\n";

    //--------------------------------------------------------------------------------------------------------------------------

    for (int i = 0; i < 16; i++)
    {
        Candy[i].candy = i;
    }
    solve(0);
    cout << "Exhaustive: " << ExhaustiveSol << "\n";
    return 0;
}
