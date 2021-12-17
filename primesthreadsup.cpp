#include <iostream>
#include <sstream>
#include <thread>

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
    int g{ 4 };
    while (x > g)
    {
        if (g % 2 == 0)
        {
            ++g;
        }
        int y{ g };
        int threadyone{ y };
        int threadytwo{};
        int threadythree{};
        int threadyfour{};
        int divcount{};
        if (threadyone % 4 != 0)
        {
            while (threadyone % 4 != 0)
            {
                --threadyone;
            }
            threadytwo = (threadyone / 4) * 3;
            threadythree = (threadyone / 4) * 2;
            threadyfour = (threadyone / 4);
        }
        else
        {
            threadytwo = (y / 4) * 3;
            threadythree = (y / 4) * 2;
            threadyfour = (y / 4);
        }
        int d1{};
        int d2{};
        int d3{};
        int d4{};
        std::thread t1(looper, g, y, threadytwo, std::ref(d1));
        std::thread t2(looper, g, threadytwo, threadythree, std::ref(d2));
        std::thread t3(looper, g, threadythree, threadyfour, std::ref(d3));
        std::thread t4(looper, g, threadyfour, 0, std::ref(d4));
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        divcount = d1 + d2 + d3 + d4;
        if (divcount == 2)
        {
            std::cout << g << " is prime\n";
        }
        else { std::cout << g << " is not prime\n"; }
        ++g;
    }
}
int main(int argc, char* argv[])
{
    std::stringstream convert{ argv[1] };
    int num{};
    if (!(convert >> num))
        num = 0;
    primes(num);
    return 0;
}
