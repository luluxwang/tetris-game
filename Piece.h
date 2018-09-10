#include "UserInterface.h"
#include "Well.h"

#include <vector>
using namespace std;

#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType();

////////////////////////////////////////ABSTRACT CLASS////////////////////////////////////////
class Piece
{
public:
    Piece(Well& w);
    virtual ~Piece();
    void display(Screen& screen) const;
    void displayNext(Screen & screen) const;
    char accessPiece(int x, int y) const;
    void updatePiece(char ch, int x, int y);
    char accessTemp(int x, int y) const;
    void updateTemp(char ch, int x, int y);
    bool checkBottomBounds() const;              //Returns false if it hits something
    bool checkRightBounds() const;               //Returns false if it hits something
    bool checkLeftBounds() const;                //Returns false if it hits something
    int  getX() const;
    int  getY() const;
    int  getOrientation() const;
    void changeOrientation();
    bool canRotate();
    
    //ACTIONS//
    bool fall();
    void landed();
    void gotoBottom();
    virtual bool moveLeft();
    virtual bool moveRight();
    virtual void vaporPiece();
    virtual void  foamBomb(int x, int y);
    
    //PURE VIRTUAL FUNCTIONS//
    virtual void createShape() = 0;     //Each shape is different and just "a piece" has no real shape
    virtual void rotateClockwise() = 0;     //go back and work on this

private:
 //   vector<vector<char>> m_bounds;          //To record current piece
  //  vector<vector<char>> tempRotation;      //To record next rotation
    char m_bounds[4][4];
    char tempRotation[4][4];
    int     m_x;
    int     m_y;
    int     orientation;
    Well*   wellptr;
    int     endXPos;
    int     endXNeg;
    int     endYPos;
    int     endYNeg;
};

////////////////////////////////////////O PIECE////////////////////////////////////////
class PieceO : public Piece
{
public:
    PieceO(Well& w);
    virtual ~PieceO();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////I PIECE////////////////////////////////////////
class PieceI : public Piece
{
public:
    PieceI(Well& w);
    virtual ~PieceI();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////J PIECE////////////////////////////////////////
class PieceJ : public Piece
{
public:
    PieceJ(Well& w);
    virtual ~PieceJ();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////L PIECE////////////////////////////////////////
class PieceL : public Piece
{
public:
    PieceL(Well& w);
    virtual ~PieceL();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////T PIECE////////////////////////////////////////
class PieceT : public Piece
{
public:
    PieceT(Well& w);
    virtual ~PieceT();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////S PIECE////////////////////////////////////////
class PieceS : public Piece
{
public:
    PieceS(Well& w);
    virtual ~PieceS();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////Z PIECE////////////////////////////////////////
class PieceZ : public Piece
{
public:
    PieceZ(Well& w);
    virtual ~PieceZ();
    virtual void createShape();
    virtual void rotateClockwise();
private:
};

////////////////////////////////////////CRAZY PIECE////////////////////////////////////////
class PieceCrazy : public Piece
{
public:
    PieceCrazy(Well& w);
    virtual ~PieceCrazy();
    virtual void createShape();
    virtual void rotateClockwise();
    virtual bool moveLeft();
    virtual bool moveRight();
private:
};

////////////////////////////////////////VAPOR PIECE////////////////////////////////////////
class PieceVapor : public Piece
{
public:
    PieceVapor(Well& w);
    virtual ~PieceVapor();
    virtual void createShape();
    virtual void rotateClockwise();
    virtual void vaporPiece();
private:
    Well* wellptr;
};

////////////////////////////////////////FOAM PIECE////////////////////////////////////////
class PieceFoam : public Piece
{
public:
    PieceFoam(Well& w);
    virtual ~PieceFoam();
    virtual void createShape();
    virtual void rotateClockwise();
    virtual void foamBomb(int x, int y);
private:
    Well* wellptr;
    int Xlimit;
    int Ylimit;
};


#endif // PIECE_INCLUDED
