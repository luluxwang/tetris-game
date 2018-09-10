#include "Piece.h"

////////////////////////////////////////CONSTRUCTOR AND DESTRUCTOR////////////////////////////////////////
Piece::Piece(Well& w)   //Each piece has a pointer to the main well
: m_x(3), m_y(0), orientation(0)        //Start coordinates of piece's box's top left corner are (3,0)
{
    wellptr = &w;
    
   /* vector<vector<char>> tempVec(4, vector<char>(4, ' '));
    m_bounds = tempVec;
    tempRotation = tempVec;
    endXPos = 0;
    endXNeg = 0;
    endYPos = 0;
    endYNeg = 0;*/
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_bounds[i][j] = ' ';
        }
    }
}

Piece::~Piece()
{
    //Empty
}

void Piece::display(Screen& screen) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                screen.gotoXY(m_x+i, m_y+j);
                screen.printChar(m_bounds[i][j]);
            }
        }
    }
    screen.refresh();
}

void Piece::displayNext(Screen& screen) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                screen.gotoXY(16+i, 4+j);
                screen.printChar(m_bounds[i][j]);
            }
        }
    }
}

char Piece::accessPiece(int x, int y) const
{
    return m_bounds[x][y];
}

void Piece::updatePiece(char ch, int x, int y)
{
    m_bounds[x][y] = ch;
}

char Piece::accessTemp(int x, int y) const
{
     return tempRotation[x][y];
}

void Piece::updateTemp(char ch, int x, int y)
{
    tempRotation[x][y] = ch;
}

bool Piece::fall()
{
    if (checkBottomBounds())
    {
        m_y++;
        return true;
    }
    return false;
}

void Piece::landed()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                wellptr->update('$', m_x+i, m_y+j);
            }
        }
    }
}

void Piece::gotoBottom()    //work on this
{
    int newY = 0;               //Y-coord of where the block can possibly fall
    int bottomOfPiece = 0;      //Y-coord of the bottom of the piece
    int falldown = 0;           //Difference between Y-coords (ie. how many spaces to move down)
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                bottomOfPiece = j;
                newY = wellptr->scanWell(m_x, m_y+j);
            }
        }
    }
    falldown = newY - bottomOfPiece;
    while (falldown > 0)
    {
        fall();
        falldown--;
    }
}

bool Piece::moveLeft()
{
    if (checkLeftBounds())
    {
        m_x--;
    }
    return true;
}

bool Piece::moveRight()
{
    if (checkRightBounds())
    {
        m_x++;
    }
    return true;
}

bool Piece::canRotate()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tempRotation[i][j] == '#')
            {
                if (wellptr->accessWell(m_x+i, m_y+j) != ' ')       //If the well has something other than a space
                                                                    //the new orientation will NOT be valid
                {
                    return false;       //Cannot rotate
                }
            }
        }
    }
    return true;
}


bool Piece::checkBottomBounds() const     //see if the piece is going to collide into anything while falling
{                                   //returns false if you've hit something; true if you're ok to keep falling
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#') //if you come across a '#', go to the well and see if the row below it is free or not
            {
                if ((wellptr->accessWell(m_x+i, m_y+j+1) != ' ') || (wellptr->accessWell(m_x+i, m_y+j+1) == 0))
                {
                    return false;
                }
            }
        }
    }
    return true;       //OK to fall
}

bool Piece::checkRightBounds() const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                if (wellptr->accessWell(m_x+i+1, m_y+j) != ' ' && wellptr->accessWell(m_x+i+1, m_y+j) != 0)
                {
                    return false;    //You've hit something
                }
            }
        }
    }
    return true;
}

bool Piece::checkLeftBounds() const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_bounds[i][j] == '#')
            {
                if (wellptr->accessWell(m_x+i-1, m_y+j) != ' ' && wellptr->accessWell(m_x+i-1, m_y+j) != 0)
                {
                    return false;    //You've hit something
                }
            }
        }
    }
    return true;
}

int Piece::getX() const
{
    return m_x;
}

int Piece::getY() const
{
    return m_y;
}

int Piece::getOrientation() const
{
    return orientation;
}

void Piece::changeOrientation()
{
    if (orientation == 3)
        orientation = 0;
    else
    {
        orientation++;
    }
}

void Piece::vaporPiece()
{
    //Nothing...will get overwritten by VAPOR PIECE
}

void Piece::foamBomb(int x, int y)
{
    //Nothing...will get overwritten by FOAM PIECE
}

////////////////////////////////////////O PIECE////////////////////////////////////////
PieceO::PieceO(Well& w)
: Piece(w)
{
}

PieceO::~PieceO()
{
    //Empty
}

void PieceO::createShape()
{
    updatePiece('#', 0, 0);
    updatePiece('#', 0, 1);
    updatePiece('#', 1, 0);
    updatePiece('#', 1, 1);
}

void PieceO::rotateClockwise()
{
    //No changes when O rotates
}

////////////////////////////////////////I PIECE////////////////////////////////////////
PieceI::PieceI(Well& w)
: Piece(w)
{
}

PieceI::~PieceI()
{
    //Empty
}

void PieceI::createShape()
{
    updatePiece('#', 0, 1);
    updatePiece('#', 1, 1);
    updatePiece('#', 2, 1);
    updatePiece('#', 3, 1);
}

void PieceI::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the NEW(!) orientation value
    {
        case 0:
        case 2:
            updateTemp('#', 0, 1);
            updateTemp('#', 2, 1);
            updateTemp('#', 3, 1);
            
            updateTemp(' ', 1, 0);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 1, 3);
            
            if (canRotate())
            {
                updatePiece('#', 0, 1);
                updatePiece('#', 2, 1);
                updatePiece('#', 3, 1);
                
                updatePiece(' ', 1, 0);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 1, 3);
            }
            break;
        case 1:
        case 3:
            updateTemp('#', 1, 0);
            updateTemp('#', 1, 2);
            updateTemp('#', 1, 3);
            
            updateTemp(' ', 0, 1);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 3, 1);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 1, 2);
                updatePiece('#', 1, 3);
                
                updatePiece(' ', 0, 1);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 3, 1);
            }
            break;
    }
}

////////////////////////////////////////J PIECE////////////////////////////////////////
PieceJ::PieceJ(Well& w)
: Piece(w)
{
}

PieceJ::~PieceJ()
{
    //Empty
}

void PieceJ::createShape()
{
    updatePiece('#', 1, 1);
    updatePiece('#', 2, 1);
    updatePiece('#', 3, 1);
    updatePiece('#', 3, 2);
}

void PieceJ::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
            updateTemp('#', 1, 1);
            updateTemp('#', 2, 1);
            updateTemp('#', 3, 1);
            updateTemp('#', 3, 2);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 1, 1);
                updatePiece('#', 2, 1);
                updatePiece('#', 3, 1);
                updatePiece('#', 3, 2);
                
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 1:
            updateTemp('#', 2, 1);
            updateTemp('#', 2, 2);
            updateTemp('#', 2, 3);
            updateTemp('#', 1, 3);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 1, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 2, 1);
                updatePiece('#', 2, 2);
                updatePiece('#', 2, 3);
                updatePiece('#', 1, 3);
                
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 1, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 2:
            updateTemp('#', 1, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 2);
            updateTemp('#', 3, 2);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 1, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 2);
                updatePiece('#', 3, 2);
                
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 3:
            updateTemp('#', 1, 0);
            updateTemp('#', 1, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 0);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 1, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 0);
                
                updatePiece(' ', 0, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
    }
}

////////////////////////////////////////L PIECE////////////////////////////////////////
PieceL::PieceL(Well& w)
: Piece(w)
{
}

PieceL::~PieceL()
{
    //Empty
}

void PieceL::createShape()
{
    updatePiece('#', 0, 1);
    updatePiece('#', 1, 1);
    updatePiece('#', 2, 1);
    updatePiece('#', 0, 2);
}

void PieceL::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
            updateTemp('#', 0, 1);
            updateTemp('#', 1, 1);
            updateTemp('#', 2, 1);
            updateTemp('#', 0, 2);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 0, 1);
                updatePiece('#', 1, 1);
                updatePiece('#', 2, 1);
                updatePiece('#', 0, 2);
                
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 1:
            updateTemp('#', 1, 0);
            updateTemp('#', 2, 0);
            updateTemp('#', 2, 1);
            updateTemp('#', 2, 2);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 1, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 2, 0);
                updatePiece('#', 2, 1);
                updatePiece('#', 2, 2);
            
                updatePiece(' ', 0, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 1, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 2:
            updateTemp('#', 0, 2);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 1);
            updateTemp('#', 2, 2);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 1, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 3);
            updateTemp(' ', 2, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 0, 2);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 1);
                updatePiece('#', 2, 2);
            
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 1, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 3);
                updatePiece(' ', 2, 3);
                updatePiece(' ', 3, 3);
            }
            break;
        case 3:
            updateTemp('#', 1, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 1, 3);
            updateTemp('#', 2, 3);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 0);
            updateTemp(' ', 3, 0);
            updateTemp(' ', 0, 1);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 3, 1);
            updateTemp(' ', 0, 2);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 3, 2);
            updateTemp(' ', 0, 3);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 1, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 1, 3);
                updatePiece('#', 2, 3);
            
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 0);
                updatePiece(' ', 3, 0);
                updatePiece(' ', 0, 1);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 3, 1);
                updatePiece(' ', 0, 2);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 3, 2);
                updatePiece(' ', 0, 3);
                updatePiece(' ', 3, 3);
            }
            break;
    }
}

////////////////////////////////////////T PIECE////////////////////////////////////////
PieceT::PieceT(Well& w)
: Piece(w)
{
}

PieceT::~PieceT()
{
    //Empty
}

void PieceT::createShape()
{
    updatePiece('#', 0, 1);
    updatePiece('#', 1, 0);
    updatePiece('#', 1, 1);
    updatePiece('#', 2, 1);
}

void PieceT::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
            updateTemp('#', 0, 1);
            updateTemp('#', 1, 0);
            updateTemp('#', 2, 1);
            
            updateTemp(' ', 1, 2);
            
            if (canRotate())
            {
                updatePiece('#', 0, 1);
                updatePiece('#', 1, 0);
                updatePiece('#', 2, 1);
                
                updatePiece(' ', 1, 2);
            }
            break;
        case 1:
            updateTemp('#', 1, 0);
            updateTemp('#', 2, 1);
            updateTemp('#', 1, 2);
            
            updateTemp(' ', 0, 1);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 2, 1);
                updatePiece('#', 1, 2);
                
                updatePiece(' ', 0, 1);
            }
            break;
        case 2:
            updateTemp('#', 0, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 1);
            
            updateTemp(' ', 1, 0);
            
            if (canRotate())
            {
                updatePiece('#', 0, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 1);
                
                updatePiece(' ', 1, 0);
            }
            break;
        case 3:
            updateTemp('#', 1, 0);
            updateTemp('#', 0, 1);
            updateTemp('#', 1, 2);
            
            updateTemp(' ', 2, 1);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 0, 1);
                updatePiece('#', 1, 2);
                
                updatePiece(' ', 2, 1);
            }
            break;
    }
}

////////////////////////////////////////S PIECE////////////////////////////////////////
PieceS::PieceS(Well& w)
: Piece(w)
{
}

PieceS::~PieceS()
{
    //Empty
}

void PieceS::createShape()
{
    updatePiece('#', 0, 2);
    updatePiece('#', 1, 1);
    updatePiece('#', 1, 2);
    updatePiece('#', 2, 1);
}

void PieceS::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
        case 2:
            updateTemp('#', 0, 2);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 1);
            
            updateTemp(' ', 1, 0);
            updateTemp(' ', 2, 2);
            
            if (canRotate())
            {
                updatePiece('#', 0, 2);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 1);
                
                updatePiece(' ', 1, 0);
                updatePiece(' ', 2, 2);
            }
            break;
        case 1:
        case 3:
            updateTemp('#', 1, 0);
            updateTemp('#', 2, 1);
            updateTemp('#', 2, 2);
            
            updateTemp(' ', 0, 2);
            updateTemp(' ', 1, 2);
            
            if (canRotate())
            {
                updatePiece('#', 1, 0);
                updatePiece('#', 2, 1);
                updatePiece('#', 2, 2);
                
                updatePiece(' ', 0, 2);
                updatePiece(' ', 1, 2);
            }
            break;
    }
}

////////////////////////////////////////Z PIECE////////////////////////////////////////
PieceZ::PieceZ(Well& w)
: Piece(w)
{
}

PieceZ::~PieceZ()
{
    //Empty
}

void PieceZ::createShape()
{
    updatePiece('#', 0, 1);
    updatePiece('#', 1, 1);
    updatePiece('#', 1, 2);
    updatePiece('#', 2, 2);
}

void PieceZ::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
        case 2:
            updateTemp('#', 0, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 2);
            
            updateTemp(' ', 2, 0);
            updateTemp(' ', 2, 1);
            
            if (canRotate())
            {
                updatePiece('#', 0, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 2);
                
                updatePiece(' ', 2, 0);
                updatePiece(' ', 2, 1);
            }
            break;
        case 1:
        case 3:
            updateTemp('#', 2, 0);
            updateTemp('#', 2, 1);
            updateTemp('#', 1, 2);
            
            updateTemp(' ', 0, 1);
            updateTemp(' ', 2, 2);
            
            if (canRotate())
            {
                updatePiece('#', 2, 0);
                updatePiece('#', 2, 1);
                updatePiece('#', 1, 2);
                
                updatePiece(' ', 0, 1);
                updatePiece(' ', 2, 2);
            }
            break;
    }
}

////////////////////////////////////////CRAZY PIECE////////////////////////////////////////
PieceCrazy::PieceCrazy(Well& w)
: Piece(w)
{
}

PieceCrazy::~PieceCrazy()
{
    //Empty
}

void PieceCrazy::createShape()
{
    updatePiece('#', 0, 0);
    updatePiece('#', 1, 1);
    updatePiece('#', 2, 1);
    updatePiece('#', 3, 0);
}

void PieceCrazy::rotateClockwise()
{
    changeOrientation();            //Increments orientation to the next one
    switch (getOrientation())       //Accesses the orientation value (after it's been changed!)
    {
        case 0:
            updateTemp('#', 0, 0);
            updateTemp('#', 1, 1);
            updateTemp('#', 2, 1);
            updateTemp('#', 3, 0);
            
            updateTemp(' ', 0, 3);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 0, 0);
                updatePiece('#', 1, 1);
                updatePiece('#', 2, 1);
                updatePiece('#', 3, 0);
            
                updatePiece(' ', 0, 3);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 3, 3);
            }
            break;
        case 1:
            updateTemp('#', 3, 0);
            updateTemp('#', 2, 1);
            updateTemp('#', 2, 2);
            updateTemp('#', 3, 3);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 1);
            updateTemp(' ', 1, 2);
            updateTemp(' ', 0, 3);
            
            if (canRotate())
            {
                updatePiece('#', 3, 0);
                updatePiece('#', 2, 1);
                updatePiece('#', 2, 2);
                updatePiece('#', 3, 3);
            
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 1);
                updatePiece(' ', 1, 2);
                updatePiece(' ', 0, 3);
            }
            break;
        case 2:
            updateTemp('#', 0, 3);
            updateTemp('#', 1, 2);
            updateTemp('#', 2, 2);
            updateTemp('#', 3, 3);
            
            updateTemp(' ', 0, 0);
            updateTemp(' ', 1, 1);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 3, 0);
            
            if (canRotate())
            {
                updatePiece('#', 0, 3);
                updatePiece('#', 1, 2);
                updatePiece('#', 2, 2);
                updatePiece('#', 3, 3);
            
                updatePiece(' ', 0, 0);
                updatePiece(' ', 1, 1);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 3, 0);
            }
            break;
        case 3:
            updateTemp('#', 0, 0);
            updateTemp('#', 1, 1);
            updateTemp('#', 1, 2);
            updateTemp('#', 0, 3);
            
            updateTemp(' ', 3, 0);
            updateTemp(' ', 2, 1);
            updateTemp(' ', 2, 2);
            updateTemp(' ', 3, 3);
            
            if (canRotate())
            {
                updatePiece('#', 0, 0);
                updatePiece('#', 1, 1);
                updatePiece('#', 1, 2);
                updatePiece('#', 0, 3);
                
                updatePiece(' ', 3, 0);
                updatePiece(' ', 2, 1);
                updatePiece(' ', 2, 2);
                updatePiece(' ', 3, 3);
            }
            break;
    }
}

bool PieceCrazy::moveLeft()
{
    if (checkRightBounds())
    {
        Piece::moveRight();
        return true;
    }
    return false;
    
}

bool PieceCrazy::moveRight()
{
    if (checkLeftBounds())
    {
        Piece::moveLeft();
        return true;
    }
    return false;
}

////////////////////////////////////////VAPOR PIECE////////////////////////////////////////
PieceVapor::PieceVapor(Well& w)
: Piece(w)
{
    wellptr = &w;
}

PieceVapor::~PieceVapor()
{
    //Empty
}

void PieceVapor::createShape()
{
    updatePiece('#', 1, 0);
    updatePiece('#', 2, 0);
}

void PieceVapor::rotateClockwise()
{
    //No changes when vapor rotates
}

void PieceVapor::vaporPiece()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (accessPiece(i, j) == '#')
            {
                if (wellptr->accessWell(getX()+i, getY()+j) != '@')
                    wellptr->update(' ', getX() + i, getY() + j);       //Vaporize itself
                
                if (wellptr->accessWell(getX()+i, getY()+j+1) != '@')
                    wellptr->update(' ', getX() + i, getY() + j + 1);     //Vaporize one row above
                
                if (wellptr->accessWell(getX()+i, getY()+j+2) != '@')
                    wellptr->update(' ', getX() + i, getY() + j + 2);     //Vaporize two rows above
                
                if (wellptr->accessWell(getX()+i, getY()+j-1) != '@')
                    wellptr->update(' ', getX() + i, getY() + j - 1);     //Vaporize one row below
                
                if (wellptr->accessWell(getX()+i, getY()+j-2) != '@')
                    wellptr->update(' ', getX() + i, getY() + j - 2);     //Vaporize two rows below
            }
        }
    }
}

////////////////////////////////////////FOAM PIECE////////////////////////////////////////
PieceFoam::PieceFoam(Well& w)
: Piece(w)
{
    wellptr = &w;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (accessPiece(i, j) == '#')
            {
                Xlimit = getX() + i + 5;
                
            }
        }
    }
    Xlimit = 0;
    Ylimit = 0;
}

PieceFoam::~PieceFoam()
{
    //Empty
}

void PieceFoam::createShape()
{
    updatePiece('#', 1, 1);
}

void PieceFoam::rotateClockwise()
{
    //No changes when foam rotates
}

void PieceFoam::foamBomb(int x, int y)
{
    //Attempted recrusion but was unsuccessful...
    
    if (wellptr->accessWell(x, y) != ' ')       //Base case of recursion
    {
        return;
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (accessPiece(i, j) == '#')
            {
                wellptr->update('*', x+i, y+j);                   //Change's foam bomb's current location to '*'
                
                if (wellptr->accessWell(x+i, y+j-1) == ' ')       //North
                {
                    wellptr->update('*', x+i, y+j-1);
                 //   return foamBomb(x, y-1);
                }
                if (wellptr->accessWell(x+i+1, y+j) == ' ')       //East
                {
                    wellptr->update('*', x+i+1, y+j);
                  //  return foamBomb(x+1, y);
                }
                if (wellptr->accessWell(x+i, y+j+1) == ' ')       //South
                {
                    wellptr->update('*', x+i, y+j+1);
                //    return foamBomb(x, y+1);
                }
                if (wellptr->accessWell(x+i-1, y+j) == ' ')       //West
                {
                    wellptr->update('*', x+i-1, y+j);
                  //  return foamBomb(x-1, y);
                }
            }
        }
    }
    
   /* if (x+2 >= wellptr->cols())
    {
        endXPos = wellptr->cols()-1;
        endCol2 = x-2;
    }
    if (x-2 <= 0)
    {
        endCol = x+2;
        endCol2 = 0;
    }
    
    if (y+2 >= wellptr->rows())
    {
        endRow = wellptr->rows()-1;
        endRow2 = y-2;
    }
    if (y-2 <= 0)
    {
        endCol = y+2;
        endCol2 = 0;
    }*/
}
