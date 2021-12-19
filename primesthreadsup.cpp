#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <iterator>
#include<numeric>

const auto processor_count = std::thread::hardware_concurrency();
std::vector<int> threadcount{};

void mapper()
{
    int iter = 1;
    while (processor_count > threadcount.size()) {
        threadcount.push_back(iter);
        ++iter;
    }
}
void looper(int x, int y, int z, int& d)
{

    while (y > z)
    {
        if (y % 2 == 0)
        {
            --y;
        }
        else if (x % y == 0)
        {
            ++d;
            --y;
        }
        else {
            --y;
        }
    }
}

void primes(int x)
{
    unsigned int threadsize = threadcount.size();
    int g = threadsize;
    while (x > g)
    {
        if (g % 2 == 0)
        {
            ++g;
        }
        
        std::vector<int> threaddiv{};
        for (int i = 0; i < threadcount.size(); i++)
        {
            threaddiv.push_back(threadcount[i]);
        }
        

        int y{ g };
        int divcount{};
        int newthready = y;
        
        if (newthready % threadsize != 0)
        {
            while (newthready % threadsize != 0)
            {
                --newthready;
            }
            for(int i = 0; i < threaddiv.size(); i++)
            {
                int tempspace = threadcount[i];
                threaddiv[i] = (newthready / threadsize) * tempspace;
            }
            threaddiv.back() = y;
        }
        else
        {
            for (int i = 0; i < threaddiv.size(); i++)
            {
                int tempspace = threadcount[i];
                threaddiv[i] = (newthready / threadsize) * tempspace;
            }
            threaddiv.back() = y;
        }
        
       
        std::vector<int> divisorcount{};
        int newdivcount = 0;
        while (processor_count > divisorcount.size()) {
            divisorcount.push_back(newdivcount);            
        }
        
        std::vector<std::thread> actualThreads;
        threaddiv.insert(threaddiv.begin(), 0);
        for (int i = 0; i < threadcount.size(); i++) {
            std::thread th(looper, g, threaddiv[i + 1], threaddiv[i], std::ref(divisorcount[i]));
            actualThreads.push_back(std::move(th));
        }

        
        for (std::thread& th : actualThreads)
        {
            // If thread Object is Joinable then Join that thread.
            if (th.joinable())
                th.join();
        }

               
        divcount = std::accumulate(divisorcount.begin(), divisorcount.end(), 0);
        if (divcount == 2)
        {
            std::cout << g << " is prime\n";
        }
        else { std::cout << g << " is not prime\n"; }

        divisorcount.clear();
        threaddiv.clear();
        actualThreads.clear();

        ++g;
    }
}
int main()
{
    int num{};
    std::cout << "enter a number" << std::endl;
    std::cin >> num;
    mapper();
    primes(num);
    std::cout << threadcount.size();
    return 0;
}
