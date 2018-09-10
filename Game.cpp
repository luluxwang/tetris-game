#include "Game.h"
#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

Game::Game(int width, int height)
 : m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_level(1)
{
    Well(m_well);       //Each game has a well
    
    m_score = 0;
    m_rowsLeft = 5 * m_level;
    levelStart = true;
    
    if (1000-(100*(m_level-1) > 100))
        maxTime = 1000-(100*(m_level-1));
    else
        maxTime = 100;
}

void Game::play()
{
    createPiece();
    pieceptr->createShape();
    createNextPiece();
    nextPieceptr->createShape();

    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  Level, Score, Rows Left
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // [in UserInterface.h]
    
    for(;;)
    {
        if ( ! playOneLevel())
        {
            break;
        }
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();
        m_level++;
        m_rowsLeft = 5 * m_level;       //Updates the number of rows left needed to complete level
                                        //Score does NOT reset
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}

void Game::displayPrompt(std::string s)     
{
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

void Game::displayStatus()
{
    // [Replace this with code to display the status]
    m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
    m_screen.printString("Next Piece:");
    
    if (!levelStart)
    {
        m_screen.gotoXY(NEXT_PIECE_X, NEXT_PIECE_Y);
        nextPieceptr->displayNext(m_screen);
    }
    
    m_screen.gotoXY(SCORE_X, SCORE_Y);
    string score = "Score:     ";
    string scoreString = to_string(m_score);
    int scoreLen = scoreString.length();            //Works under the assumption that m_score is always 7 characters or less
    for (int i = 0; i < 7-scoreLen; i++)
        score += " ";
    score += scoreString;
    m_screen.printString(score);
    
    m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
    string rowsLeft = "Rows Left: ";
    string rowsLeftString = to_string(m_rowsLeft);
    int rowsLeftLen = rowsLeftString.length();      //Works under the assumption that m_rowsLeft is always 7 characters or less
    for (int i = 0; i < 7-rowsLeftLen; i++)
        rowsLeft += " ";
    rowsLeft += rowsLeftString;
    m_screen.printString(rowsLeft);
    
    m_screen.gotoXY(LEVEL_X, LEVEL_Y);
    string level = "Level:     ";
    string levelString = to_string(m_level);
    int levelLen = levelString.length();            //Works under the assumption that m_level is always 7 characters or less
    for (int i = 0; i < 7-levelLen; i++)
        level += " ";
    level += levelString;
    m_screen.printString(level);
}

bool Game::playOneLevel()
{
    bool gameInPlay = true;
    while (gameInPlay)
    {
        levelStart = false;
        refreshScreen();
        pieceptr->display(m_screen);
        
        bool stopped = false;
        bool fallOnce = true;
        Timer m_timer;
        
        while (m_timer.elapsed() < maxTime)
        {
            char ch;
            if (getCharIfAny(ch))
            {
                switch (ch)
                {
                    case ' ':
                        pieceptr->gotoBottom();
                        pieceptr->landed();         //Update well to '$'
                        doSpecialThings();          //Do any special things that may occur or not
                        removeFilledRows();         //Remove filled rows
                        m_well.display(m_screen, WELL_X, WELL_X);
                        
                        delete pieceptr;
                        pieceptr = nextPieceptr;    //Reassign current piece pointer to next piece
                        createNextPiece();          //Reassign next piece pointer to a new next piece
                        nextPieceptr->createShape();
                        refreshScreen();
                        
                        stopped = false;
                        break;
                    case ARROW_LEFT:
                        pieceptr->moveLeft();
                        refreshScreen();
                        break;
                    case ARROW_UP:
                        pieceptr->rotateClockwise();
                        refreshScreen();
                        break;
                    case ARROW_DOWN:
                        pieceptr->fall();
                        refreshScreen();
                        break;
                    case ARROW_RIGHT:
                        pieceptr->moveRight();
                        refreshScreen();
                        break;
                    case 'Q':
                    case 'q':
                        return false;
                }
            }
        }
        if (fallOnce)
        {
            stopped = !(pieceptr->fall());  //If it didn't fall, return false, stopped is true
            refreshScreen();
            fallOnce = false;
        }
        if (stopped)
        {
            pieceptr->landed();         //Update well to '$'
            doSpecialThings();          //Do any special things that may occur or not
            removeFilledRows();         //Remove filled rows
            refreshScreen();
        
            delete pieceptr;
            pieceptr = nextPieceptr;    //Reassign current piece pointer to next piece
            createNextPiece();          //Reassign next piece pointer to a new next piece
            nextPieceptr->createShape();
            stopped = false;
        }
        if ((isOverlap()))
        {
            pieceptr->landed();
            refreshScreen();
            delete pieceptr;
            delete nextPieceptr;
            return false;
        }
        if (rowsLeft() <= 0)
        {
            //Don't vaporize the rows
            m_screen.clear();                   //Make sure the next piece doesn't display
            m_well.display(m_screen, WELL_X, WELL_Y);
            displayStatus();
            
            clearWell();                        //Reset the well to empty spaces and '@' borders
            delete pieceptr;                    //Delete the current piece
            delete nextPieceptr;                //Delete the next piece
            
            createPiece();                      //Remake a new piece at random for the next level
            pieceptr->createShape();
            createNextPiece();                  //Rameke a new next piece at random for the next level
            nextPieceptr->createShape();
            return true;
        }
    }
    return true;
}

void Game::clearWell()      //Resets the well to empty spaces and '@' borders
{
    for (int col = 0; col < m_well.cols(); col++)
    {
        for (int row = 0; row < m_well.rows(); row++)
        {
            m_well.update(' ', col, row);
            m_well.update('@', 0, row);
            m_well.update('@', m_well.cols()-1, row);
            m_well.update('@', col, m_well.rows()-1);
        }
    }
}

bool Game::isOverlap() const     //Checks to see if when a new piece goes into the well, will it overlap anything?
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((nextPieceptr->accessPiece(i, j) != ' ') &&
                (m_well.accessWell(nextPieceptr->getX() + i, nextPieceptr->getY() + j) != ' '))
                return true;        //It DOES overlap...BAD
        }
    }
    return false;       //Good to go!
}

void Game::createPiece()    //Depending on which piece is returned, the current piece pointer will point to a new Piece of returned type
{
    switch (chooseRandomPieceType())
    {
        case PIECE_O:
            pieceptr = new PieceO(m_well);
            break;
        case PIECE_I:
            pieceptr = new PieceI(m_well);
            break;
        case PIECE_J:
            pieceptr = new PieceJ(m_well);
            break;
        case PIECE_L:
            pieceptr = new PieceL(m_well);
            break;
        case PIECE_T:
            pieceptr = new PieceT(m_well);
            break;
        case PIECE_S:
            pieceptr = new PieceS(m_well);
            break;
        case PIECE_Z:
            pieceptr = new PieceZ(m_well);
            break;
        case PIECE_VAPOR:
            pieceptr = new PieceVapor(m_well);
            break;
        case PIECE_FOAM:
            pieceptr = new PieceFoam(m_well);
            break;
        case PIECE_CRAZY:
            pieceptr = new PieceCrazy(m_well);
            break;
        default:
            pieceptr = new PieceO(m_well);
            break;
    }
}

void Game::createNextPiece()    //Depending on which piece is returned, the next piece pointer will point to a new Piece of returned type
{
    switch (chooseRandomPieceType())
    {
        case PIECE_O:
            nextPieceptr = new PieceO(m_well);
            break;
        case PIECE_I:
            nextPieceptr = new PieceI(m_well);
            break;
        case PIECE_J:
            nextPieceptr = new PieceJ(m_well);
            break;
        case PIECE_L:
            nextPieceptr = new PieceL(m_well);
            break;
        case PIECE_T:
            nextPieceptr = new PieceT(m_well);
            break;
        case PIECE_S:
            nextPieceptr = new PieceS(m_well);
            break;
        case PIECE_Z:
            nextPieceptr = new PieceZ(m_well);
            break;
        case PIECE_VAPOR:
            nextPieceptr = new PieceVapor(m_well);
            break;
        case PIECE_FOAM:
            nextPieceptr = new PieceFoam(m_well);
            break;
        case PIECE_CRAZY:
            nextPieceptr = new PieceCrazy(m_well);
            break;
        default:
            nextPieceptr = new PieceO(m_well);
            break;
    }
}

void Game::refreshScreen()      //Clears the screen, redisplays well, status, prompt, and piece
{
    m_screen.clear();
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    pieceptr->display(m_screen);
}

void Game::doSpecialThings()    //If the current piece pointer is either a VAPOR PIECE or FOAM BOMB then it will perform its special actions accordingly
{
    pieceptr->vaporPiece();
    pieceptr->foamBomb(pieceptr->getX(), pieceptr->getY());
}

int Game::removeFilledRows()
{
    int rowsRemoved = 0;
    for (int i = 0; i < m_well.rows(); i++)
    {
        int rowCount = 0;               //Counts the number of non-space items along one row
        for (int j = 1; j < m_well.cols()-1; j++)
        {
            if ((m_well.accessWell(j, i) == '$') || (m_well.accessWell(j, i) == '*'))
            {
                rowCount++;
            }
            if (rowCount == m_well.cols()-2)   //If the count is the size of the well-2 (to exclude borders), then it's filled
            {
                m_well.vaporizeRow(i);          //Vaporize the rows
                rowsRemoved++;
                if (m_rowsLeft > 0)     //Ensure that m_rowsLeft never displays negative
                {
                    m_rowsLeft--;
                }
            }
        }
    }
    updateScore(rowsRemoved);           //Based on number of rows removed simultaneously, the score will be updated
    return rowsRemoved;
}

void Game::updateScore(int val)     //Update score based on number of rows removed simultaneously
{
    if (val == 1)
    {
        m_score += 100;
    }
    else if (val == 2)
    {
        m_score += 200;
    }
    else if (val == 3)
    {
        m_score += 400;
    }
    else if (val == 4)
    {
        m_score += 800;
    }
    else if (val == 5)
    {
        m_score += 1600;
    }
    else
    {
        m_score += 0;        //Includes case when rowsVaporized is 0 and any other cases (which aren't possible but just in case)
    }
}

int Game::score() const
{
    return m_score;
}

int Game::level() const
{
    return m_level;
}

int Game::rowsLeft() const
{
    return m_rowsLeft;
}


//Four steps: Make game. Make piece. Start falling. Check stop falling. Check well. Repeat.








