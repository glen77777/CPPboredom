#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <iterator>
#include <numeric>

const auto processor_count = std::thread::hardware_concurrency(); // find number of cpu threads
std::vector<int> threadcount{};
// create a vector group that will house a value for each cpu thread
//ikik this isn't how multithreading is supposed to work but whatever man

void mapper()
{
    int iter = 1;
    while (processor_count > threadcount.size()) {
        threadcount.push_back(iter);
        ++iter;
    }
}
// create a list of values and append them with pushback to the vector group?? i mean I think this is how vectors work
//I just view them as more functional arrays man, I just work here bro idk, with a 4 core processor with hyperthreading
//this should end up being like vector <int> threadcount = { "1", "2", "3", "4", "5", "6", "7", "8"}
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
// So the above portion here is kind of just a prime finder, so in the first part y%2==0 basically checks if it's even,
//the only even prime is 2 so... the next part checks the current value of the countup numberand tracks the divisor count...
//i.e. if we are checking 9 we have to check 1/9,3/9,5/9,7/9,9/9 and for a number to be prime it has to only be evenly divisible by itself and one
// so 1/9 will add a count to the divisor count, i.e. d, 9/9 will add a count to d, and if it ends up being a divisor count of two when we
//check the sum of all the thread's "d's" then we know it is prime... in this case of 9 however 3 will also add a count to d and d will
//exceed 2 so no dice
void primes(int x)  //the meat of it, kinda
{
    unsigned int threadsize = threadcount.size(); //checks the size of the vector group made to house a slot for each cpu thread,
    //yes you can just take highest value, whatever man
    int g = threadsize;
    //sets another value the same, which may seem weird but remember you have a target numer to count up to, i.e. entered at cmdline, then
    //you have the number you are currently checking, then you have the current number from 1 to the currently checked number that you have to
    //divide currently checked number by to see if they are cleanly divisible, add a count to the divisor "d" count if so
    while (x > g)
    {//num from main gets passed here as x
        if (g % 2 == 0)
        {
            ++g;
        }
        //again, if it is even then skip it
        std::vector<int> threaddiv{};
        for (int i = 0; i < threadcount.size(); i++)
        {
            threaddiv.push_back(threadcount[i]);
        }
// I think I'm just making another vector group in this part, If I remember correctly I have one counter for the number to calculate up to,
//one counter for the current number, on vector that holds the floor and ceiling of the part of the number set delegated to each respective thread,
//a std ref for the divisor count, and a vector for the threads themselves

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
        }// so here I am accounting for overhang of sorts, my method of "threading" this program is to try to break up the set
        //of number I have to divide the current number by into as many close to equal size sets as there are cpu threads available and assign
        //a thread for each set, so what I'm doing here is checking if it is a cleanly divisible number of total division
        //candidates or not, it's usually not and subracting one from the top with that while newthready is not cleanly divisible by
        //threadcount piece up there in the while statement until I have a cleanly divisible set and then appending the overhang to the top thread
        else
        {
            for (int i = 0; i < threaddiv.size(); i++)
            {
                int tempspace = threadcount[i];
                threaddiv[i] = (newthready / threadsize) * tempspace;
            }
            threaddiv.back() = y;
        }
        //this part above is for if it is cleanly divisible, which I don't think is possible ever but that's more thought so nah, we're chillin'

        std::vector<int> divisorcount{};
        int newdivcount = 0;
        while (processor_count > divisorcount.size()) {
            divisorcount.push_back(newdivcount);
        }
        //this is where I make a vector to hold the divisor counts
        std::vector<std::thread> actualThreads;
        threaddiv.insert(threaddiv.begin(), 0);
        for (int i = 0; i < threadcount.size(); i++) {
            std::thread th(looper, g, threaddiv[i + 1], threaddiv[i], std::ref(divisorcount[i]));
            actualThreads.push_back(std::move(th));
        }
        //this is the vector that holds the actual threads, the thread call include the function to execute and the values to pass to it
        //i.e. looper is the function, g is the current number being determined if prime or not, threaddiv i is the lower number of the chunk
        //of the number set this thread is responsible for, so the i+1 is the higher number of this chunk of numbers, and div count is a standard
        // ref so it can be sent into the thread, used as the counter within the thread, and pull back out when the thread finishes

        for (std::thread& th : actualThreads)
        {
            // If thread Object is Joinable then Join that thread. i.e., if the piece of the set that was split for that cpu thread is
            //done, mark it done
            if (th.joinable())
                th.join();
        }


        divcount = std::accumulate(divisorcount.begin(), divisorcount.end(), 0);
        if (divcount == 2)
        {
            std::cout << g << " is prime\n";
        }
        else { std::cout << g << " is not prime\n"; }
        //this section is where you add up all the divisor counters that get pulled back out of the finished threads and add them up to determine
        //if 2 "prime" or not, i.e. div by 1 and itself only, and below you reset some of the values for the next number until the
        //number you reach is equal to x or num, which is the number you pas in from the cli
        divisorcount.clear();
        threaddiv.clear();
        actualThreads.clear();

        ++g;
        //this is the part where after all the threads are done for that number, determine if it is prime by adding up "d" values for the group
        //of sets thare are made by splitting up the one large set of numbers that you have to divide by
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

