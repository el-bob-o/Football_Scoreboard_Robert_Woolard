//Dr. Tyson McMillan a Soccer Scoreboard using
//Object Oriented Programming in C++
//Updates 4-21-2021 - Dr_T
//Dr_T teaching Object Oriented Scoreboard in C++

#include "board.h"




int main()
{
#pragma omp parallel
#pragma omp sections
    {
//Taking the timer out for now
//#pragma omp section
//        timer();
#pragma omp section
        menu();
    }


  return 0;
}
