#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

class Console
{
    private:
        HANDLE ha = GetStdHandle(STD_OUTPUT_HANDLE);
    public:
        enum colors{};
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


int main() {
    Console c = Console();
    std::fstream myFile;

    std::string word = "santa";
    myFile.open("answer.txt", std::ios::out);
    if (myFile.is_open())
    {
        myFile << word;
        myFile.close();
    }

    int attempt = 1;
    boolean isRight = false;

    std::cout << "Welcome to Wordle this is a secret message from Owen :D" << std::endl;
    for (int attempt = 1; attempt <= 5; attempt++)
    {
        //Gets the Users guess
        std::string guess;
        std::cout << "Attempt " << attempt << ". Please enter a 5 letter word.";
        std::cin >> guess;
        while (guess.length() != 5)
        {
            std::cout << "Please enter a 5 letter word." << std::endl;
            std::cin >> guess;
        }
        
        //Checks each letter of the guess
        for (int i = 0; i <= 4; i++){
            char letter = guess.at(i);

            //Letter is in the correct spot
            if (word.at(i) == letter){
                c.printColor(letter, "green");
                continue;
            }

            int appeared;
            for (int j = 0; j <= 4; j++)
            {
                if (word.at(j) == letter)
                {
                    appeared++;
                }
            }

            //letter does not appear at all
            if (appeared == 0) 
            {
                c.printColor(letter, "grey");
                continue;
            }

            //Letter is in the word just the wrong spot
            else if(true)
            {
                c.printColor(letter, "yellow");
            }
        }
        std::cout << "" <<std::endl;
        c.setColor("white");
        if (word == guess)
        {
            std::cout << "You guessed the word correct in " << attempt << " tries!";
            break;
        }
    }

}