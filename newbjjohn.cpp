#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <ctime>

using namespace std;

struct Card
{
    string suit;
    int value;
};

class Deck
{
    vector<Card> cards;

public:
    Deck()
    {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        for (const string &suit : suits)
        {
            for (int value = 1; value <= 13; ++value)
            {
                cards.push_back({suit, value});
            }
        }
        shuffleDeck();
    }

    void shuffleDeck()
    {
        random_device rd;
        default_random_engine eng(rd());
        shuffle(cards.begin(), cards.end(), eng);
    }

    Card drawCard()
    {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

void displayBlackjackRules()
{
    cout << "Blackjack Rules:\n";
    cout << "1. Try to get as close to 21 as possible without going over.\n";
    cout << "2. Face cards are worth 10. Aces are worth 1 or 11.\n";
    cout << "3. Dealer must hit until reaching a minimum of 17.\n";
    cout << "4. You can hit or stand on your turn.\n";
    cout << "-------------------------------------\n";
    cout << "This game of blackjack has some twist!! Play a few hands and find out what they are!";
}

int calculateScore(const vector<Card> &hand)
{
    int score = 0, aces = 0;
    for (const Card &card : hand)
    {
        if (card.value >= 10)
            score += 10;
        else if (card.value == 1)
        {
            score += 11;
            aces++;
        }
        else
            score += card.value;

        while (score > 21 && aces > 0)
        {
            score -= 10;
            aces--;
        }
    }
    return score;
}

void displayHand(const vector<Card> &hand, const string &name)
{
    cout << name << "'s hand: ";
    for (const Card &card : hand)
    {
        cout << (card.value == 1 ? "A" : (card.value > 10 ? "10" : to_string(card.value)))
             << " of " << card.suit << ", ";
    }
    cout << " (Score: " << calculateScore(hand) << ")\n";
}

int loadBalance()
{
    ifstream inFile("savefile.txt");
    int balance = 1000; // Default starting balance if no save file
    if (inFile)
    {
        inFile >> balance;
        cout << "Balance loaded from save: $" << balance << "\n";
    }
    else
    {
        cout << "No save file found. Starting with default balance: $" << balance << "\n";
    }
    return balance;
}

void saveBalance(int balance)
{
    ofstream outFile("savefile.txt");
    outFile << balance;
    cout << "Game saved! Balance: $" << balance << " has been saved.\n";
}

void playGame(int &balance)
{
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    Deck deck;
    vector<Card> playerHand, dealerHand;
    char choice;
    int bet;

    while (true)
    {
        playerHand.clear();
        dealerHand.clear();

        // Betting
        cout << "\nBalance: $" << balance << "\nPlace your bet: ";
        cin >> bet;

        if (bet > balance)
        {
            cout << "You don't have enough money to bet that amount!\n";
            continue;
        }

        // Initial deal
        playerHand.push_back(deck.drawCard());
        playerHand.push_back(deck.drawCard());
        dealerHand.push_back(deck.drawCard());
        dealerHand.push_back(deck.drawCard());

        cout << "Dealer's visible card: "
             << (dealerHand[0].value == 1 ? "A" : (dealerHand[0].value > 10 ? "10" : to_string(dealerHand[0].value)))
             << " of " << dealerHand[0].suit << "\n";

        bool playerBusted = false;

        // Player's turn
        while (true)
        {
            displayHand(playerHand, "Player");

            if (calculateScore(playerHand) > 21)
            {
                cout << "You bust! You lose $" << bet << "\n";
                balance -= bet;
                playerBusted = true;
                break;
            }

            cout << "Do you want to (H)it or (S)tand? ";
            cin >> choice;

            if (choice == 'S' || choice == 's')
            {
                // 20% chance to unintentionally hit instead of standing
                if (rand() % 5 == 0) // 1 in 5 chance
                {
                    cout << "Oops! You tried to stand, but hit instead!\n";
                    playerHand.push_back(deck.drawCard());
                }
                else
                {
                    break;
                }
            }
            else if (choice == 'H' || choice == 'h')
            {
                playerHand.push_back(deck.drawCard());

                // 20% chance to unintentionally draw an extra card
                if (rand() % 5 == 0) // 1 in 5 chance
                {
                    cout << "Lucky double! You drew an extra card!\n";
                    playerHand.push_back(deck.drawCard());
                }
            }
        }

        // Dealer's turn (only if player didn't bust)
        if (!playerBusted && calculateScore(playerHand) <= 21)
        {
            while (calculateScore(dealerHand) < 17)
            {
                dealerHand.push_back(deck.drawCard());
            }

            displayHand(dealerHand, "Dealer");

            // Determine outcome
            int playerScore = calculateScore(playerHand);
            int dealerScore = calculateScore(dealerHand);

            if (dealerScore > 21 || playerScore > dealerScore)
            {
                cout << "You win! You gain $" << bet << "\n";
                balance += bet;
            }
            else if (playerScore < dealerScore)
            {
                cout << "Dealer wins. You lose $" << bet << "\n";
                balance -= bet;
            }
            else
            {
                cout << "It's a tie! No money lost or won.\n";
            }
        }

        // Check if player wants to play another round
        if (balance <= 0)
        {
            cout << "You're out of money! Game over.\n";
            break;
        }

        cout << "Play another round? (Y/N): ";
        cin >> choice;
        if (choice == 'N' || choice == 'n')
            break;
    }
}

int main()
{
    int balance = 1000;
    char choice;
    bool gameRunning = true;

    while (gameRunning)
    {
        cout << "\nMenu:\n";
        cout << "1. View Game Rules\n";
        cout << "2. Start Game\n";
        cout << "3. Load Game\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            displayBlackjackRules();
            break;
        case '2':
            balance = 1000; // Start fresh with default balance
            playGame(balance);
            break;
        case '3':
            balance = loadBalance();
            playGame(balance);
            break;
        case '4':
            char saveChoice;
            cout << "Would you like to save your game before exiting? (Y/N): ";
            cin >> saveChoice;
            if (saveChoice == 'Y' || saveChoice == 'y')
            {
                saveBalance(balance);
            }
            gameRunning = false;
            cout << "Exiting the game.\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
