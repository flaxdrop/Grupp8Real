

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>
struct cardInfo // Storing card Value
{
    int cardValue;              // The value to use when calc
    std::string cardInfomation; // String to display card
};

std::string getPlayerName()
{
    std::string name;
    std::cout << "Please enter your name: ";
    std::cin >> name;
    std::cout << "Your name is: " << name << std::endl;
    return name;
}

// Denna funktion gör bara så att funktionen getplayername kan kallas med variabeln playerName
// använd playerName för att skriva ut namnet som man hämtade.

void displayBlackjackRules()
{

    std::cout << "Welcome to Blackjack!\n";
    std::cout << "Rules!\n";
    std::cout << "1. The goal of the game is to get as close to 21 as possible without going over.\n";
    std::cout << "2. The number cards are worth their face value.\n";
    std::cout << "3. Face cards (Jack, Queen and King) Are worth 10.\n";
    std::cout << "4. Aces are either 1 or 11 depending on whats more favorable.\n";
    std::cout << "5. You can either hit (draw a card) or stand (End your turn).\n";
    std::cout << "6. If your hand exceeds 21, you bust and lose the round.\n";
    std::cout << "7. The dealer must hit until their total is atleast 17.\n";
    std::cout << "8. Whoever is closest to 21 without busting wins the round!\n";
    std::cout << "-----------------------------------------------------------------------------------------------\n\n\n";
}

void displayTwistRules()
{
    std::cout << "This is not your usual blackjack game, there will be some twists when you least expect it Such as: \n";
    std::cout << "1. Free Hit Twist:\n";
    std::cout << "   - Drawn cards that trigger this twist allow you to take a 'free hit' without the risk of busting.\n";
    std::cout << "   - You can only use this twist once per turn when it's triggered.\n\n";

    // std::cout << "2. Reset Hand Value Twist:\n";
    // std::cout << "   - If this twist is triggered, it resets the hand value of either the player or dealer to 10.\n";
    // std::cout << "   - Useful for avoiding a bust or getting a strategic advantage.\n\n";

    // std::cout << "3. Card Swap Twist:\n";
    // std::cout << "   - This twist allows you to swap one card from your hand with one of the dealer’s cards.\n";
    // std::cout << "   - Choose wisely, as this twist can change the outcome significantly!\n\n";

    // std::cout << "4. Twist Cards:\n";
    // std::cout << "   - Specific cards like the Ace of Spades or Queen of Hearts are designated as twist cards.\n";
    // std::cout << "   - When a twist card is drawn by either the player or dealer, it triggers one of the twists above.\n\n";

    std::cout << "Good luck! Keep an eye out for twist cards and use them strategically to gain an advantage.\n";
    std::cout << "-----------------------------------------------------------------------------------------------\n\n\n";
}

bool askToLoadGame(int &playerScore, std::string &playerName)
{
    std::ifstream infile("highscore.txt");
    if (infile.good())
    {
        char choice;
        std::cout << "A saved highscore file was found. Do you want to load the game? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            if (infile >> playerScore >> playerName)
            {
                std::cout << "Game loaded successfully! " << " score: " << playerScore << "\n";
                return true;
            }
            else
            {
                std::cout << "Error reading saved game data.\n";
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        std::cout << "No saved game found.\n";
    }
    return false;
}

void askToSave(int balance, std::string playerName)
{
    char choice;
    std::cout << "Do you want to save your game? (y/n): ";
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        std::ofstream outfile("highscore.txt");
        if (outfile.is_open())
        {
            outfile << balance << "\n";
            outfile << playerName << "\n";
            std::cout << "Game saved successfully!\n";
        }
        else
        {
            std::cout << "Error saving the game.\n";
        }
    }
    else
    {
        std::cout << "Continuing the game...\n";
    }
}

class Deck
{
public:
    // All cards in deck
    std::vector<std::string> color = {"♥", "♦", "♠", "♣"};
    std::vector<int> value = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
};
cardInfo DrawCard()
{
    // Draw new card to CardInfo
    Deck deck;
    int cardDrawnValue = deck.value.at(0 + rand() % 12); // Draw a random card
    std::string cardInfo = deck.color.at(0 + rand() % 3) + " ";
    switch (cardDrawnValue) // Sets name for all clothing cards
    {
    case 1:
        cardInfo += "Ace";
        break;
    case 11:
        cardInfo += "Knight";
        break;
    case 12:
        cardInfo += "Queen";
        break;
    case 13:
        cardInfo += "King";
        break;
    default:
        cardInfo += std::to_string(cardDrawnValue);
        break;
    }
    if (cardDrawnValue > 10)
    {
        cardDrawnValue = 10;
    }
    return {cardDrawnValue, cardInfo};
}
int calculateHandValue(const std::vector<cardInfo> &hand)
{
    int totalValue = 0;
    for (const cardInfo &card : hand)
    {
        if (card.cardValue == 1 && totalValue < 11) // ace
        {
            totalValue += 11; // räknar först ace som 11
        }
        else
        {
            totalValue += card.cardValue;
        }
    }
    return totalValue;
}
bool determineWinner(int playerHandValue, int dealerHandValue)
{
    bool youWon;
    std::cout << "Final Hand Values:\n";
    std::cout << "Player's Hand Value: " << playerHandValue << "\n";
    std::cout << "Dealer's Hand Value: " << dealerHandValue << "\n";
    if (playerHandValue > 21 && dealerHandValue > 21)
    {
        if (playerHandValue >= dealerHandValue)
        {
            std::cout << "Dealer wins!\n";
            return youWon = false;
        }
        else
        {
            std::cout << "Player wins!\n";
            return youWon = true;
        }
    }
    else if (playerHandValue > 21 && dealerHandValue <= 21)
    {
        std::cout << "Player busts! Dealer wins.\n";
        return youWon = false;
    }
    else if (dealerHandValue > 21 && playerHandValue <= 21)
    {
        std::cout << "Dealer busts! Player wins.\n";
        return youWon = true;
    }
    else if (playerHandValue <= 21 && dealerHandValue <= 21)
    {
        if (playerHandValue <= dealerHandValue)
        {
            std::cout << "Dealer wins!\n";
            return youWon = false;
        }
        else
        {
            std::cout << "Player wins!\n";
            return youWon = true;
        }
    }
}
std::vector<cardInfo> playerTurn(int playerHandValue, std::vector<cardInfo> playerHand, std::string playerName)
{
    bool isRoundEnd = false;
    int choice = 0;
    int rndCurse;
    do
    {
        rndCurse = 1 + rand() % 10;
        srand(time(NULL));
        choice = 0;
        if (rndCurse <= 2)
        {
            playerHand.push_back(DrawCard());
            std::cout << playerName << " gets cursed and draws extra card: " << playerHand.back().cardInfomation << std::endl;
            playerHandValue = calculateHandValue(playerHand);
            std::cout << playerName << " have: " << playerHandValue << std::endl;
        }
        playerHand.push_back(DrawCard());
        std::cout << playerName << " draws: " << playerHand.back().cardInfomation << std::endl;
        playerHandValue = calculateHandValue(playerHand);
        std::cout << playerName << " have: " << playerHandValue << std::endl;
        if (playerHandValue < 21)
        {
            std::cout << "1. Hit\n2. Stand" << std::endl;
            std::cin >> choice;
            switch (choice)
            {
            case 2:
                isRoundEnd = true;
                break;

            default:
                isRoundEnd = false;
                break;
            }
        }
        else
        {
            isRoundEnd = true;
        }

    } while (isRoundEnd == false);
    return playerHand;
}
std::vector<cardInfo> dealerTurn(int dealerHandValue, std::vector<cardInfo> dealerHand)
{
    bool isRoundEnd = false;
    do
    {
        Sleep(1000);
        srand(time(NULL));
        dealerHand.push_back(DrawCard());
        std::cout << "Dealer draws: " << dealerHand.back().cardInfomation << std::endl;
        dealerHandValue = calculateHandValue(dealerHand);
        std::cout << "Dealer have: " << dealerHandValue << std::endl;
        if (dealerHandValue < 17)
        {
            isRoundEnd = false;
        }
        else
        {
            isRoundEnd = true;
        }
    } while (isRoundEnd == false);
    return dealerHand;
}

// Function to check if the input string is a valid number
bool isValidNumber(const std::string &str)
{
    for (char const &c : str)
    {
        if (std::isdigit(c) == 0)
            return false; // Check for non-digit characters
    }
    return true;
}
bool askToContinue()
{
    char choice;
    std::cout << "Do you want to continue game? (y/n): ";
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        return true;
    }
    else
    {
        return false;
    }
}
int main()
{
    std::vector<cardInfo> dealerHand;
    std::vector<cardInfo> playerHand;
    cardInfo card;
    std::string playerName;
    std::vector<cardInfo> playerCards = {};
    const int startBalance = 500;
    int choice, bet;
    int balance = 0;
    int loop = true;
    bool gameLoaded = false;
    bool exitGame = false;
    do
    {
        std::cout << "\n\t****************************\n";
        std::cout << "\t*    Welcome to Blackjack  *\n";
        std::cout << "\t****************************\n";
        std::cout << "\n\t╔═══════════════════════════╗";
        std::cout << "\n\t║         MAIN MENU         ║";
        std::cout << "\n\t╠═══════════════════════════╣";
        std::cout << "\n\t║ 1. Show game rules        ║";
        std::cout << "\n\t║ 2. Load saved game        ║";
        std::cout << "\n\t║ 3. Start a new game       ║";
        std::cout << "\n\t║ 4. Exit                   ║";
        std::cout << "\n\t╚═══════════════════════════╝";

        std::cout << "\n\nEnter your choice: ";
        std::cin >> choice;
        switch (choice)
        {
        case 1:
            displayBlackjackRules();
            displayTwistRules();
            break;

        case 2:
            std::cout << "Loading saved game...\n";
            gameLoaded = askToLoadGame(balance, playerName);
            if (gameLoaded == false)
            {
                loop = true;
            }
            else if (gameLoaded == true)
            {
                loop = false;
            }
            break;

        case 3:
            playerName = getPlayerName();
            std::cout << "Starting a new game." << std::endl;
            balance = startBalance;
            loop = false;
            break;

        case 4:
            std::cout << "Exiting the game, goodbye!.\n";
            exitGame = true;
            loop = false;
            break;

        default:
            std::cout << "Invalid option, please try again.\n";
            break;
        }
    } while (loop == true);

    std::string betInput; // String to capture input
    bool isBetValid;
    if (exitGame == false)
    {
        do
        {
            do
            {
                isBetValid = false; // Set to false initially to enter the loop
                std::cout << "You have: " << balance << std::endl
                          << "How much do you wanna bet? ";
                std::cin >> betInput; // Read input as a string

                // Validate input
                if (isValidNumber(betInput))
                {
                    bet = std::stoi(betInput); // Convert string to integer
                    if (bet > 0 && bet <= balance)
                    {
                        isBetValid = true; // Valid bet
                        balance -= bet;
                    }
                    else
                    {
                        std::cout << "Bet must be between 1 and " << balance << std::endl;
                    }
                }
                else
                {
                    std::cout << "Invalid input. Please enter a number." << std::endl; // Handle invalid input
                }

            } while (!isBetValid);

            dealerHand.clear();
            playerHand.clear();
            //  Dealer draws first card
            dealerHand.push_back(DrawCard());
            playerHand.push_back(DrawCard());
            std::cout << "_______________________________________________________" << std::endl;
            std::cout << "Dealer draws: " << dealerHand.at(0).cardInfomation << std::endl;
            std::cout << playerName << " draws: " << playerHand.at(0).cardInfomation << std::endl;
            std::cout << "_______________________________________________________" << std::endl;
            int playerHandValue = 0, dealerHandValue = 0;
            playerHand = playerTurn(playerHandValue, playerHand, playerName);
            std::cout << "_______________________________________________________" << std::endl;
            dealerHand = dealerTurn(dealerHandValue, dealerHand);
            std::cout << "_______________________________________________________" << std::endl;
            // Calculate hand values
            dealerHandValue = calculateHandValue(dealerHand); // Calculate dealer's hand value
            playerHandValue = calculateHandValue(playerHand); // Calculate player's hand value

            // Determine the winner
            bool youWon = determineWinner(playerHandValue, dealerHandValue);
            if (youWon == true)
            {
                balance += bet * 2;
            }
            std::cout << "You have " << balance << "$ Left!" << std::endl;
            if (balance > 0)
            {
                loop = askToContinue();
            }
            else
            {
                std::cout << "You are exiting the game!";
                loop = false;
            }

        } while (loop == true);
        if (balance > 0)
        {
            askToSave(balance, playerName);
        }
    }

    return 0;
}
