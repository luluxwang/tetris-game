#include "Well.h"
#include "UserInterface.h"

Well::Well()                         //2D Array of Characters
: MAX_ROWS(19), MAX_COLS(12)
{
    for (int col = 0; col < MAX_COLS; col++)
    {
        for (int row = 0; row < MAX_ROWS; row++)
        {
            theWell[col][row] = ' ';
            theWell[0][row] = '@';
            theWell[MAX_COLS-1][row] = '@';
            theWell[col][MAX_ROWS-1] = '@';
        }
    }
}

void Well::display(Screen& screen, int x, int y) const
{
    for (int col = 0; col < MAX_COLS; col++)
    {
        for (int row = 0; row < MAX_ROWS; row++)
        {
            screen.gotoXY(x + col, y + row);
            screen.printChar(theWell[col][row]);
        }
    }
    screen.refresh();
}

int Well::rows() const
{
    return MAX_ROWS;
}

int Well::cols() const
{
    return MAX_COLS;
}

void Well::update(char ch, int x, int y)
{
    if (x < MAX_COLS && y < MAX_ROWS)       //If the coordinates are within the well
        theWell[x][y] = ch;                 //change the well to the desired character
}

char Well::accessWell(int x, int y) const
{
    if (x < MAX_COLS && y < MAX_ROWS)       //If the coordinates are within the well
        return theWell[x][y];               //return the character at the location
    return 0;                               //otherwise return 0
}

int Well::scanWell(int x, int y) const      //Checks and returns the y-coordinate of the further place the piece will be able to fall in the well
{
    for (int i = 0; i < MAX_ROWS-y-1; i++)
    {
        for (int j = 0; j < 4; j++)         //j<4 because you only need to check the space under the piece
        {
            if ((theWell[i+1][j] == '$')||(theWell[i+1][j] == '*'))
                return y+i;                 //If there's something, return the coordinate
        }
    }
    return MAX_ROWS-1;                      //Otherwise return the largest y coordinate possible
}

int Well::emptyRowAbove(int row) const        //Returns Y coordinate of empty row
{
    for (int i = row; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < MAX_COLS; j++)
        {
            if (theWell[i-1][j] == ' ')
            {
                count++;
            }
            if (count == MAX_COLS-2)
            {
                return i-1;
            }
        }
    }
    return -1;                      //No empty rows above (not really possible but just in case)
}

void Well::vaporizeRow(int row)
{
    for (int y = row; y > 0; y--)              //Ensures nothing below the filled line gets changed
    {
        for (int x = 0; x < MAX_COLS; x++)
        {
            theWell[x][y] = theWell[x][y-1];    //Move every row down one
        }
    }
}











