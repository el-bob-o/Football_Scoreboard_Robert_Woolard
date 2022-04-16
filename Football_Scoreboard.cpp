/* Using the structure from: Dr. Tyson McMillan a Soccer Scoreboard
Robert Woolard
COSC 1437
04/16/2022
Dr. Tyson McMillan*/

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
