#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>
// [Add other #include directives as necessary.]

class Piece;

class Game
{
public:
    Game(int width, int height);
    void play();
    bool playOneLevel();
    void displayPrompt(std::string s);
    void displayStatus();
    // [Add other members as necessary]
    void createPiece();
    void createNextPiece();
    void refreshScreen();
    void doSpecialThings();
    bool isOverlap() const;           //work on this!!
    int  removeFilledRows();
    int  rowsLeft() const;
    int  score() const;
    void updateScore(int val);
    int  level() const;
    void clearWell();
    
private:
    Well    m_well; 
    Screen  m_screen;
    int     m_level;
    // [Add other members as necessary]
    Piece*  pieceptr;
    Piece*  nextPieceptr;
    int     m_score;
    int     m_rowsLeft;
    int     maxTime;
    bool    levelStart;
};

#endif // GAME_INCLUDED
