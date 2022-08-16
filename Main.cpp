#include <bits/stdc++.h>

using namespace std;

// Creating Board list
vector <vector <char>> Table (9, vector <char> (9, ' '));

char player; // Black or White
int ValidMovesCount;
int BlackCoins, WhiteCoins; // Number of Coins
int Round;

//**********************************

// Game description
void welcome (); 
// Replacing W & White in the middle
void TableBoard (); 
// Printing the Board
void Board (); 
void CoinCount ();
// Shows which player has the right to move
void playerturn ();
// Changing the Player
void changePlayer ();
// Checks if the inputs are valid or not
void input (int& row, int& column); 

// Determine if a move is legal (searches with checkmove function)
bool isValidMove (int row, int column); 
// Does the move and flip the coins
void Movement (int row, int column); 
// Determines if a move is legal
bool checkMove (int newRow, int newColumn, int deltaRow, int deltaColumn); 
// Flips appropriate pieces after a coin is placed down
void Flip (int newRow, int newColumn, int deltaRow, int deltaColumn); 

// Printing legal moves
void PrintValidMoves ();
// Determine winner
void Winner ();
// Undo the last move or Adimt
void Undo_Admit (bool& admit, vector <vector <char>> undo);
// Clear "*"'s
void ClearTable ();
// if user wants to restart the program it returns true
void Restart (bool& restart);

//**********************************

int main ()
{
	cout << endl;

    // sets to true in order to run the fist while
    bool restart = true;
    while (restart)
    {
        restart = false;

        welcome ();
        TableBoard ();

        ValidMovesCount = 64;
        Round = 1;
        
        // is been used for restarting the program
        vector <vector <char>> temp (9, vector <char> (9, ' '));
        temp = Table;
        
        // is been used in Undo_Admit function
        vector <vector <char>> undo (9, vector <char> (9, ' '));
        undo = Table;

        // it continues until ValidMovesCount equals 0
	    while (true)
        { 
            if (Round > 1) system ("cls"); // clears terminal
            
            changePlayer (); 
            PrintValidMoves ();
            Board ();            
            
            if (ValidMovesCount == 0) break;

            undo = Table;
            playerturn ();
            CoinCount ();
            
            int row, column;
            bool admit = false;
            
            while (!admit)
            {
                input (row, column);
                Movement (row, column);
                Board ();
                Undo_Admit (admit, undo);
            }

            ClearTable ();
        }

        Winner ();

		cout << "Wanna continue? type 4 for YES or 3 for NO ";
        Restart (restart);
        
        Table = temp;
        system ("cls");
    }
}

//**********************************

void Board ()
{
    cout << "    0   1   2   3   4   5   6   7" << endl;
    
    for (int row = 0; row <= 8; row ++)
    {
    	cout << "  ";
        for (int k = 0; k <= 7; k ++) cout << " ---";
        cout << endl;

        bool flag = true;
		if (row == 8) flag = false;

	  	if (flag)
        {
		    cout << row << " ";
            for( int column = 0; column <= 7; column ++)
		    	cout << "| " << Table [row][column] << " ";
		    cout << "|";
        }
	
		cout << endl;
    }
}

//**********************************

void TableBoard ()
{
    Table [3][3] = 'w';
    Table [4][4] = 'w';
    Table [4][3] = 'b';
    Table [3][4] = 'b';
}

//**********************************

void CoinCount ()
{
    int Black = 0, White = 0;
    
    for (int i = 0; i <= 7; i++)
        for (int j = 0; j <= 7; j++)
        {
            if (Table [i][j] == 'b') Black ++;
            else if (Table [i][j] == 'w') White ++;
        }
    
    cout << "    " << "BlackCoins: " << Black
        << "    WhiteCoins: " << White << endl;

    WhiteCoins = White;
    BlackCoins = Black;
}

//**********************************

void changePlayer ()
{
    // for 2k rounds is black turn to move, for 2k + 1 rounds is white turn
    if (Round % 2 == 1) player = 'b';
    else player = 'w';
    
    Round ++;
}

//**********************************

void playerturn ()
{
    if (player == 'w') cout << "Player turn: White";
    else cout << "Player turn: Black";
}

//**********************************

void welcome ()
{
    cout << "Welcome to the Othello simulation. This C++ program is a playable Othello game,"<< endl
	<< "which consists of two players ('White' and 'Black')  competing for space on the board." << endl
    <<"First player is black. Please enter the row number then the column" << endl << endl;
}

//**********************************

void input (int& row, int& column)
{
    cout << "Enter your Move: (e.g: 23) ";
    int command;
    cin >> command;

    column = command % 10;
    row = command / 10;

    // check if this goes off of the board
    if (command >= 100 || row < 0 || row > 7 || column < 0 || column > 7)
    {
    	cout << "invalid input, pls try again ";
        input (row, column);
	}

    // check if the move is legal or not
    while (Table [row][column] != '*')
    {
        cout << "invalid input, pls try again ";
        input (row, column);
    }
	
	cout << endl;
}

//**********************************

bool isValidMove (int row, int column)
{
    if (Table [row][column] != ' ') return false;
    // use deltas to find all 8 surrounding positions, 3 positions above, 2 positions on same row, 3 positions below
    // check down-left
    else if (checkMove (row - 1, column - 1, -1, -1)) return true;
    // check down
    else if (checkMove (row - 1, column, -1, 0)) return true;
    // check down-right
    else if (checkMove (row - 1, column + 1, -1, +1)) return true;
    // check left
    else if (checkMove (row, column - 1, 0, -1)) return true;
    // check right
    else if (checkMove (row, column + 1, 0, 1)) return true;
    // check up-left
    else if (checkMove (row + 1, column - 1, +1, -1)) return true;
    // check up
    else if (checkMove (row + 1, column, +1, 0)) return true;
    // check up-right
    else if (checkMove (row + 1, column + 1, +1, +1)) return true;
    else return false;
}

//**********************************

void Movement (int row, int column)
{
    Table [row][column] = player;
    
    // check down-left
    if (checkMove (row - 1, column - 1, -1, -1)) Flip (row - 1, column - 1, -1, -1);
    // check down
    if (checkMove (row - 1, column, -1, 0)) Flip (row - 1, column, -1, 0);
    // check down-right
    if (checkMove (row - 1, column + 1, -1, +1)) Flip (row - 1, column + 1, -1, +1);
    // check left
    if (checkMove (row, column - 1, 0, -1)) Flip (row, column - 1, 0, -1);
    // check right
    if (checkMove (row, column + 1, 0, 1)) Flip (row, column + 1, 0, 1);
    // check up-left
    if (checkMove (row + 1, column - 1, +1, -1)) Flip (row + 1, column - 1, +1, -1);
    // check up
    if (checkMove (row + 1, column, +1, 0)) Flip (row + 1, column, +1, 0);
    // check up-right
    if (checkMove (row + 1, column + 1, +1, +1)) Flip (row + 1, column + 1, +1, +1);

}

//**********************************

bool checkMove (int newRow, int newColumn, int deltaRow, int deltaColumn)
{
    char opponent = 'w';
    if (player == 'w') opponent = 'b';

    if ((newRow < 0) || (newColumn < 0) || (newRow > 7) || (newColumn > 7)) return false;

    if (Table [newRow][newColumn] == opponent)
    {
        while (true)
        {
            // return if this goes off of the board
            if ((newRow < 0) || (newColumn < 0) || (newRow > 7) || (newColumn > 7)) return false;
            newRow += deltaRow;
            newColumn += deltaColumn;
            if ((newRow < 0) || (newColumn < 0) || (newRow > 7) || (newColumn > 7)) return false;

            if (Table [newRow][newColumn] == ' ' || Table [newRow][newColumn] == '*') return false;
            else if (Table [newRow][newColumn] == player) return true;
        }
    }
    return false;
}

//**********************************

void Flip (int newRow, int newColumn, int deltaRow, int deltaColumn)
{
    char opponent = 'w';
    if (opponent == player) opponent = 'b';

    Table [newRow][newColumn] = player;
    
    // continue in this delta position until the next character is no longer the opponent
    while (opponent == Table [newRow + deltaRow][newColumn + deltaColumn])
    {
        newRow += deltaRow;
        newColumn += deltaColumn;
        Table [newRow][newColumn] = player;
    }
    
}

//**********************************

void PrintValidMoves ()
{
    ValidMovesCount = 0;
    for (int row = 0; row <= 7; row++)
        for (int column = 0; column <= 7; column++)
            if (isValidMove (row, column))
            {
                ValidMovesCount ++;
                Table [row][column] = '*';
            }
    cout << endl;

    return;
}

//**********************************

void ClearTable ()
{
    for (int row = 0; row <= 7; row ++)
        for (int column = 0; column <= 7; column ++)
            if (Table [row][column] == '*') Table [row][column] = ' ';
}

//**********************************

void Winner ()
{
    if (BlackCoins > WhiteCoins) cout << "Winner is: Black" << endl;
    if (BlackCoins < WhiteCoins) cout << "Winner is: White" << endl;
}

//**********************************

void Undo_Admit (bool& admit, vector <vector <char>> undo)
{
    cout <<"type 0 for UNDO and 1 for ADMIT: ";
    int command;
    cin >> command;
    
	if (command != 0 && command != 1)
    {
    	cout << "invalid input! ";
    	Undo_Admit (admit, undo);
	}
		
    cout << endl;
    
    if (command == 0)
    {
        admit = false;
        Table = undo;
        Board ();
    }
    else admit = true;
}

//**********************************

void Restart (bool& restart)
{
	int ask;
    cin >> ask;
    
	if (ask != 4 && ask != 3)
    {
    	cout << "invalid input! ";
    	Restart (restart);
	}
	
	if (ask == 4) restart = true;
    else restart = false;
}