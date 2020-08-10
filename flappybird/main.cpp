#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
using namespace std;

#define fajl "highscore.txt"

bool gameOver;
const int width = 40;
const int height = 20;
int score = 0;
int highscore = 0;

int x,y,xc1,yc1,xc2,yc2; //pozicija glave ptice

void enable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

void Setup(){
    gameOver = false;
    score = 0;
    x = 5;
    y = 11;
    xc1 = 19;
    yc1 = 11;
    xc2 = 39;
    yc2 = 11;
}
void Draw(){
    system("clear");
    for(int i = 0; i < width ;i++)
        cout << "#";
    cout << endl;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if( j == x && i == y){
                cout << '>';
            }else if(j > 0 && j < 5 && (i == y+1  || i == y|| i == y-1)){
                cout << '|';
            }else if(j == xc1 -2 && (i == yc1+4 || i == yc1-4)){
                cout << ")";
            }else if(j == xc1 +2 && (i == yc1+4 || i == yc1-4)){
                cout << "(";
            }else if(j > xc1-3 && j < xc1 +3 && (i == yc1+4 || i == yc1-4 )){
                cout << '-';
            }else if((j == xc1 +2 || j == xc1 -2) && (i > yc1 + 3 || i < yc1 -3 )){
                cout << '|';
            }else if(j == xc2 -2 && (i == yc2+4 || i == yc2-4)){
                cout << ")";
            }else if(j == xc2 +2 && (i == yc2+4 || i == yc2-4)){
                cout << "(";
            }else if(j > xc2-3 && j < xc2 +3 && (i == yc2+4 || i == yc2-4 )){
                cout << '-';
            }else if((j == xc2 +2 || j == xc2 -2) && (i > yc2 + 3 || i < yc2 -3 )){
                cout << '|';
            }else{
                cout << ' ';
            }
        }
        cout << endl;
    }

    for(int i = 0; i < width ;i++)
        cout << "#";
    cout << endl << "HighScore: " << highscore << " Score: " << score << endl;

}
void Input(){
    enable_raw_mode();
    if (kbhit()){
        y-=3;
    }
    disable_raw_mode();
    tcflush(0, TCIFLUSH);
}
void Logic(){
    y++;
    if(xc1 <= -2){
        score++;
        xc1 = 42;
        yc1 = 7+rand()%7;
    }else{
        xc1-=2;
    }
    if(xc2 <= -2){
        score++;
        xc2 = 42;
        yc2 = 7+rand()%7;
    }else{
        xc2-=  2;
    }
    if(y < 3 || y > 18     ||
        (xc1 < 7&&(y < yc1 -2  || y > yc1 + 2)) ||
        (xc2 < 7&&(y < yc2 -2|| y > yc2 + 2)))
    {
        gameOver = true;
    }


}

int main()
{
    int opcije = 1;
    Setup();
    while(opcije){
        while(!gameOver){
            Input();
            Logic();
            Draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(1 00));
        }
        if(score > highscore){
            cout << "!!NOVI HIGHTSCORE :D !!" <<endl;
            highscore = score;
        }
        Setup();
        cout << "Igramo jos?,ukucaj neki br xP" << endl;
        cout << "0 exit" << endl;
        cin >> opcije;
    }
   // Draw();
    return 0;
}
