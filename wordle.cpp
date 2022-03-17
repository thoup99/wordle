#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

class Console
{
    private:
        HANDLE ha = GetStdHandle(STD_OUTPUT_HANDLE);
    public:
        void printColor();
        void changeColor();


    void printColor(char letter, std::string colorName)
    {
        setColor(colorName);
        std::cout << letter;
    }
    
    void setColor(std::string colorName)
    {
        if (colorName == "white")
            SetConsoleTextAttribute(ha, 7);
        else if (colorName == "grey" or colorName == "gray")
            SetConsoleTextAttribute(ha, 8);
        else if (colorName == "yellow")
            SetConsoleTextAttribute(ha, 14);
        else if (colorName == "green")
            SetConsoleTextAttribute(ha, 10);
        else
        {
            std::cout << "Error. Color not Found. Name " << colorName;
        }
    }
};

class LetterBank
{
    private:
        std::string letters;
        int values[26];
        Console console;
    public:
        LetterBank(Console c)
        {
            console = c;
            resetValues();
            letters = "abcdefghijklmnopqrstuvwxyz";
        }

        void resetValues()
        { 
            for (int i = 0; i < 26; i++)
            {
                values[i] = 1;
            } 
        }  

        void setValue(char letter, int newValue)
        {
            int index = letters.find(letter);
            values[index] = newValue;
        }

        int getValue(char letter)
        {
            int index = letters.find(letter);
            return values[index];
        }

        void printValues()
        {
            for (int i = 0; i < 26; i++)
            {
                if (values[i] == 0)
                {
                    console.printColor(letters.at(i), "grey");
                }
                else if (values[i] == 1)
                {
                    console.printColor(letters.at(i), "white");
                }
                else if (values[i] == 2)
                {
                    console.printColor(letters.at(i), "yellow");
                }
                else
                {
                    console.printColor(letters.at(i), "green");
                }
            } 
            std::cout << std::endl;
            console.setColor("white");
        }
};

boolean binarySearch(std::string arr[], int start, int end, std::string value){
    int high = end;
    int low = start;
    while (low <= high)
    {
        int mid = (high + low) / 2;
        if (arr[mid] == value)
        {
            return true;
        }
        else
        {
            if (value.compare(arr[mid]) < 0)
            {
                high = mid - 1;
            }
            else
            {
                low = mid + 1;
            }
        }
    }
    return false;
}


int main() {
    Console console = Console();
    LetterBank letterBank = LetterBank(console);
    std::fstream myFile;

    //Load word list
    std::string word_list[2315];
    myFile.open("word_list.txt", std::ios::in);
    if (myFile.is_open())
    {
        int index = 0;
        std::string temp;
        while (std::getline(myFile, temp))
        {
            word_list[index] = temp;
            index++;
        }
        myFile.close();
    }
    
                
    //load allowed list
    std::string allowed_list[10657];
    myFile.open("allowed_list.txt", std::ios::in);
    if (myFile.is_open())
    {
        int index = 0;
        std::string temp;
        while (std::getline(myFile, temp))
        {
            allowed_list[index] = temp;
            index++;
        }
        myFile.close();
    }

    boolean playAgain;
    do
    {

        //Picks a random word from the word list
        srand(time(0));
        int randIndex = rand() % 2314 + 1;
        std::string word = word_list[randIndex];

        //Writes the current word to a file
        myFile.open("answer.txt", std::ios::out);
        if (myFile.is_open())
        {
            myFile << word;
            myFile.close();
        }

        letterBank.resetValues();

        int attempt = 1;
        boolean isRight = false;

        std::cout << "Welcome to Wordle" << std::endl;
        for (int attempt = 1; attempt <= 6; attempt++)
        {
            //Gets the Users guess
            boolean isValidGuess = false;
            std::string guess;

            std::cout << "Attempt " << attempt << ". ";
            while (!isValidGuess)
            {

                while (guess.length() != 5)
                {
                    std::cout << "Please enter a 5 letter word.\t";
                    letterBank.printValues();
                    std::cin >> guess;
                }

                //Converts to lower case
                for (int i = 0; i < 5; i++)
                {
                    guess.at(i) = std::tolower(guess.at(i));
                }

                //Checks to see if the word entered is in the word lists
                if (!(binarySearch(word_list, 0, 2314, guess) or binarySearch(allowed_list, 0, 10656, guess)))
                {
                    std::cout << "That word is not in the word list. ";
                    guess = "";
                    continue;
                }
                isValidGuess = true;
            }

            //Goes back one line in the terminal and then backspaces 5 times.
            std::cout << "\033[F" << "\b\b\b\b\b";

            //Checks each letter of the guess
            for (int i = 0; i <= 4; i++){
                char letter = guess.at(i);

                //Letter is in the correct spot
                if (word.at(i) == letter){
                    console.printColor(letter, "green");
                    letterBank.setValue(letter, 4);
                    continue;
                }

                int timesInWord = 0;
                int timesLetterCorrect = 0;
                int timesInGuessBefore = 0;

                for (int j = 0; j <= 4; j++)
                {
                    if (word.at(j) == letter)
                    {
                        timesInWord++;
                        if (word.at(j) == guess.at(j))
                        {
                            timesLetterCorrect++;
                        }  
                    }                     
                    if (guess.at(j) == letter and j <= i)
                    {
                        timesInGuessBefore++;
                    }                                      
                }

                //letter does not appear at all
                if (timesInWord == 0) 
                {
                    console.printColor(letter, "grey");
                    letterBank.setValue(letter, 0);
                }
                
                //Letter is in the word just the wrong spot
                else
                {
                    int yellowsNeeded = 1 + timesInWord - timesLetterCorrect - timesInGuessBefore;
                    if(yellowsNeeded > 0)
                    {
                        console.printColor(letter, "yellow");
                        if (letterBank.getValue(letter) == 1)
                        {
                            letterBank.setValue(letter, 2);
                        }
                    }                     
                    else
                    {
                        console.printColor(letter, "grey");
                    }      
                }
            }
            std::cout << "" <<std::endl;
            console.setColor("white");
            if (word == guess)
            {
                std::cout << "You guessed the word correct in " << attempt << " tries!" << std::endl;
                isRight = true;
                break;
            }
        }
        if (!isRight)
        {
            console.setColor("white");
            std::cout << "You did not guess the word properly. It was " << word << "." << std::endl;
        }

        //Checks if the user wants to play again
        std::string response;        
        while (!(response == "y" or response == "n"))
        {
            std::cout << "Would you like to play again? (y/n)" << std::endl;
            std::cin >> response;
        }
        if (response == "y")
        {
            playAgain = true;
        }
        else
        {
            playAgain = false;
        }
    } while (playAgain);
}