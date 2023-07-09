#include <iostream>
#include <vector>

struct Color
{
    char value;

    Color(char value)
    {
        this->value = value;
    }

    char getCode()
    {
        switch (value)
        {
        case 1:
            return 'R';
        case 2:
            return 'Y';
        default:
            return ' ';
        }
    }

    bool operator==(Color a)
    {
        if (this->getCode() == a.getCode())
            return true;
        else
            return false;
    }
};

class ConnectFour
{
private:
    int NumberOfRows = 6;
    int NumberOfCols = 7;
    int NumberOfDirs = 4;

    std::vector<std::vector<int>> directions = {
        {1, -1}, // SouthWest
        {1, 0},  // South
        {1, 1},  // SouthEast
        {0, 1}   // East
    };

    std::vector<std::vector<Color>> board;

    std::vector<std::vector<std::vector<std::vector<int>>>> possibleDirs;

public:
    void init()
    {
        // setup board
        board = std::vector<std::vector<Color>>(NumberOfRows, std::vector<Color>(NumberOfCols, 0));

        // setup possible directions for each position
        possibleDirs = std::vector<std::vector<std::vector<std::vector<int>>>>(NumberOfRows, std::vector<std::vector<std::vector<int>>>(NumberOfCols));

        for (int r = 0; r < NumberOfRows; r++)
        {
            for (int c = 0; c < NumberOfCols; c++)
            {
                for (int i = 0; i < NumberOfDirs; i++)
                {
                    if (r + 3 * directions[i][0] <= 5 &&
                        c + 3 * directions[i][1] <= 6 &&
                        c + 3 * directions[i][1] >= 0)
                    {
                        possibleDirs[r][c].push_back(directions[i]);
                    }
                }
            }
        }
    }
    void printBoard()
    {

        // +-+-+-+-+-+-+-+
        // | | | | | | | |
        // +-+-+-+-+-+-+-+

        std::string rowDivider = "+-+-+-+-+-+-+-+";

        std::cout << "\n===================\n\n";
        std::cout << rowDivider << std::endl;
        for (int r = NumberOfRows - 1; r >= 0; r--)
        {
            for (int c = 0; c < NumberOfCols; c++)
            {
                std::cout << '|' << board[r][c].getCode();
            }
            std::cout << '|' << '\n';
            std::cout << rowDivider << std::endl;
        }
        std::cout << "\n===================\n\n";
    }
    void play()
    {
        int turn = 0, col = 0;
        bool isRunning = true;

        while (turn < NumberOfRows * NumberOfCols && isRunning)
        {
            Color currColor(turn % 2 ? 1 : 2);

            while (true)
            {
                std::cout << "\nPlayer " << currColor.getCode() << " enter the column [1-7]: ";

                int input;
                std::cin >> input;

                if (input > 7 || input < 1)
                {
                    std::cout << "\nPlease input a valid digit [1-7]" << std::endl;

                    // check for failure in case bad input is given like string instead of int
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(512, '\n');
                    }
                    continue;
                }

                if (insert(currColor, input - 1))
                {
                    break;
                }
                else
                {
                    std::cout << "\nPlease input a valid digit [1-7]. " << std::endl;
                }
            }

            printBoard();

            // check for winner
            bool foundWinner = false;
            for (int r = 0; r < NumberOfRows; r++)
            {
                for (int c = 0; c < NumberOfCols; c++)
                {
                    if (board[r][c] == currColor)
                    {
                        for (int i = 0; i < possibleDirs[r][c].size(); i++)
                        {
                            if (countConsecutive(r, c, possibleDirs[r][c][i], currColor) >= 4)
                            {
                                foundWinner = true;
                                isRunning = false;
                                break;
                            }
                        }

                        if (foundWinner)
                        {
                            break;
                        }
                    }
                }

                if (foundWinner)
                {
                    break;
                }
            }

            if (foundWinner)
            {
                std::cout << "\nPlayer " << currColor.getCode() << " wins!" << std::endl;
            }

            turn++;
        }

        if (isRunning)
        {
            // nobody wins
            std::cout << "\nNobody won :\\ \n";
        }
    }
    bool insert(Color color, int col)
    {
        if (col < 0 || col >= NumberOfCols)
        {
            // incorrect columns
            return false;
        }

        for (int r = 0; r < NumberOfRows; r++)
        {
            // insert at first non-empty row
            if (board[r][col].getCode() == ' ')
            {
                board[r][col] = color;
                return true;
            }
        }

        // col is completely filled
        return false;
    }

    int countConsecutive(int r, int c, std::vector<int> &dir, Color color)
    {
        int count = 0;
        while (r < NumberOfRows && r >= 0 && c < NumberOfCols && c >= 0 && board[r][c] == color)
        {
            r += dir[0];
            c += dir[1];
            count++;
        }
        return count;
    }
};

int main()
{
    ConnectFour game;
    game.init();
    game.printBoard();
    game.play();
}