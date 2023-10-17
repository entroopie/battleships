#include <iostream>
#include <stdio.h>
#include <cmath>
using namespace std;

#define infiniteRange 99999
#define blankValue 99
#define playerA 0
#define playerB 1
#define MAX_SHIPS 10
#define MAX_SHIPS_ALL 40

enum BATTLESHIPS { DES = 2, CRU = 3, BAT = 4, CAR = 5 };

struct playerInformation {
    bool shipsAlreadyPlaced = false;
    bool playerBecameAi = false;
    int parts = 0;
    int fleet[CAR + 1] = { 0,0,4,3,2,1 };
    int y1;
    int x1;
    int y2;
    int x2;
    int ships_Y[CAR + 1][MAX_SHIPS + 1];
    int ships_X[CAR + 1][MAX_SHIPS + 1];
    char ships_D[CAR + 1][MAX_SHIPS + 1];
    bool whichShips[CAR + 1][MAX_SHIPS + 1];
    int ships_ENGINE_Y[CAR + 1][MAX_SHIPS + 1];
    int ships_ENGINE_X[CAR + 1][MAX_SHIPS + 1];
    int ships_CANNON_Y[CAR + 1][MAX_SHIPS + 1];
    int ships_CANNON_X[CAR + 1][MAX_SHIPS + 1];
    bool ships_ENGINE[CAR + 1][MAX_SHIPS + 1];
    bool ships_CANNON[CAR + 1][MAX_SHIPS + 1];
    bool ships_RADAR[CAR + 1][MAX_SHIPS + 1];
    char** board;
} playerData[2];

void deleteArrayAndExit(char** boardGlobal, char** boardValidator, const int Y) // END THE GAME
{
    for (int i = 0; i < Y; i++)
    {
        delete[] boardGlobal[i];
        delete[] playerData[playerA].board[i];
        delete[] playerData[playerB].board[i];
    }
    delete[] boardGlobal;
    delete[] playerData[playerA].board;
    delete[] playerData[playerB].board;
    for (int i = 0; i < Y + 2; i++)
    {
        delete[] boardValidator[i];
    }
    delete[] boardValidator;
    exit(0);
}

int defineClassOfTheShip(const char ship[])                                                                                                                                             // USEFUL FUNCTIONALITIES
{
    if (strcmp(ship, "CAR") == 0) {
        return CAR;
    }
    else if (strcmp(ship, "BAT") == 0) {
        return BAT;
    }
    else if (strcmp(ship, "CRU") == 0) {
        return CRU;
    }
    else if (strcmp(ship, "DES") == 0) {
        return DES;
    }
    else return 0;
}

double calculatePoints(const int posY, const int posX, const int destinyY, const int destinyX)
{
    double result;
    result = sqrt(pow(abs(posY - destinyY), 2) + pow(abs(posX - destinyX), 2));
    return result;
}

void checkWinCondition(const int player, char** boardGlobal, char** boardValidator, const int Y)
{
    if (playerData[player].parts <= 0)
    {
        switch (player)
        {
        case playerA:
            printf("B won\n");
            break;
        case playerB:
            printf("A won\n");
            break;
        }
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
}

void nameTheShip(char shipName[], const int temp)
{
    switch (temp)
    {
    case CAR:
        shipName[0] = 'C';
        shipName[1] = 'A';
        shipName[2] = 'R';
        break;
    case BAT:
        shipName[0] = 'B';
        shipName[1] = 'A';
        shipName[2] = 'T';
        break;
    case CRU:
        shipName[0] = 'C';
        shipName[1] = 'R';
        shipName[2] = 'U';
        break;
    case DES:
        shipName[0] = 'D';
        shipName[1] = 'E';
        shipName[2] = 'S';
        break;
    }
    shipName[3] = '\0';
}

int definePlayer(const char who)
{
    switch (who)
    {
    case 'A':
        return playerA;
        break;
    case 'B':
        return playerB;
        break;
    }
    return blankValue;
}

int inversePlayer(const int player)
{
    if (player == playerA)
    {
        return playerB;
    }
    else return playerA;
}

int changeTemp(const int temp, const char direction)
{
    if (direction == 'S' || direction == 'E')
        return temp * (-1);
    else return temp;
}

int inverse(const int temp)
{
    if (temp == 1)
        return -1;
    else return 1;
}

int changeVal(const char direction)
{
    if (direction == 'N' || direction == 'W')
        return 1;
    else return -1;
}

char inverseDir(const char temp)
{
    if (temp == 'W')
        return 'E';
    if (temp == 'E')
        return 'W';
    if (temp == 'N')
        return 'S';
    if (temp == 'S')
        return 'N';
    return NULL;
}

char spitOutDirection(const int tempDirection)
{
    switch (tempDirection)
    {
    case 0:
        return 'N';
        break;
    case 1:
        return 'S';
        break;
    case 2:
        return 'W';
        break;
    case 3:
        return 'E';
        break;
    }
    return NULL;
}

char changeDirection(const char direction)
{
    switch (direction)
    {
    case 'N':
        return 'W';
        break;
    case 'S':
        return 'E';
        break;
    case 'E':
        return 'N';
        break;
    case 'W':
        return 'S';
        break;
    }
    return NULL;
}

void revealMap(const int player, const int yship, const int xship, const int temp, const int whichShip, const int Y, const int X, char** boardGlobal, bool undo)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (playerData[player].ships_RADAR[temp][whichShip])
            {
                if (calculatePoints(yship, xship, i, j) <= temp)
                {
                    if (undo)
                        playerData[player].board[i][j] = '?';
                    else playerData[player].board[i][j] = boardGlobal[i][j];
                }
            }
            else
            {
                if (calculatePoints(yship, xship, i, j) <= 1)
                {
                    if (undo)
                        playerData[player].board[i][j] = '?';
                    else playerData[player].board[i][j] = boardGlobal[i][j];
                }
            }
        }
    }
}

void fill(char** boardGlobal, const int Y, const int X)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (playerData[playerA].board[i][j] != '?')
            {
                playerData[playerA].board[i][j] = boardGlobal[i][j];
            }
            if (playerData[playerB].board[i][j] != '?')
            {
                playerData[playerB].board[i][j] = boardGlobal[i][j];
            }
        }
    }
}                                                                                                                                                                               // END OF USEFUL FUNCTIONALITIES

char printPartOfTheShip(char** boardGlobal, int y, int x, const int player)                                                                                                     // PRINTING AREA
{
    if (boardGlobal[y][x] == '+')
    {
        for (int p = playerA; p <= playerB; p++)
        {
            if (player != blankValue)
                p = player;
            for (int k = DES; k <= CAR; k++)
            {
                for (int l = 0; l < MAX_SHIPS; l++)
                {
                    if (playerData[p].whichShips[k][l])
                    {
                        if (playerData[p].ships_ENGINE_Y[k][l] == y && playerData[p].ships_ENGINE_X[k][l] == x)
                        {
                            return '%';
                        }
                        else if (playerData[p].ships_CANNON_Y[k][l] == y && playerData[p].ships_CANNON_X[k][l] == x)
                        {
                            return '!';
                        }
                        else if (playerData[p].ships_Y[k][l] == y && playerData[p].ships_X[k][l] == x)
                        {
                            return '@';
                        }
                    }
                }
            }
            if (player != blankValue)
                return playerData[p].board[y][x];
        }
        return boardGlobal[y][x];
    }
    else if (player != blankValue)
        return playerData[player].board[y][x];
    else return boardGlobal[y][x];
}

void extendedPrinting(char** boardGlobal, const int Y, const int X, const int player)
{
    int temp = '0' - 1;
    if (X > 10)
    {
        printf("  ");
        for (int i = 0; i < X; i++)
        {
            if (i % 10 == 0)
                temp++;
            printf("%c", char(temp));
        }
        printf("\n");
    }
    printf("  ");
    temp = '0' - 1;
    for (int i = 0; i < X; i++)
    {
        printf("%d", i % 10);
    }
    printf("\n");
    for (int i = 0; i < Y; i++)
    {
        if (i % 10 == 0)
            temp++;
        printf("%c%d", char(temp), i % 10);
        for (int j = 0; j < X; j++)
        {
            if (player == blankValue)
                printf("%c", printPartOfTheShip(boardGlobal, i, j, blankValue));
            else
            {
                fill(boardGlobal, Y, X);
                printf("%c", printPartOfTheShip(boardGlobal, i, j, player));
            }
        }
        printf("\n");
    }
    printf("PARTS REMAINING:: A : %d B : %d\n", playerData[playerA].parts, playerData[playerB].parts);
}

void printGlobal(char** boardGlobal, const int Y, const int X)
{
    int version;
    cin >> version;
    if (version == 0)
    {
        for (int i = 0; i < Y; i++)
        {
            for (int j = 0; j < X; j++)
            {
                printf("%c", boardGlobal[i][j]);
            }
            printf("\n");
        }
        printf("PARTS REMAINING:: A : %d B : %d\n", playerData[playerA].parts, playerData[playerB].parts);
    }
    else if (version == 1)
        extendedPrinting(boardGlobal, Y, X, blankValue);
}

void printPlayers(const int player, const int Y, const int X, char** boardGlobal)
{
    fill(boardGlobal, Y, X);
    int version;
    cin >> version;
    if (version == 0)
    {
        for (int i = 0; i < Y; i++)
        {
            for (int j = 0; j < X; j++)
            {
                if (player == playerA)
                    printf("%c", playerData[playerA].board[i][j]);
                else printf("%c", playerData[playerB].board[i][j]);
            }
            printf("\n");
        }
    }
    else extendedPrinting(boardGlobal, Y, X, player);
}                                                                                                                                                                               // END OF PRINTING AREA

bool borderValidation(const int yship, const int xship, const int temp, const char direction, const int player)                                                                 // VALIDATION AREA
{
    switch (direction)
    {
    case 'N':
        if ((yship + temp - 1 > playerData[player].y2) || (yship < playerData[player].y1) || (xship < playerData[player].x1) || (xship > playerData[player].x2))
            return 1;
        break;
    case 'S':
        if ((yship > playerData[player].y2) || (yship - temp + 1 < playerData[player].y1) || (xship < playerData[player].x1) || (xship > playerData[player].x2))
            return 1;
        break;
    case 'W':
        if ((xship < playerData[player].x1) || (xship + temp - 1 > playerData[player].x2) || (yship < playerData[player].y1) || (yship > playerData[player].y2))
            return 1;
        break;
    case 'E':
        if ((xship > playerData[player].x2) || (xship - temp + 1 < playerData[player].x1) || (yship < playerData[player].y1) || (yship > playerData[player].y2))
            return 1;
        break;
    }
    return 0;
}

bool reefValidation(const int temp, int yship, int xship, char** boardGlobal, const char direction)
{
    for (int i = 0; i < temp; i++)
    {
        if (boardGlobal[yship][xship] == '#')
        {
            return true;
        }
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
    return false;
}

bool tooCloseValidation(const int temp, int yship, int xship, const char direction, char** boardValidator)
{
    yship++;
    xship++;
    for (int i = 0; i < temp; i++)
    {
        int countShips = 0;
        for (int p = yship - 1; p <= yship + 1; p++)
        {
            for (int q = xship - 1; q <= xship + 1; q++)
            {
                if (boardValidator[p][q] == playerA || boardValidator[p][q] == playerB)
                {
                    countShips++;
                }
            }
        }
        if (countShips != 0)
        {
            return true;
        }
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
    return false;
}

bool validationErrorsPlaceForSHIP(const int yship, const int xship, const int temp, const char direction, const int player, const char command[], const char playerDisplay, const int whichShip, const char ship[], const char construction[], char** boardGlobal, char** boardValidator)
{
    if (borderValidation(yship, xship, temp, direction, player))
    {
        printf("INVALID OPERATION %c%s %c %d %d %c %d %s %s%c: NOT IN STARTING POSITION", '"', command, playerDisplay, yship, xship, direction, whichShip, ship, construction, '"');
        return 1;
    }
    if (reefValidation(temp, yship, xship, boardGlobal, direction))
    {
        printf("INVALID OPERATION %c%s %c %d %d %c %d %s %s%c: PLACING SHIP ON REEF", '"', command, playerDisplay, yship, xship, direction, whichShip, ship, construction, '"');
        return 1;
    }
    if (tooCloseValidation(temp, yship, xship, direction, boardValidator))
    {
        printf("INVALID OPERATION %c%s %c %d %d %c %d %s %s%c: PLACING SHIP TOO CLOSE TO OTHER SHIP", '"', command, playerDisplay, yship, xship, direction, whichShip, ship, construction, '"');
        return 1;
    }
    if (playerData[player].fleet[temp] <= 0)
    {
        printf("INVALID OPERATION %c%s %c %d %d %c %d %s %s%c: ALL SHIPS OF THE CLASS ALREADY SET", '"', command, playerDisplay, yship, xship, direction, whichShip, ship, construction, '"');
        return 1;
    }
    return 0;
}

bool validationErrorsPlace(const int yship, const int xship, const int temp, const char direction, const int player, const char command[], const int whichShip, const char ship[], char** boardGlobal, char** boardValidator, const bool constr, const char construction[], const bool playerIsAi)
{
    char playerDisplay;
    if (player == playerA)
    {
        playerDisplay = 'A';
    }
    else playerDisplay = 'B';
    if (constr && validationErrorsPlaceForSHIP(yship, xship, temp, direction, player, command, playerDisplay, whichShip, ship, construction, boardGlobal, boardValidator))
    {
        return 1;
    }
    if (playerData[player].whichShips[temp][whichShip] && playerIsAi == false)
    {
        printf("INVALID OPERATION %c%s %d %d %c %d %s%c: SHIP ALREADY PRESENT", '"', command, yship, xship, direction, whichShip, ship, '"');
        return 1;
    }
    else if (playerIsAi == false)
    {
        playerData[player].whichShips[temp][whichShip] = 1;
    }
    if (borderValidation(yship, xship, temp, direction, player) && playerIsAi == false)
    {
        printf("INVALID OPERATION %c%s %d %d %c %d %s%c: NOT IN STARTING POSITION", '"', command, yship, xship, direction, whichShip, ship, '"');
        return 1;
    }
    else if (borderValidation(yship, xship, temp, direction, player))
    {
        return 1;
    }
    if (reefValidation(temp, yship, xship, boardGlobal, direction) && playerIsAi == false)
    {
        printf("INVALID OPERATION %c%s %d %d %c %d %s%c: PLACING SHIP ON REEF", '"', command, yship, xship, direction, whichShip, ship, '"');
        return 1;
    }
    else if (reefValidation(temp, yship, xship, boardGlobal, direction))
    {
        return 1;
    }
    if (tooCloseValidation(temp, yship, xship, direction, boardValidator) && playerIsAi == false)
    {
        printf("INVALID OPERATION %c%s %d %d %c %d %s%c: PLACING SHIP TOO CLOSE TO OTHER SHIP", '"', command, yship, xship, direction, whichShip, ship, '"');
        return 1;
    }
    else if (tooCloseValidation(temp, yship, xship, direction, boardValidator))
    {
        return 1;
    }
    if (playerData[player].fleet[temp] <= 0 && playerIsAi == false)
    {
        printf("INVALID OPERATION %c%s %d %d %c %d %s%c: ALL SHIPS OF THE CLASS ALREADY SET", '"', command, yship, xship, direction, whichShip, ship, '"');
        return 1;
    }
    return 0;
}

bool validationErrorsMove(const int player, const int temp, const int whichShip, const char command[], const char ship[], const char whereToGo, const int Y, const int X, const int countMoves[][MAX_SHIPS + 1], char** boardGlobal, char** boardValidator)
{
    for (int i = DES; i <= CAR; i++)
    {
        if (countMoves[i][whichShip] < 0)
        {
            printf("INVALID OPERATION %c%s %d %s %c%c: SHIP MOVED ALREADY", '"', command, whichShip, ship, whereToGo, '"');
            return 1;
        }
    }
    if (playerData[player].ships_ENGINE[temp][whichShip] == 0)
    {
        printf("INVALID OPERATION %c%s %d %s %c%c: SHIP CANNOT MOVE", '"', command, whichShip, ship, whereToGo, '"');
        return 1;
    }
    if (playerData[player].ships_Y[temp][whichShip] < 0 || playerData[player].ships_Y[temp][whichShip] >= Y || playerData[player].ships_X[temp][whichShip] < 0 || playerData[player].ships_X[temp][whichShip] >= X)
    {
        printf("INVALID OPERATION %c%s %d %s %c%c: SHIP WENT FROM BOARD", '"', command, whichShip, ship, whereToGo, '"');
        return 1;
    }
    if (reefValidation(temp, playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], boardGlobal, playerData[player].ships_D[temp][whichShip]))
    {
        printf("INVALID OPERATION %c%s %d %s %c%c: PLACING SHIP ON REEF", '"', command, whichShip, ship, whereToGo, '"');
        return 1;
    }
    if (tooCloseValidation(temp, playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], playerData[player].ships_D[temp][whichShip], boardValidator))
    {
        printf("INVALID OPERATION %c%s %d %s %c%c: PLACING SHIP TOO CLOSE TO OTHER SHIP", '"', command, whichShip, ship, whereToGo, '"');
        return 1;
    }
    return 0;
}

bool extended_shootingValidation(const int player, const int temp, const int whichShip, const int yshot, const int xshot, const int range, const char ship[], const char command[], const int countShots[][MAX_SHIPS + 1])
{
    if (calculatePoints(playerData[player].ships_CANNON_Y[temp][whichShip], playerData[player].ships_CANNON_X[temp][whichShip], yshot, xshot) > range)
    {
        printf("INVALID OPERATION %c%s %d %s %d %d%c: SHOOTING TOO FAR", '"', command, whichShip, ship, yshot, xshot, '"');
        return 1;
    }
    if (playerData[player].ships_CANNON[temp][whichShip] == 0)
    {
        printf("INVALID OPERATION %c%s %d %s %d %d%c: SHIP CANNOT SHOOT", '"', command, whichShip, ship, yshot, xshot, '"');
        return 1;
    }
    if (countShots[temp][whichShip] > temp)
    {
        printf("INVALID OPERATION %c%s %d %s %d %d%c: TOO MANY SHOOTS", '"', command, whichShip, ship, yshot, xshot, '"');
        return 1;
    }
    return 0;
}

bool shootingValidation(const char command[], const int yshot, const int xshot, const int Y, const int X)
{
    if (playerData[playerA].fleet[CAR] != 0 || playerData[playerA].fleet[BAT] != 0 || playerData[playerA].fleet[CRU] != 0 || playerData[playerA].fleet[DES] != 0 || playerData[playerB].fleet[CAR] != 0 || playerData[playerB].fleet[BAT] != 0 || playerData[playerB].fleet[CRU] != 0 || playerData[playerB].fleet[DES] != 0)
    {
        printf("INVALID OPERATION %c%s %d %d%c: NOT ALL SHIPS PLACED", '"', command, yshot, xshot, '"');
        return true;
    }
    if (yshot < 0 || yshot >= Y || xshot < 0 || xshot >= X)
    {
        printf("INVALID OPERATION %c%s %d %d%c: FIELD DOES NOT EXIST", '"', command, yshot, xshot, '"');
        return true;
    }
    return false;
}                                                                                                                                                                                           // END OF VALIDATION AREA

void globalShipPlacement(char** boardGlobal, const char construction[], const int temp, const int whichShip, const int yship, const int xship, const int player, const int i)               // PLACE AREA
{
    if (construction[i] == '0')
    {
        if (i == 0)
            playerData[player].ships_RADAR[temp][whichShip] = 0;
        if (i == (temp - 1))
            playerData[player].ships_ENGINE[temp][whichShip] = 0;
        if (i == 1)
            playerData[player].ships_CANNON[temp][whichShip] = 0;
        boardGlobal[yship][xship] = 'x';
        playerData[player].board[yship][xship] = 'x';
    }
    else
    {
        boardGlobal[yship][xship] = '+';
        playerData[player].board[yship][xship] = '+';
    }
}

void place_ship(const char command[], char** boardGlobal, const int player, char** boardValidator, const int Y, const int X, const bool shipCommand)
{
    char construction[6];
    int temp = 0, yship, xship, whichShip;
    char ship[256], direction;
    cin >> yship >> xship >> direction >> whichShip >> ship;
    temp = defineClassOfTheShip(ship);
    playerData[player].parts += temp;
    if (shipCommand)
    {
        int count = 0, i = 0;
        cin >> construction;
        while (construction[i] != '\0')
        {
            if (construction[i] == '0')
                count++;
            i++;
        }
        playerData[player].parts -= count;
    }
    if (validationErrorsPlace(yship, xship, temp, direction, player, command, whichShip, ship, boardGlobal, boardValidator, shipCommand, construction, false))
    {
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    playerData[player].ships_Y[temp][whichShip] = yship;
    playerData[player].ships_X[temp][whichShip] = xship;
    playerData[player].ships_D[temp][whichShip] = direction;
    playerData[player].fleet[temp]--;
    for (int i = 0; i < temp; i++)
    {
        boardValidator[yship + 1][xship + 1] = char(player);
        if (shipCommand)
            globalShipPlacement(boardGlobal, construction, temp, whichShip, yship, xship, player, i);
        else
        {
            boardGlobal[yship][xship] = '+';
        }
        if (i == (temp - 1))
        {
            playerData[player].ships_ENGINE_Y[temp][whichShip] = yship;
            playerData[player].ships_ENGINE_X[temp][whichShip] = xship;
            if (temp == DES)
            {
                playerData[player].ships_CANNON_Y[temp][whichShip] = yship;
                playerData[player].ships_CANNON_X[temp][whichShip] = xship;
            }
        }
        else if (i == 1)
        {
            playerData[player].ships_CANNON_Y[temp][whichShip] = yship;
            playerData[player].ships_CANNON_X[temp][whichShip] = xship;
        }
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
    revealMap(player, playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], temp, whichShip, Y, X, boardGlobal, false);
    fill(boardGlobal, Y, X);
}                                                                                                                                                                           // END OF THE PLACE AREA

void move_ship(int yship, int xship, const char direction, const int temp, char** boardGlobal, char** boardValidator, bool stateOfShip[], const int player)                                   // MOVE AREA
{
    for (int i = 0; i < temp; i++)
    {
        boardValidator[yship + 1][xship + 1] = char(player);
        if (stateOfShip[i])
            boardGlobal[yship][xship] = '+';
        else
            boardGlobal[yship][xship] = 'x';
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
}

void delete_ship(int yship, int xship, const char direction, const int temp, char** boardGlobal, char** boardValidator, bool stateOfShip[])
{
    for (int i = 0; i < temp; i++)
    {
        if (boardGlobal[yship][xship] == 'x')
            stateOfShip[i] = 0;
        boardValidator[yship + 1][xship + 1] = '0';
        boardGlobal[yship][xship] = ' ';
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
}

void changeCannonEngineLocation(const int temp, const int whichShip, const int player)
{
    if (playerData[player].ships_D[temp][whichShip] == 'N' || playerData[player].ships_D[temp][whichShip] == 'S')
    {
        playerData[player].ships_CANNON_Y[temp][whichShip] = playerData[player].ships_Y[temp][whichShip] + changeVal(playerData[player].ships_D[temp][whichShip]);
        playerData[player].ships_CANNON_X[temp][whichShip] = playerData[player].ships_X[temp][whichShip];
        playerData[player].ships_ENGINE_Y[temp][whichShip] = playerData[player].ships_Y[temp][whichShip] + (changeTemp(temp, playerData[player].ships_D[temp][whichShip]) + inverse(changeVal(playerData[player].ships_D[temp][whichShip])));
        playerData[player].ships_ENGINE_X[temp][whichShip] = playerData[player].ships_X[temp][whichShip];
    }
    else
    {
        playerData[player].ships_CANNON_Y[temp][whichShip] = playerData[player].ships_Y[temp][whichShip];
        playerData[player].ships_CANNON_X[temp][whichShip] = playerData[player].ships_X[temp][whichShip] + changeVal(playerData[player].ships_D[temp][whichShip]);
        playerData[player].ships_ENGINE_Y[temp][whichShip] = playerData[player].ships_Y[temp][whichShip];
        playerData[player].ships_ENGINE_X[temp][whichShip] = playerData[player].ships_X[temp][whichShip] + (changeTemp(temp, playerData[player].ships_D[temp][whichShip]) + inverse(changeVal(playerData[player].ships_D[temp][whichShip])));

    }
}

void changeShipLocation(const char whereToGo, const int temp, const int whichShip, const int player)
{
    switch (whereToGo)
    {
    case 'F':
        if (playerData[player].ships_D[temp][whichShip] == 'N' || playerData[player].ships_D[temp][whichShip] == 'S')
            playerData[player].ships_Y[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        else playerData[player].ships_X[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        break;
    case 'L':
        if (playerData[player].ships_D[temp][whichShip] == 'N' || playerData[player].ships_D[temp][whichShip] == 'S')
        {
            playerData[player].ships_Y[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
            for (int i = 0; i < temp - 1; i++)
                playerData[player].ships_X[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        }
        else
        {
            for (int i = 0; i < temp - 1; i++)
                playerData[player].ships_Y[temp][whichShip] += changeVal(playerData[player].ships_D[temp][whichShip]);
            playerData[player].ships_X[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        }
        playerData[player].ships_D[temp][whichShip] = changeDirection(playerData[player].ships_D[temp][whichShip]);
        break;
    case 'R':
        if (playerData[player].ships_D[temp][whichShip] == 'W' || playerData[player].ships_D[temp][whichShip] == 'E')
        {
            playerData[player].ships_X[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
            for (int i = 0; i < temp - 1; i++)
                playerData[player].ships_Y[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        }
        else
        {
            for (int i = 0; i < temp - 1; i++)
                playerData[player].ships_X[temp][whichShip] += changeVal(playerData[player].ships_D[temp][whichShip]);
            playerData[player].ships_Y[temp][whichShip] += inverse(changeVal(playerData[player].ships_D[temp][whichShip]));
        }
        playerData[player].ships_D[temp][whichShip] = inverseDir(changeDirection(playerData[player].ships_D[temp][whichShip]));
        break;
    }
}

void moveMain(const char command[], const int player, char** boardGlobal, char** boardValidator, int countMoves[][MAX_SHIPS + 1], const int Y, const int X)
{
    bool stateOfShip[CAR] = { 1,1,1,1,1 };
    int temp, whichShip;
    char ship[10], whereToGo; // MOZLIWY ERROR
    cin >> whichShip >> ship >> whereToGo;
    temp = defineClassOfTheShip(ship);
    countMoves[temp][whichShip]--;
    revealMap(player, playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], temp, whichShip, Y, X, boardGlobal, true);
    delete_ship(playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], playerData[player].ships_D[temp][whichShip], temp, boardGlobal, boardValidator, stateOfShip);
    changeShipLocation(whereToGo, temp, whichShip, player);
    if (validationErrorsMove(player, temp, whichShip, command, ship, whereToGo, Y, X, countMoves, boardGlobal, boardValidator))
    {
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    changeCannonEngineLocation(temp, whichShip, player);
    move_ship(playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], playerData[player].ships_D[temp][whichShip], temp, boardGlobal, boardValidator, stateOfShip, player);
    revealMap(player, playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], temp, whichShip, Y, X, boardGlobal, false);
    fill(boardGlobal, Y, X);
}                                                                                                                                                                               // END OF MOVE AREA

void engineCannonRadar_State(const int yshot, const int xshot, const int player)                                                                                                // SHOOTING AND SPY PLANES AREA
{
    for (int i = 0; i < CAR + 1; i++)
    {
        for (int j = 0; j < MAX_SHIPS + 1; j++)
        {
            if ((yshot == playerData[player].ships_CANNON_Y[i][j]) && (xshot == playerData[player].ships_CANNON_X[i][j]))
            {
                playerData[player].ships_CANNON[i][j] = 0;
            }
            if ((yshot == playerData[player].ships_ENGINE_Y[i][j]) && (xshot == playerData[player].ships_ENGINE_X[i][j]))
            {
                playerData[player].ships_ENGINE[i][j] = 0;
            }
            if ((yshot == playerData[player].ships_Y[i][j]) && (xshot == playerData[player].ships_X[i][j]))
            {
                playerData[player].ships_RADAR[i][j] = 0;
            }
        }
    }
}

void extended_shooting(int player, char** boardGlobal, char** boardValidator, const int Y, const char command[], int countShots[][MAX_SHIPS + 1])
{
    int whichShip;
    char ship[256];
    int yshot, xshot, temp = 0, range = 0;
    cin >> whichShip >> ship >> yshot >> xshot;
    temp = defineClassOfTheShip(ship);
    if (temp == CAR)
        range = infiniteRange;
    else range = temp;
    countShots[temp][whichShip]++;
    if (extended_shootingValidation(player, temp, whichShip, yshot, xshot, range, ship, command, countShots))
    {
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    else
    {
        player = inversePlayer(player);
        if (boardGlobal[yshot][xshot] == '+')
        {
            boardGlobal[yshot][xshot] = 'x';
            engineCannonRadar_State(yshot, xshot, player);
            playerData[player].parts--;
            checkWinCondition(player, boardGlobal, boardValidator, Y);
        }
    }
}

void sendPlane(const char command[], const int player, char** boardGlobal, int countShots[][MAX_SHIPS + 1], char** boardValidator, const int Y, const int X)
{
    int whichShip, yPlane, xPlane;
    cin >> whichShip >> yPlane >> xPlane;
    countShots[CAR][whichShip]++;
    if (playerData[player].ships_CANNON[CAR][whichShip] == 0)
    {
        printf("INVALID OPERATION %c%s%d %d %d%c: CANNOT SEND PLANE", '"', command, whichShip, yPlane, xPlane, '"');
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    if (countShots[CAR][whichShip] > CAR)
    {
        printf("INVALID OPERATION %c%s %d %d %d%c: ALL PLANES SENT", '"', command, whichShip, yPlane, xPlane, '"');
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    for (int i = yPlane - 1; i <= yPlane + 1; i++)
    {
        for (int j = xPlane - 1; j <= xPlane + 1; j++)
        {
            if (i >= 0 && i < Y && j >= 0 && j < X)
            {
                playerData[playerA].board[i][j] = boardGlobal[i][j];
            }
        }
    }
}

void shoot(const char command[], char** boardGlobal, int player, const int Y, const int X, char** boardValidator)
{
    int yshot, xshot;
    cin >> yshot >> xshot;
    if (shootingValidation(command, yshot, xshot, Y, X))
    {
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    if (boardGlobal[yshot][xshot] == '+')
    {
        boardGlobal[yshot][xshot] = 'x';
        playerData[player].parts--;
        checkWinCondition(player, boardGlobal, boardValidator, Y);
    }
}                                                                                                                                                                   // END OF SHOOTING AND SPYPLANES AREA

void set_fleet(char** boardGlobal, char** boardValidator, const int Y)                                                                                              // [state] COMMANDS AND DEFAULT PARAMETERS
{
    int player;
    char who;
    cin >> who;
    player = definePlayer(who);
    for (int i = 5; i > 1; i--)
        cin >> playerData[player].fleet[i];
    if ((playerData[player].fleet[CAR] > MAX_SHIPS) || (playerData[player].fleet[BAT] > MAX_SHIPS) || (playerData[player].fleet[CRU] > MAX_SHIPS) || (playerData[player].fleet[DES] > MAX_SHIPS))
    {
        printf("TOO MANY SHIPS\n");
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }

}

void reefInitialization(char command[], char** boardGlobal, char** boardValidator, const int Y, const int X)
{
    int yReef, xReef;
    cin >> yReef >> xReef;
    if (yReef < 0 || yReef >= Y || xReef < 0 || xReef >= X)
    {
        printf("INVALID OPERATION %c%s %d %d%c: REEF IS NOT PLACED ON BOARD", '"', command, yReef, xReef, '"');
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    else
    {
        boardGlobal[yReef][xReef] = '#';
        boardValidator[yReef + 1][xReef + 1] = '#';
    }
}

void initPosition()
{
    char player;
    cin >> player;
    switch (player)
    {
    case 'A':
        cin >> playerData[playerA].y1;
        cin >> playerData[playerA].x1;
        cin >> playerData[playerA].y2;
        cin >> playerData[playerA].x2;
        break;
    case 'B':
        cin >> playerData[playerB].y1;
        cin >> playerData[playerB].x1;
        cin >> playerData[playerB].y2;
        cin >> playerData[playerB].x2;
        break;
    }
}

int nextPlayer()
{
    char player;
    cin >> player;
    switch (player)
    {
    case 'A':
        return playerA;
        break;
    case 'B':
        return playerB;
        break;
    }
    return NULL;
}

void printShipInformationForSave(const int player, const int j, const int k, char** boardGlobal, const char shipName[])                                                                     // SAVE
{
    int yTemp = 0;
    int xTemp = 0;
    printf("%d %d %c %d %c%c%c ", playerData[player].ships_Y[j][k], playerData[player].ships_X[j][k], playerData[player].ships_D[j][k], k, shipName[0], shipName[1], shipName[2]);
    yTemp = playerData[player].ships_Y[j][k];
    xTemp = playerData[player].ships_X[j][k];
    for (int l = 0; l < j; l++)
    {
        if (boardGlobal[yTemp][xTemp] == 'x')
        {
            printf("0");
        }
        else printf("1");
        if (playerData[player].ships_D[j][k] == 'N' || playerData[player].ships_D[j][k] == 'S')
            yTemp += changeVal(playerData[player].ships_D[j][k]);
        else xTemp += changeVal(playerData[player].ships_D[j][k]);
    }
}

void printSave(char** boardGlobal, const int player)
{
    char displayPlayer;
    if (player == 0)
    {
        displayPlayer = 'A';
    }
    else displayPlayer = 'B';
    printf("INIT_POSITION %c %d %d %d %d\n", displayPlayer, playerData[player].y1, playerData[player].x1, playerData[player].y2, playerData[player].x2);
    printf("SET_FLEET %c ", displayPlayer);
    for (int i = CAR; i >= DES; i--)
    {
        int count = 0;
        for (int j = 0; j <= MAX_SHIPS; j++)
        {
            if (playerData[player].whichShips[i][j] == 1)
            {
                count++;
            }
        }
        printf("%d ", count);
    }
    printf("\n");
    for (int j = CAR; j >= DES; j--)
    {
        for (int k = 0; k <= MAX_SHIPS; k++)
        {
            if (playerData[player].whichShips[j][k] == 1)
            {
                char shipName[3];
                printf("SHIP %c ", displayPlayer);
                nameTheShip(shipName, j);
                printShipInformationForSave(player, j, k, boardGlobal, shipName);
                printf("\n");
            }
        }
    }
}                                                                                                                                               // END OF SAVE SECTION

void saveModule(const int Y, const int X, const bool whichPlayer, char** boardGlobal)
{
    printf("[state]\n");
    printf("BOARD_SIZE %d %d\n", Y, X);
    if (whichPlayer == 0)
        printf("NEXT_PLAYER A\n");
    else printf("NEXT_PLAYER B\n");
    for (int i = 0; i < Y; i++)
        for (int j = 0; j < X; j++)
            if (boardGlobal[i][j] == '#')
            {
                printf("REEF %d %d\n", i, j);
            }
    printSave(boardGlobal, playerA);
    printSave(boardGlobal, playerB);
    printf("[state]\n");
}

void defaultParameters(char** boardGlobal, char** boardValidator, const int Y, const int X)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            boardGlobal[i][j] = ' ';
        }
    }
    for (int i = 0; i < Y + 2; i++)
    {
        for (int j = 0; j < X + 2; j++)
        {
            boardValidator[i][j] = '0';
        }
    }
    playerData[playerA].x1 = 0;
    playerData[playerA].x2 = X - 1;
    playerData[playerA].y1 = 0;
    playerData[playerA].y2 = Y / 2 - 1; // border A
    playerData[playerB].x1 = 0;
    playerData[playerB].x2 = X - 1;
    playerData[playerB].y1 = Y / 2 + 1; // border B
    playerData[playerB].y2 = Y - 1;
    for (int p = playerA; p <= playerB; p++)
    {
        for (int i = DES; i <= CAR; i++)
        {
            for (int j = 0; j < MAX_SHIPS; j++)
            {
                playerData[p].whichShips[i][j] = 0;
                playerData[p].ships_CANNON[i][j] = 1;
                playerData[p].ships_ENGINE[i][j] = 1;
                playerData[p].ships_RADAR[i][j] = 1;
            }
        }
        playerData[p].board = new char* [Y];
        for (int i = 0; i < Y; i++)
        {
            playerData[p].board[i] = new char[X];
        }
        for (int i = 0; i < Y; i++)
        {
            for (int j = 0; j < X; j++)
            {
                playerData[p].board[i][j] = '?';
            }
        }
    }
}

void changeCoordinates_AI(int* yship, int* xship, const char whereToGo, char* direction, const int temp)
{
    switch (whereToGo)
    {
    case 'F':
        if (*direction == 'N' || *direction == 'S')
            *yship += inverse(changeVal(*direction));
        else *xship += inverse(changeVal(*direction));
        break;
    case 'L':
        if (*direction == 'N' || *direction == 'S')
        {
            *yship += inverse(changeVal(*direction));
            for (int i = 0; i < temp - 1; i++)
                *xship += inverse(changeVal(*direction));
        }
        else
        {
            for (int i = 0; i < temp - 1; i++)
                *yship += changeVal(*direction);
            *xship += inverse(changeVal(*direction));
        }
        *direction = changeDirection(*direction);
        break;
    case 'R':
        if (*direction == 'W' || *direction == 'E')
        {
            *xship += inverse(changeVal(*direction));
            for (int i = 0; i < temp - 1; i++)
                *yship += inverse(changeVal(*direction));
        }
        else
        {
            for (int i = 0; i < temp - 1; i++)
                *xship += changeVal(*direction);
            *yship += inverse(changeVal(*direction));
        }
        *direction = inverseDir(changeDirection(*direction));
        break;
    }
}

void place_ship_AI(int yship, int xship, const char direction, const int temp, char** currentStateOfGame, const int player, char** currentValidator)
{
    for (int i = 0; i < temp; i++)
    {
        currentValidator[yship + 1][xship + 1] = char(player);
        currentStateOfGame[yship][xship] = '+';
        if (direction == 'N' || direction == 'S')
            yship += changeVal(direction);
        else
            xship += changeVal(direction);
    }
}

void AImoving(char** currentStateOfGame, char** currentValidator, const int temp, const int whichShip, int countMoves[][MAX_SHIPS + 1], int SEED, const int player, const int Y, const int X)
{
    int yship = playerData[player].ships_Y[temp][whichShip], xship = playerData[player].ships_X[temp][whichShip];
    char shipName[10], whereToGo = '0', dship = playerData[player].ships_D[temp][whichShip];
    bool possibleMove[3] = { 1,1,1 }, stateOfShip[CAR] = { 1,1,1,1,1 };
    nameTheShip(shipName, temp);
    if (playerData[player].ships_ENGINE[temp][whichShip] == false)
    {
        countMoves[temp][whichShip]--;
        return;
    }
    while (true)
    {
        srand(SEED);
        int tempWhere = rand() % 3;
        if (possibleMove[tempWhere])
        {
            switch (tempWhere)
            {
            case 0:
                whereToGo = 'F';
                break;
            case 1:
                whereToGo = 'L';
                break;
            case 2:
                whereToGo = 'R';
                break;
            }
            delete_ship(yship, xship, dship, temp, currentStateOfGame, currentValidator, stateOfShip);
            changeCoordinates_AI(&yship, &xship, whereToGo, &dship, temp);
            if (yship >= 0 && yship < Y && xship >= 0 && xship < X && reefValidation(temp, yship, xship, currentStateOfGame, dship) == false && tooCloseValidation(temp, yship, xship, dship, currentValidator) == false)
            {
                printf("MOVE %d %s %c\n", whichShip, shipName, whereToGo);
                place_ship_AI(yship, xship, dship, temp, currentStateOfGame, player, currentValidator);
                countMoves[temp][whichShip]--;
                break;
            }
            else {
                place_ship_AI(playerData[player].ships_Y[temp][whichShip], playerData[player].ships_X[temp][whichShip], playerData[player].ships_D[temp][whichShip], temp, currentStateOfGame, player, currentValidator);
                yship = playerData[player].ships_Y[temp][whichShip];
                xship = playerData[player].ships_X[temp][whichShip];
                dship = playerData[player].ships_D[temp][whichShip];
                possibleMove[tempWhere] = 0;
            }
            if (possibleMove[0] == false && possibleMove[1] == false && possibleMove[2] == false)
            {
                countMoves[temp][whichShip]--;
                break;
            }
            SEED++;
        }
        else
        {
            countMoves[temp][whichShip]--;
            break;
        }
    }
}

bool shootingAtOwnUnitsOrReefs(const int player, char** currentValidator, const int yshot, const int xshot)
{
    if (currentValidator[yshot + 1][xshot + 1] == player || currentValidator[yshot + 1][xshot + 1] == '#')
        return true;
    else return false;
}

bool enemyVisibleOnRadar(const int player, const int Y, const int X, int *yshot, int *xshot, char** currentValidator, char** currentStateOfGame, const int temp, const int whichShip, const int range)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            if (currentStateOfGame[i][j] == '+' && playerData[player].board[i][j] != '?' && calculatePoints(playerData[player].ships_CANNON_Y[temp][whichShip], playerData[player].ships_CANNON_X[temp][whichShip], i, j) <= range && shootingAtOwnUnitsOrReefs(player, currentValidator, i, j) == false)
            {
                *yshot = i;
                *xshot = j;
                return true;
            }
        }
    }
    return false;
}

void AIshootingBasic(const int SEED, const int player, const int Y, const int X, char** currentValidator, char** currentStateOfGame, const int temp, const int whichShip)
{
    srand(SEED);
    int yshot, xshot;
    while (true)
    {
        if (enemyVisibleOnRadar(player, Y, X, &yshot, &xshot, currentValidator, currentStateOfGame, temp, whichShip, infiniteRange))
            break;
        yshot = rand() % Y;
        xshot = rand() % X;
        if (shootingAtOwnUnitsOrReefs(player, currentValidator, yshot, xshot) == false)
            break;
    }
    printf("SHOOT %d %d\n", yshot, xshot);
    if (currentStateOfGame[yshot][xshot] == '+')
        currentStateOfGame[yshot][xshot] = 'x';
}

void AIshootingExtended(const int shipLength, const int whichShip, const int player, char** currentStateOfGame, char** currentValidator, const int Y, const int X, int countShots[][MAX_SHIPS + 1], const int SEED)
{
    srand(SEED);
    char shipName[10];
    int range = shipLength, yshot = 0, xshot = 0;
    nameTheShip(shipName, shipLength);
    if (range == CAR)
        range = infiniteRange;
    if (playerData[player].ships_CANNON[shipLength][whichShip] == false)
    {
        countShots[shipLength][whichShip]++;
        return;
    }
    else if(enemyVisibleOnRadar(player, Y, X, &yshot, &xshot, currentValidator, currentStateOfGame, shipLength, whichShip, range) == false)
    {
        do
        {
            yshot = rand() % Y;
            xshot = rand() % X;
        } while (calculatePoints(playerData[player].ships_CANNON_Y[shipLength][whichShip], playerData[player].ships_CANNON_X[shipLength][whichShip], yshot, xshot) > range || shootingAtOwnUnitsOrReefs(player, currentValidator, yshot, xshot) == true);
    }
    printf("SHOOT %d %s %d %d\n", whichShip, shipName, yshot, xshot);
    countShots[shipLength][whichShip]++;
    if (currentStateOfGame[yshot][xshot] == '+')
        currentStateOfGame[yshot][xshot] = 'x';
}

void AIplacing(const int shipLength, const int player, int AI_FLEET[], char** currentStateOfGame, char** currentValidator, const int SEED)
{
    srand(SEED);
    char shipName[10];
    nameTheShip(shipName, shipLength);
    while (AI_FLEET[shipLength] > 0)
    {
        int yShip = rand() % (playerData[player].y2 + 1 - playerData[player].y1) + playerData[player].y1;
        int xShip = rand() % (playerData[player].x2 + 1 - playerData[player].x1) + playerData[player].x1;
        int tempDirection = rand() % 4;
        int whichShip = AI_FLEET[shipLength] - 1;
        if (validationErrorsPlace(yShip, xShip, shipLength, spitOutDirection(tempDirection), player, shipName, whichShip, shipName, currentStateOfGame, currentValidator, false, shipName, true) == false)
        {
            printf("PLACE_SHIP %d %d %c %d %s\n", yShip, xShip, spitOutDirection(tempDirection), whichShip, shipName);
            place_ship_AI(yShip, xShip, spitOutDirection(tempDirection), shipLength, currentStateOfGame, player, currentValidator);
            AI_FLEET[shipLength]--;
            if (AI_FLEET[DES] <= 0 && AI_FLEET[CRU] <= 0 && AI_FLEET[BAT] <= 0 && AI_FLEET[CAR] <= 0)
            {
                playerData[player].shipsAlreadyPlaced = true;
            }
        }
    }
}

void insertCurrentStateOfGame(char** currentStateOfGame, char** currentValidator, char** boardGlobal, char** boardValidator, const int Y, const int X)
{
    for (int i = 0; i < Y; i++)
    {
        for (int j = 0; j < X; j++)
        {
            currentStateOfGame[i][j] = boardGlobal[i][j];
        }
    }
    for (int i = 0; i < Y + 2; i++)
    {
        for (int j = 0; j < X + 2; j++)
        {
            currentValidator[i][j] = boardValidator[i][j];
        }
    }
}

void AImain(const int player, char** boardGlobal, char** boardValidator, const int Y, const int X, int SEED, const bool EXTENDED_SHIPS)
{
    char** currentStateOfGame;
    currentStateOfGame = new char* [Y];
    for (int i = 0; i < Y; i++)
        currentStateOfGame[i] = new char[X];
    char** currentValidator;
    currentValidator = new char* [Y + 2];
    for (int i = 0; i < Y + 2; i++)
        currentValidator[i] = new char[X + 2];
    insertCurrentStateOfGame(currentStateOfGame, currentValidator, boardGlobal, boardValidator, Y, X);
    int AI_FLEET[CAR + 1], countMoves[CAR + 1][MAX_SHIPS + 1], countShots[CAR + 1][MAX_SHIPS + 1];
    for (int i = DES; i <= CAR; i++)
    {
        AI_FLEET[i] = playerData[player].fleet[i];
        for (int j = 0; j < MAX_SHIPS; j++)
        {
            countShots[i][j] = 0;
            if (i == CAR)
                countMoves[i][j] = 2;
            else countMoves[i][j] = 3;
        }
    }
    for (int i = CAR; i >= DES; i--)
    {
        if (playerData[player].shipsAlreadyPlaced == false)
        {
            AIplacing(i, player, AI_FLEET, currentStateOfGame, currentValidator, SEED);
            SEED += 5;
        }
        else if (EXTENDED_SHIPS == false)
        {
            AIshootingBasic(SEED, player, Y, X, currentValidator, currentStateOfGame, i, 0);
            break;
        }
        else
        {
            for (int j = 0; j < MAX_SHIPS; j++)
            {
                if (playerData[player].whichShips[i][j])
                {
                    while (countShots[i][j] < i) {
                        AIshootingExtended(i, j, player, currentStateOfGame, currentValidator, Y, X, countShots, SEED);
                        SEED += 5;
                    }
                    while (countMoves[i][j] > 0)
                    {
                        AImoving(currentStateOfGame, currentValidator, i, j, countMoves, SEED, player, Y, X);
                    }
                }
            }
        }
    }
    for (int i = 0; i < Y; i++)
        delete[] currentStateOfGame[i];
    delete[] currentStateOfGame;
    for (int i = 0; i < Y + 2; i++)
        delete[] currentValidator[i];
    delete[] currentValidator;
}

void checkIfNextPlayerIsAi(char** boardGlobal, char** boardValidator, const bool whichPlayer, const int Y, const int X, const bool EXTENDED_SHIPS, int* SEED)
{
    if (playerData[playerA].playerBecameAi && whichPlayer == playerA)
    {
        printf("[playerA]\n");
        AImain(playerA, boardGlobal, boardValidator, Y, X, *SEED, EXTENDED_SHIPS);
        *SEED += 5;
        printf("[playerA]\n");
    }
    else if (playerData[playerB].playerBecameAi && whichPlayer == playerB)
    {
        printf("[playerB]\n");
        AImain(playerB, boardGlobal, boardValidator, Y, X, *SEED, EXTENDED_SHIPS);
        *SEED += 5;
        printf("[playerB]\n");
    }
}

void playerMain(char** boardGlobal, char** boardValidator, const int Y, const int X, bool whichPlayer, const bool EXTENDED_SHIPS, const bool player, int* SEED, const bool turn_on_printing)
{
    char command[20];
    int countMoves[CAR + 1][MAX_SHIPS + 1];
    int countShots[CAR + 1][MAX_SHIPS + 1];
    for (int i = 0; i < CAR + 1; i++)
    {
        for (int j = 0; j < MAX_SHIPS + 1; j++)
        {
            countShots[i][j] = 0;
            if (i == CAR)
                countMoves[i][j] = 2;
            else countMoves[i][j] = 3;
        }
    }
    if (whichPlayer != player)
    {
        if (player)
            printf("INVALID OPERATION %c[playerB] %c: THE OTHER PLAYER EXPECTED", '"', '"');
        else printf("INVALID OPERATION %c[playerA] %c: THE OTHER PLAYER EXPECTED", '"', '"');
        deleteArrayAndExit(boardGlobal, boardValidator, Y);
    }
    while (cin >> command)
    {
        if (strcmp(command, "[playerA]") == 0 || strcmp(command, "[playerB]") == 0)
        {
            if (whichPlayer == playerB && turn_on_printing && playerData[player].playerBecameAi)
                extendedPrinting(boardGlobal, Y, X, blankValue);
            whichPlayer = inversePlayer(player);
            checkIfNextPlayerIsAi(boardGlobal, boardValidator, whichPlayer, Y, X, EXTENDED_SHIPS, SEED);
            return;
        }
        else if (strcmp(command, "PLACE_SHIP") == 0)
        {
            place_ship(command, boardGlobal, player, boardValidator, Y, X, false);
        }
        else if (strcmp(command, "SHOOT") == 0)
        {
            if (EXTENDED_SHIPS)
            {
                extended_shooting(player, boardGlobal, boardValidator, Y, command, countShots);
            }
            else
            {
                shoot(command, boardGlobal, inversePlayer(player), Y, X, boardValidator);
            }
        }
        else if (strcmp(command, "MOVE") == 0)
        {
            moveMain(command, player, boardGlobal, boardValidator, countMoves, Y, X);
        }
        else if (strcmp(command, "PRINT") == 0)
        {
            printPlayers(player, Y, X, boardGlobal);
        }
        else if (strcmp(command, "SPY") == 0)
        {
            sendPlane(command, player, boardGlobal, countShots, boardValidator, Y, X);
            fill(boardGlobal, Y, X);
        }
    }
}

int main()
{
    int Y = 21, X = 10, SEED = 0;
    bool whichPlayer = playerA, EXTENDED_SHIPS = false, turn_on_printing = false;
    char groupOfCommands[20];
    char** boardGlobal; // GLOBAL BOARD
    boardGlobal = new char* [Y];
    for (int i = 0; i < Y; i++)
    {
        boardGlobal[i] = new char[X];
    }
    char** boardValidator; // VALIDATION BOARD
    boardValidator = new char* [Y + 2];
    for (int i = 0; i < Y + 2; i++)
    {
        boardValidator[i] = new char[X + 2];
    }
    defaultParameters(boardGlobal, boardValidator, Y, X); // DEFAULT PARAMETERS
    while (cin >> groupOfCommands)
    {
        if (strcmp(groupOfCommands, "[state]") == 0)
        {
            char command[20];
            while (cin >> command)
            {
                if (strcmp(command, "[state]") == 0)
                {
                    checkIfNextPlayerIsAi(boardGlobal, boardValidator, whichPlayer, Y, X, EXTENDED_SHIPS, &SEED);
                    break;
                }
                if (strcmp(command, "SET_FLEET") == 0)
                {
                    set_fleet(boardGlobal, boardValidator, Y);
                }
                if (strcmp(command, "PRINT") == 0)
                {
                    printGlobal(boardGlobal, Y, X);
                }
                if (strcmp(command, "NEXT_PLAYER") == 0)
                {
                    whichPlayer = nextPlayer();
                }
                if (strcmp(command, "BOARD_SIZE") == 0)
                {
                    for (int i = 0; i < Y; i++)
                    {
                        delete[] boardGlobal[i];
                        delete[] playerData[playerA].board[i];
                        delete[] playerData[playerB].board[i];
                    }
                    delete[] boardGlobal;
                    delete[] playerData[playerA].board;
                    delete[] playerData[playerB].board;
                    for (int i = 0; i < Y + 2; i++)
                        delete[] boardValidator[i];
                    delete[] boardValidator;
                    cin >> Y >> X;
                    boardGlobal = new char* [Y];
                    playerData[playerA].board = new char* [Y];
                    playerData[playerB].board = new char* [Y];
                    for (int i = 0; i < Y; i++)
                    {
                        boardGlobal[i] = new char[X];
                        playerData[playerA].board[i] = new char[X];
                        playerData[playerB].board[i] = new char[X];
                    }
                    boardValidator = new char* [Y + 2];
                    for (int i = 0; i < (Y + 2); i++)
                    {
                        boardValidator[i] = new char[X + 2];
                    }
                    defaultParameters(boardGlobal, boardValidator, Y, X);
                }
                if (strcmp(command, "INIT_POSITION") == 0)
                {
                    initPosition();
                }
                if (strcmp(command, "SHIP") == 0)
                {
                    char player;
                    cin >> player;
                    place_ship(command, boardGlobal, definePlayer(player), boardValidator, (Y), (X), true);
                }
                if (strcmp(command, "REEF") == 0)
                {
                    reefInitialization(command, boardGlobal, boardValidator, (Y), (X));
                }
                if (strcmp(command, "EXTENDED_SHIPS") == 0)
                {
                    EXTENDED_SHIPS = 1;
                }
                if (strcmp(command, "SAVE") == 0)
                {
                    saveModule(Y, X, whichPlayer, boardGlobal);
                }
                if (strcmp(command, "SET_AI_PLAYER") == 0)
                {
                    char who;
                    cin >> who;
                    playerData[definePlayer(who)].playerBecameAi = true;
                }
                if (strcmp(command, "SEED") == 0)
                {
                    cin >> SEED;
                }
                if (strcmp(command, "TURN_ON_PRINTING") == 0)
                {
                    turn_on_printing = true;
                }
            }
        }
        else if (strcmp(groupOfCommands, "[playerA]") == 0)
        {
            playerMain(boardGlobal, boardValidator, Y, X, whichPlayer, EXTENDED_SHIPS, playerA, &SEED, turn_on_printing);
            whichPlayer = playerB;
        }
        else if (strcmp(groupOfCommands, "[playerB]") == 0)
        {
            playerMain(boardGlobal, boardValidator, Y, X, whichPlayer, EXTENDED_SHIPS, playerB, &SEED, turn_on_printing);
            whichPlayer = playerA;
        }
    }
    deleteArrayAndExit(boardGlobal, boardValidator, Y);
}
