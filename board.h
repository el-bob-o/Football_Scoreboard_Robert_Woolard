#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <iostream>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <cmath>
#include <thread>
#include "Input_Validation_Extended.h"
using namespace std;

enum Position{ LEFT, CENTER, RIGHT };

//The following class is to help with positioning of text
//taken from http://www.cplusplus.com/forum/general/256212/
void print( Position pos, string s, int linelength )
{
   int spaces = 0;
   switch( pos )
   {
      case CENTER: spaces = ( linelength - s.size() ) / 2; break;
      case RIGHT : spaces =   linelength - s.size()      ; break;
   }
   if ( spaces > 0 ) cout << string( spaces, ' ' );
   cout << s;
}

/*I cannot figure out how to have the timer update on the screen while also taking input for the menu.
Well, it can take input in one thread but whatever is being typed is overwritten by the clock updating.
The input for the menu thread is still valid, you just can't see what you are typing since it is overwritten.
I have it running in it's own thread but my bigger issue is placement on the screen.
By using the \r command it is just writing to the terminal.
I'm looking into mutex but I don't have time to figure it out.*/
void timer() //Attempting to make a timer, having trouble with actively displaying the countdown
{
    int sec = 00;
    int min = 15;

    while (true)
    {
#pragma omp critical
        //attempting to rewrite the timer on the current line, it works but makes menu input messy
        cout << setw(2) << setfill('0') << min << ":" << setw(2) << setfill('0') << sec-- << "\r";
        cout << "Enter selection:";

        this_thread::sleep_for(chrono::duration<double>(1.0));

        if (sec < 0) {
            min--;
            sec = 59;
        }

    }

}


class Team
{
  private:
    string teamName;
    string coachName;
    string cityName;
    int score;
    bool homeStatus;
    int timeoutCount;
    string name;
    public:
      Team() //default constructor
      {
        score = 0;
        homeStatus = false; //visitor = false, home = true
        teamName = "DefaultTeamName";
        timeoutCount = 3;
        coachName = "DefaultCoachName";
        cityName = "DefaultCityName";
      }
      void setScore(int s) { score = s; }
      void setHomeStatus(bool hs) { homeStatus = hs; }
      void setTeamName(string n) { teamName = n; }
      void setTimeoutCount(int to) { timeoutCount = to; }
      void setCoachName(string sCN) { coachName = sCN; }
      void setCityName(string cn) { cityName = cn; }
      int getScore() const { return score; }
      bool getHomeStatus() const { return homeStatus; }
      string getTeamName() const { return teamName; }
      int getTimeoutCount() const { return timeoutCount; }
      string getCoachName() const { return coachName; }
      string getCityName() const { return cityName; }
};

class Scoreboard
{
  private:
    int quarter;
    Team home; //object that is a member of another object
    Team visitor;
    Team hold; //for holding team data
    Team timeout;
  public:
    Scoreboard()
    {
      quarter = 1;
    }
    void setQuarter(int q) { quarter = q; }
    void setHome(Team hSet) { home = hSet; }
    void setVisitor(Team vSet) { visitor = vSet; }
    void setTimeoutCount(Team toSet) { timeout = toSet; }
    int getQuarter() const { return quarter; }
    Team getHome() const { return home; }
    Team getVisitor() const { return visitor; }
    Team getTimeoutCount() const { return timeout; }
    const int LINELENGTH = 20;
    void showScoreboard()
    {
       int teamSpace = ((61 - home.getTeamName().size())/2); //variable to compute number of spaces between team names
       string tSpace(teamSpace, ' '); //variable to print spaces between team names
       int citySpace = ((61 - home.getCityName().size())/2); //variable to compute number of spaces between city names
       string cSpace(citySpace, ' '); //variable to print spaces between city names
       int coachSpace = ((61 - home.getCoachName().size())/2); //variable to compute number of spaces between coach names
       string coSpace(coachSpace, ' '); //variable to print spaces between coach names
       string header( 60, '*');
       string color = "";
       string reset = "\x1b[0m";
       color = "\x1b[32;4m"; //green
       string score = "\x1b[36;1m"; //score color

       if(home.getHomeStatus() == true)
       {
         int teamSpace = ((61 - home.getTeamName().size())/2); //variable to compute number of spaces between team names
         string tSpace(teamSpace, ' '); //variable to print spaces between team names
         int citySpace = ((61 - home.getCityName().size())/2); //variable to compute number of spaces between city names
         string cSpace(citySpace, ' '); //variable to print spaces between city names
         int coachSpace = ((61 - home.getCoachName().size())/2); //variable to compute number of spaces between coach names
         string coSpace(coachSpace, ' '); //variable to print spaces between coach names
         cout << color << header << reset << endl;
         cout << "\n" << "\x1b[34;1m";
         print(CENTER, "HOME", LINELENGTH);
         cout << "\t\t\t\t";
         print (CENTER, "VISITOR", LINELENGTH);
         cout << "\n" << "\x1b[32;1m";
         print( CENTER, home.getTeamName(), LINELENGTH );
         cout << tSpace;
         print( CENTER, visitor.getTeamName(), LINELENGTH );
         cout << "\n\t " << "\x1b[36;1m" << home.getScore() << "\t\t\t\t\t " << visitor.getScore() << reset << "\n";
         cout << "\x1b[31;1m" << "\t\t\t QTR: " << quarter << "\x1b[35;1m" << "\n";
         print(CENTER, "Timeouts", LINELENGTH);
         cout << "\t\t\t\t";
         print(CENTER, "Timeouts", LINELENGTH);
         cout << "\n\t " << home.getTimeoutCount() << "\t\t\t\t\t " << visitor.getTimeoutCount() << "\x1b[33;1m" << "\n\n";
         print( CENTER, home.getCityName(), LINELENGTH );
         cout << cSpace;
         print( CENTER, visitor.getCityName(), LINELENGTH );
         cout << "\n";
         print( CENTER, home.getCoachName(), LINELENGTH );
         cout << coSpace;
         print( CENTER, visitor.getCoachName(), LINELENGTH );
         cout << "\n" << color << header << reset << endl;
         for(int i = 0; i < 60; i++) { cout << "="; } cout << endl;

         //proces to show the home team status
         cout << "Home> \t";
         cout << "Team 1: " << home.getTeamName() << "*";
       }
       else if(home.getHomeStatus() == false) //swapping teams (I'm sure there is a better way)
       {
         int teamSpace = ((61 - visitor.getTeamName().size())/2); //variable to compute number of spaces between team names
         string tSpace(teamSpace, ' '); //variable to print spaces between team names
         int citySpace = ((61 - visitor.getCityName().size())/2); //variable to compute number of spaces between city names
         string cSpace(citySpace, ' '); //variable to print spaces between city names
         int coachSpace = ((61 - visitor.getCoachName().size())/2); //variable to compute number of spaces between coach names
         string coSpace(coachSpace, ' '); //variable to print spaces between coach names
         cout << color << header << reset << endl;
         cout << "\n" << "\x1b[34;1m";
         print(CENTER, "HOME", LINELENGTH);
         cout << "\t\t\t\t";
         print (CENTER, "VISITOR", LINELENGTH);
         cout << "\n" << "\x1b[32;1m";
         print( CENTER, visitor.getTeamName(), LINELENGTH );
         cout << tSpace;
         print( CENTER, home.getTeamName(), LINELENGTH );
         cout << "\n\t " << "\x1b[36;1m" << visitor.getScore() << "\t\t\t\t\t " << home.getScore() << reset << "\n";
         cout << "\x1b[31;1m" << "\t\t\t QTR: " << quarter << "\x1b[35;1m" << "\n";
         print(CENTER, "Timeouts", LINELENGTH);
         cout << "\t\t\t\t";
         print(CENTER, "Timeouts", LINELENGTH);
         cout << "\n\t " << visitor.getTimeoutCount() << "\t\t\t\t\t " << home.getTimeoutCount() << "\x1b[33;1m" << "\n\n";
         print( CENTER, visitor.getCityName(), LINELENGTH );
         cout << cSpace;
         print( CENTER, home.getCityName(), LINELENGTH );
         cout << "\n";
         print( CENTER, visitor.getCoachName(), LINELENGTH );
         cout << coSpace;
         print( CENTER, home.getCoachName(), LINELENGTH );
         cout << "\n" << color << header << reset << endl;
         for(int i = 0; i < 60; i++) { cout << "="; } cout << endl;

         //proces to show the home team status
         cout << "Home> \t";
         cout << "Team 2: " << visitor.getTeamName() << "*";
       }
       else
       {
         cout << "Error: ";
       }

       cout  << endl;
    }
};

void menu()
{
    Scoreboard s;
  Team tOne;
  Team tTwo;
  string newName = "";
  string userChoice = "";
  string newCoachName = "";
  string newCityName = "";
  int homeTeamQuestion = 0;
  int newScore = 0;
  int newQuarter = 1;
  int newTimeout = 0;

  //Set the home team
  tOne.setHomeStatus(true); //tOne is the Home Team now

  //add some initial data to s
  s.setHome(tOne);
  s.setVisitor(tTwo);


  //Loop this to make it an interactive menu

  do
  {
      system("CLS"); //clear the screen of previous content (windows)
      s.showScoreboard(); //show the current scoreboard data
      //menu choices
      cout << "1 = Update Team #1 Name" << endl;
      cout << "2 = Update Team #2 Name" << endl;
      cout << "3 = Update Team #1 Score" << endl;
      cout << "4 = Update Team #2 Score" << endl;
      cout << "5 = Update Quarter" << endl;
      cout << "6 = Update Team #1 City" << endl;
      cout << "7 = Update Team #2 City" << endl;
      cout << "8 = Update Team #1 Coach" << endl;
      cout << "9 = Update Team #2 Coach" << endl;
      cout << "10 = Update Team #1 Timeouts" << endl;
      cout << "11 = Update Team #2 Timeouts" << endl;
      cout << "C = Update Home Status" << endl;
      cout << "E = Exit" << endl;
      cout << ">";
      cin >> userChoice;

      if(userChoice == "1")
      {
        cout << "****Update Team #1 Score module*** " << endl;
        cout << "\nPlease enter a new name for Team #1 (" << tOne.getTeamName() << "): ";
        getline(cin >> ws, newName);
        //change Team #1's default name
        tOne.setTeamName(newName); //set tOne's data to the new desired name
      }
      else if(userChoice == "2")
      {
        cout << "****Update Team #2 Score module*** " << endl;
        cout << "\nPlease enter a new name for Team #2: (" << tTwo.getTeamName() << "): ";
        getline(cin >> ws, newName);
        //change Team #2's default name
        tTwo.setTeamName(newName); //set tTwo's data to the new desired name
      }
      else if(userChoice == "3")
      {
        cout << "\nUpdate Team #1 Score Module****" << endl;
        cout << "\nPlease enter a new score for Team #1 (" << tOne.getTeamName() << ", current score: " << tOne.getScore() << ")\n";
        newScore = validateInt(newScore);
        tOne.setScore(newScore);  //set the new score for tOne
      }
      else if(userChoice == "4")
      {
        cout << "\nUpdate Team #2 Score Module****" << endl;
        cout << "\nPlease enter a new score for Team #2 (" << tTwo.getTeamName() << ", current score: " << tTwo.getScore() << ")\n";
        newScore = validateInt(newScore);
        tTwo.setScore(newScore);  //set the new score for tOne
      }
      else if(userChoice == "5")
      {
        cout << "\nUpdate Quarter Module****" << endl;
        cout << "\nPlease enter a new number for the quarter:\n";
        while (true) { //loop to validate input for changing the quarter
            newQuarter = validateInt(newQuarter);
                if (newQuarter > 0 && newQuarter < 5) {
                    s.setQuarter(newQuarter);  //set the new quarter
                    break;
                }
                else {
                    cout << "Please enter a number between 1 and 4.\n";
                    continue;
                }
        }
      }
      else if(userChoice == "6")
      {
          cout << "\nUpdate City Module****" << endl;
          cout << "\nPlease enter a new name for Team #1's City (" << tOne.getTeamName() << ", current city: " << tOne.getCityName() << "):\n";
          getline(cin >> ws, newCityName);
          tOne.setCityName(newCityName);
      }
      else if(userChoice == "7")
      {
          cout << "\nUpdate City Module****" << endl;
          cout << "\nPlease enter a new name for Team #2's City (" << tTwo.getTeamName() << ", current city: " << tTwo.getCityName() << "):\n";
          getline(cin >> ws, newCityName);
          tTwo.setCityName(newCityName);
      }
      else if(userChoice == "8")
      {
          cout << "\nUpdate Team #1 Coach Module****" << endl;
          cout << "\nPlease enter Team #1's coach Name (" << tOne.getTeamName() << ", current coach: " << tOne.getCoachName() << "): ";
          getline(cin >> ws, newCoachName);
          tOne.setCoachName(newCoachName);
      }
      else if(userChoice == "9")
      {
          cout << "\nUpdate tEAM #2 Coach Module****" << endl;
          cout << "\nPlease enter Team #2's coach Name (" << tTwo.getTeamName() << ", current coach: " << tTwo.getCoachName() << "): ";
          getline(cin >> ws, newCoachName);
          tTwo.setCoachName(newCoachName);
      }
      else if(userChoice == "10")
      {
        cout << "\nUpdate Timeout Module****" << endl;
        cout << "\nPlease enter a new number of timeouts for Team #1 (" << tOne.getTeamName() << ", current timeouts: " << tOne.getTimeoutCount() << "):\n";
        while (true) { //loop to validate input for changing the timeout count
            newTimeout = validateInt(newTimeout);
                if (newTimeout >= 0 && newTimeout < 4) {
                    tOne.setTimeoutCount(newTimeout);  //set the new timeout count
                    break;
                }
                else {
                    cout << "Please enter a number between 0 and 3.\n";
                    continue;
                }
        }
      }
      else if(userChoice == "11")
      {
        cout << "\nUpdate Timeout Module****" << endl;
        cout << "\nPlease enter a new number of timeouts for Team #2 (" << tTwo.getTeamName() << ", current timeouts: " << tTwo.getTimeoutCount() << "):\n";
        while (true) { //loop to validate input for changing the timeout count
            newTimeout = validateInt(newTimeout);
                if (newTimeout >= 0 && newTimeout < 4) {
                    tTwo.setTimeoutCount(newTimeout);  //set the new timeout count
                    break;
                }
                else {
                    cout << "Please enter a number between 0 and 3.\n";
                    continue;
                }
        }
      }
      else if(userChoice == "C" || userChoice == "c")
      {
        cout << "\nUpdate Home Status Module****" << endl;
        cout << "\nWho is the home team: 1 = T1, 2=T2: ";
        homeTeamQuestion = validateInt(homeTeamQuestion);
       // cin >> homeTeamQuestion;
        if(homeTeamQuestion == 1)
        {
          tOne.setHomeStatus(true);
          tTwo.setHomeStatus(false);
        }
        else if(homeTeamQuestion == 2)
        {
          tOne.setHomeStatus(false);
          tTwo.setHomeStatus(true);
          s.setHome(tTwo);
          s.setVisitor(tOne);
        }
        else
        {
          cout << "\nInvalid Input!" << endl;
          sleep(2);
        }
      }
      else if(userChoice == "E" || userChoice == "e")
      {
        cout << "Exit chosen." << endl;
      }
      else //invalid input: default clause
      {
        cout << "\nInvalid input." << endl;
        sleep(2); //wait two seconds, go to the next process
      }

      s.setHome(tOne); //refresh the data in s to the new updates...
      s.setVisitor(tTwo); //refresh the data

  }while(userChoice != "E" && userChoice != "e");
}



#endif // BOARD_H_INCLUDED
