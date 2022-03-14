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

    //Picks a random word from the word list
    srand(time(0));
    int randIndex = rand() % 2315 + 1;
    std::string word = word_list[randIndex];

    //Writes the current word to a file
    myFile.open("answer.txt", std::ios::out);
    if (myFile.is_open())
    {
        myFile << word;
        myFile.close();
    }

    int attempt = 1;
    boolean isRight = false;

    std::cout << "Welcome to Wordle" << std::endl;
    for (int attempt = 1; attempt <= 5; attempt++)
    {
        //Gets the Users guess
        boolean isValidGuess = false;
        std::string guess;

        std::cout << "Attempt " << attempt << ". ";
        while (!isValidGuess)
        {
            
            while (guess.length() != 5)
            {
                std::cout << "Please enter a 5 letter word." << std::endl;
                std::cin >> guess;
            }
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
            char letter = std::tolower(guess.at(i));
            guess.at(i) = letter; //coverts the guess to lower case

            //Letter is in the correct spot
            if (word.at(i) == letter){
                console.printColor(letter, "green");
                continue;
            }

            int appeared = 0;
            int appearedCorrect = 0;

            for (int j = 0; j <= 4; j++)
            {
                if (word.at(j) == letter)
                {
                    appeared++;                    
                }
                if (word.at(j) == guess.at(j) and guess.at(j) == letter)
                {
                    appearedCorrect++;
                }
            }
            

            //letter does not appear at all
            if (appeared == 0) 
            {
                console.printColor(letter, "grey");
            }


            //Letter is in the word just the wrong spot
            else
            {
                if (appearedCorrect < appeared)
                {
                    console.printColor(letter, "yellow");   
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
    std::string end;
    std::cout << "Type and press enter to close." << std::endl;
    std::cin >> end;
}