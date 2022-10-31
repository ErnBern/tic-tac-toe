#include <iostream>
#include <cstdlib>
#include <ctime>
#define underline "\033[4m"
#define endunderline "\033[0m"

int rand(); // I honestly don't know why I have to do this, but I just know I have too

char board[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'}; // Making the board arrary so I can edit it easily

int isvalidresponse(int pos);

//Formats and prints the board
void printboard() {
    for (int i = 0; i < 9; i++) {
        if (i == 2 || i == 5) {
            std::cout << "  " << board[i] << "  " << endunderline << std::endl;
            continue;
        } if (i > 5 && i != 8) {
            std::cout << "  " << board[i] << "  |";
            continue;
        } if (i == 8) {
            std::cout << "  " << board[i] << "  " << std::endl;
            continue;
        }
        std::cout << underline << "  " << board[i] << "  |";
    }
}

//Checks if the game is over and checks who won or if it's a tie
int isgameover(bool isnpc, int turns = 0) {
    if (isnpc) {
        //Used for checking if a tie has occured during the npc gamemode
        int marks;
        for (int i = 0; i < 9; i++) {
            const char mark = board[i];
            if (mark != 'O' || mark != 'X') {continue;}
            marks++;
        } if (marks == 8) {return 3;} // Returns 3 if the game is a tie
    } if (!isnpc && turns > 9) { // Checks for tie in 1v1 gamemode
        return 3;
    }
    int winconditions[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}
    };
    //Checks if someone won
    for (int i = 0; i < 9; i++) {
        //I have to make these variables because winconditions[i][0], etc doesn't work
        const int conditionone = winconditions[i][0];
        const int conditiontwo = winconditions[i][1];
        const int conditionthree = winconditions[i][2];
        if (conditiontwo == conditionthree) {continue;} //The program will give someone a win 
        //if they put their mark on the 7th and 5th tile if this was not here
        if (board[conditionone] == 'O' && board[conditiontwo] == 'O' && board[conditionthree] == 'O') {return 2;} // Returns 2 if player 2 or theNPC wins
        if (board[conditionone] == 'X' && board[conditiontwo] == 'X' && board[conditionthree] == 'X') {return 1;}//Returns 1 if player 1 wins
    }
    return 0;
}

int npc(int turns = 0) {
    if (turns == 0) {
        printboard();
        turns++;
    }
    std::cout << "Enter a number:" << std::endl;
    int response;
    std::cin >> response;
    //Checks if the response is a corresponding number on the board
    if (response < 1 || response > 9) {
        std::cout << "Invalid response!" << std::endl;
        return npc(turns);
    }
    const bool vaildresponse = isvalidresponse(response); // This functions checks if a tile is already marked
    // isvalidresponse returns true if the tile is unmarked
    if (!vaildresponse) {
        std::cout << "Invalid response!" << std::endl;
        return npc(turns);
    }
    response -= 1;
    //Random number generator
    std::srand(time(NULL));
    board[response] = 'X';
    int pos = (rand() % 10);
    bool validresponse = false; // This doesn't throw an error so I left it here
    //Incase the number generatored is already on the board
    while (!validresponse || pos == response) {
        pos = (rand() % 10);
        std::cout<<pos << std::endl;
        if (board[pos] == 'X' || board[pos] == 'O') {continue;}
        validresponse = isvalidresponse(pos);
    }
    board[pos] = 'O';
    std::cout << "Player = X" << std::endl;
    std::cout << "NPC = O" << std::endl;
    printboard();
    //Checks who won, if someone did
    int win = isgameover(true);
    if (win == 1) {
        std::cout << "You Win!" << std::endl;
        return 0;
    } if (win == 2) {
        std::cout << "You lose!" << std::endl;
        return 0;
    } if (win == 3) {
        std::cout << "The game has resulted in a tie!" << std::endl;
        return 0;
    }
    npc(turns);
    return 0;
}

int twoplayers(int turns = 0) {
    if (turns == 0) {
        printboard();
    }
    std::cout << "Player 1's Turn\nEnter a number:" << std::endl;
    //Memory management ig. Code doesn't work without this
    int* response;
    response = new int;
    //Making sure the user gives a valid response
    std::cin >> *response;
    while (*response < 1 || *response > 9) {
        std::cout << "Invalid Response" << std::endl;
        std::cout << "Player 1's Turn\nEnter a number:" << std::endl;
        std::cin >> *response;
    }
    bool validresponse = isvalidresponse(*response);
    while (!validresponse) {
        std::cin >> *response;
        validresponse = isvalidresponse(*response);
        std::cout << "Invalid response!" << std::endl;
    }
    //Subtracting one so we can use it to find a place on the board array
    *response -= 1;
    board[*response] = 'X';
    turns++;
    int didplayer1win = isgameover(false, turns); 
    if (didplayer1win == 1) {
        std::cout << "Player 1 has won!" << std::endl;
        printboard();
        return 0;
    } if (didplayer1win == 3) {
        std::cout << "The game has resulted in a tie!" << std::endl;
        printboard();
        return 0;
    }
    //Prints the changes to the board
    printboard();

    std::cout << "Player 2's Turn\nEnter a number:" << std::endl;
    std::cin >> *response;
    while (*response < 1 || *response > 9) {
        std::cout << "Invalid Response" << std::endl;
        std::cout << "Player 2's Turn\nEnter a number:" << std::endl;
        std::cin >> *response;
    }
    
    validresponse = isvalidresponse(*response);
    while (!validresponse) {
        std::cin >> *response;
        validresponse = isvalidresponse(*response);
        std::cout << "Invalid response!" << std::endl;
    }
    *response -= 1;
    board[*response] = 'O';
    turns++;
    int didplayer2win = isgameover(false, turns);
    if (didplayer2win == 2) {
        std::cout << "Player 2 has won!" << std::endl;
        printboard();
        return 0;
    } if (didplayer2win == 3) {
        std::cout << "The game has resulted in a tie!" << std::endl;
        printboard();
        return 0;
    }
    printboard();
    twoplayers(turns);
    delete response;
    return 0;
}

int main() {
    std::cout << "Welcome to my simple tictactoe game!" << std::endl;
    std::cout << "Each Box will be assigned a number\n" << std::endl;
    std::cout << underline << "  1  |  2  |  3  " << endunderline << std::endl;
    std::cout << underline << "  4  |  5  |  6  "  << endunderline << std::endl;
    std::cout << "  7  |  8  |  9  \n" << std::endl;
    char response;
    bool endloop = false;

    while (!endloop) {
        std::cout << "Do you want to play with a NPC or with another player?\nA)NPC\nB)Another Player" << std::endl;
        std::cin >> response;
        if (tolower(response) == 'a') {npc(); endloop = true;} 
        else if (tolower(response) == 'b') {twoplayers(); endloop = true;}
        else {std::cout << "Invalid Response!";}
    }
    return 0;
}

int isvalidresponse(int pos) {
    pos -= 1;
    char boardpos = board[pos];
    std::cout << boardpos << std::endl;
    if (boardpos == 'O' || boardpos == 'X') {return false;}
    return true;
}