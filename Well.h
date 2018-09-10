#ifndef WELL_INCLUDED
#define WELL_INCLUDED

class Screen;       //already here pls don't change
class Piece;

class Well
{
public:
    Well();
    void display(Screen& screen, int x, int y) const;
    
    //ACCESSORS//
    int rows() const;
    int cols() const;
    int scanWell(int x, int y) const;
    int emptyRowAbove(int row) const;
    
    //MUTATORS//
    char accessWell(int x, int y) const;
    void update(char ch, int x, int y);
    void vaporizeRow(int row);
    
private:
    const int MAX_ROWS;
    const int MAX_COLS;
    char theWell[12][19];
};

#endif // WELL_INCLUDED
