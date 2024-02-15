#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Define a structure for a card
struct Card {
    std::string suit;
    std::string value;
};

// PreDefine a prototype function for allowing it to be called before its initial run
void displayUI(std::vector<std::string>& dealersCards,
    std::vector<std::string>& playersCardsStr, std::vector<Card> playersCards, int playersTotal, std::vector<Card>& deck, int bet);

// Function to create a deck of 52 standard cards
std::vector<Card> createDeck() {
    std::vector<Card> deck;
    std::string suits[] = { "H", "D", "C", "S" };
    std::string values[] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

    for (const auto& suit : suits) {
        for (const auto& value : values) {
            Card card = { suit, value };
            deck.push_back(card);
        }
    } 


    return deck;
}


void gameOver() {

}

// Function to deal two cards from the deck
// Update the return type of dealInitialCards to return vectors of Card objects
std::vector<Card> dealInitialCards(std::vector<Card>& deck) {
    std::vector<Card> initialCards;
    for (int i = 0; i < 2; ++i) {
        int randomIndex = rand() % deck.size();
        initialCards.push_back(deck[randomIndex]);
        deck.erase(deck.begin() + randomIndex);
    }
    return initialCards;
}

std::vector<Card> dealCards(std::vector<Card>& deck) {
    std::vector<Card> dealtCard;
    int randomIndex = rand() % deck.size();
    dealtCard.push_back(deck[randomIndex]);
    deck.erase(deck.begin() + randomIndex);

    return dealtCard;
}

// a few global variables
int choice;
bool firstRun = true;
int playersMoney = 1000;

// Function declarations
void clearScreen();
void drawLine();
void centeredText(const std::string& text);
void showCards(const std::string& title, const std::vector<std::string>& cards, bool hideFirst);
int cardValueToInt(const std::string& value);
int calculateTotal(const std::vector<Card>& cards);
void showOptions();
void drawBottomRow();
void getUserChoice(std::vector<Card>& deck, std::vector<Card>& playersCards, std::vector<std::string>& playersCardsStr, std::vector<std::string>& dealersCards, int& playersTotal, int bet);
void dealerPlays(std::vector<Card>& deck, std::vector<Card>& dealersCards, std::vector<std::string>& dealersCardsStr);
std::vector<Card> convertToCardVector(const std::vector<std::string>& cardStrings, const std::vector<Card>& deck);

// The heart of the program
void displayUI(std::vector<std::string>& dealersCards,
    std::vector<std::string>& playersCardsStr, std::vector<Card> playersCards, int playersTotal, std::vector<Card>& deck, int bet)
{
    if (firstRun == true) {
        firstRun = false;
        while (choice != 6 && playersTotal <= 22)
        {
            int playersTotal = calculateTotal(playersCards);
            clearScreen();
            drawLine();
            centeredText("Blackjack Game");
            drawLine();
            showCards("Dealer's Cards:", dealersCards, true);
            std::cout << "| Current Bet: " << bet << std::endl;
            showCards("Your Cards:", playersCardsStr, false);
            std::cout << "| Total: " << playersTotal << "\n";
            drawLine();

            std::cout << "Your current money: " << playersMoney << std::endl;
            std::cout << "Enter your bet: ";
            std::cin >> bet;
            while (bet <= 0 || bet > playersMoney) {
                std::cout << "Invalid bet. Enter your bet: ";
                std::cin >> bet;
            }
            playersMoney -= bet;
            drawBottomRow();
            showOptions();
            drawBottomRow();

            getUserChoice(deck, playersCards, playersCardsStr, dealersCards, playersTotal, bet);
        }

    }
    else {
        while (choice != 6 && playersTotal <= 22)
        {
            int playersTotal = calculateTotal(playersCards);
            clearScreen();
            drawLine();
            centeredText("Blackjack Game");
            drawLine();
            showCards("Dealer's Cards:", dealersCards, true);
            std::cout << "| Current Bet: " << bet << std::endl;
            std::cout << "| Remaining Cash: " << playersMoney << std::endl;
            showCards("Your Cards:", playersCardsStr, false);
            std::cout << "| Total: " << playersTotal << "\n";
            drawLine();
            showOptions();
            drawBottomRow();

            getUserChoice(deck, playersCards, playersCardsStr, dealersCards, playersTotal, bet);
        }
    }
}

// Utility Functions
void clearScreen()
{
    std::system("CLS");
}

void drawLine()
{
    std::cout << "+--------------------------------------------------+\n";
}

void centeredText(const std::string& text)
{
    int len = text.length();
    int pad = (50 - len) / 2;
    std::cout << "|";
    for (int i = 0; i < pad - 1; ++i) std::cout << " ";
    std::cout << text;
    for (int i = 0; i < pad - 1; ++i) std::cout << " ";
    if (len % 2 != 0) std::cout << " ";
    std::cout << "|\n";
}

void showCards(const std::string& title, const std::vector<std::string>& cards, bool hideFirst)
{
    std::cout << "| " << title << "\n| ";

    for (size_t i = 0; i < cards.size(); ++i)
    {
        if (hideFirst && i == 0)
        {
            std::cout << "[??] ";
        }
        else
        {
            std::cout << "[" << cards[i] << "] ";
        }
    }

    std::cout << "\n";
}

int cardValueToInt(const std::string& value) {
    if (value == "2") return 2;
    else if (value == "3") return 3;
    else if (value == "4") return 4;
    else if (value == "5") return 5;
    else if (value == "6") return 6;
    else if (value == "7") return 7;
    else if (value == "8") return 8;
    else if (value == "9") return 9;
    else if (value == "10" || value == "J" || value == "Q" || value == "K") return 10;
    else if (value == "A") return 11;
    else return 0;
}

int calculateTotal(const std::vector<Card>& cards) {
    int total = 0;
    int numAces = 0;

    for (const auto& card : cards) {
        total += cardValueToInt(card.value);
        if (card.value == "A") {
            numAces++;
        }
    }

    while (total > 21 && numAces > 0) {
        total -= 10;
        numAces--;
    }

    return total;
}

void showOptions()
{
    std::cout << "| Options:                                         |\n";
    std::cout << "| 1. Hit                                           |\n";
    std::cout << "| 2. Stand                                         |\n";
    std::cout << "| 3. Double Down                                   |\n";
    std::cout << "| 4. Surrender                                     |\n";
    std::cout << "| 5. Split                                         |\n";
    std::cout << "| 6. Quit                                          |\n";
}

void drawBottomRow()
{
    std::cout << "+--------------------------------------------------+\n";
    std::cout << "| Enter your choice: ";
}

void dealerPlays(std::vector<Card>& deck, std::vector<Card>& dealersCards, std::vector<Card>& playersCards) {
    // Reveal the dealer's first card if it's not already revealed
    if (dealersCards.size() > 0 && dealersCards[0].value == "??") {
        dealersCards[0] = playersCards[0];
    }

    // Update the dealer's total
    int dealersTotal = calculateTotal(dealersCards);

    // Dealer draws cards until the total is 17 or higher
    while (dealersTotal < 17) {
        std::vector<Card> newDealerCards = dealCards(deck);
        dealersCards.push_back(newDealerCards[0]); // Add the new card to the dealer's hand

        // Update the dealer's total
        dealersTotal = calculateTotal(dealersCards);
    }
}


void getUserChoice(std::vector<Card>& deck, std::vector<Card>& playersCards, std::vector<std::string>& playersCardsStr, std::vector<std::string>& dealersCards, int& playersTotal, int bet) {
    while (!(std::cin >> choice)) {
        std::cout << "\n| Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::vector<Card> newCards = dealCards(deck);
    switch (choice) {
    case 1:
        playersCards.insert(playersCards.end(), newCards.begin(), newCards.end());

        playersCardsStr.clear();
        for (const auto& card : playersCards) {
            playersCardsStr.push_back(card.value + "" + card.suit);
        }

        displayUI(dealersCards, playersCardsStr, playersCards, playersTotal, deck, bet);
        break;
    case 2:
        // convert the dealersCards vector string into a vector card
        std::vector<Card>dealerCards = convertToCardVector(dealersCards, deck);

        dealerPlays(deck, dealerCards, playersCards);
    }
    clearScreen();
}

std::vector<Card> convertToCardVector(const std::vector<std::string>& cardStrings, const std::vector<Card>& deck) {
    std::vector<Card> cardVector;
    for (const auto& cardString : cardStrings) {
        std::string value = cardString.substr(0, cardString.length() - 1); // Extract value
        std::string suit = cardString.substr(cardString.length() - 1); // Extract suit
        // Find the corresponding card in the deck
        for (const auto& card : deck) {
            if (card.value == value && card.suit == suit) {
                cardVector.push_back(card);
                break;
            }
        }
    }
    return cardVector;
}

// Main function
int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    std::vector<Card> deck = createDeck();
    std::vector<Card> playerCards = dealInitialCards(deck);
    std::vector<Card> dealerCards = dealInitialCards(deck);
    int playersTotal = calculateTotal(playerCards);
    int bet = 0;

    // Convert playerCards and dealerCards to vectors of strings
    std::vector<std::string> playerCardsStr;
    std::vector<std::string> dealerCardsStr;

    for (const auto& card : playerCards) {
        playerCardsStr.push_back(card.value + "" + card.suit);
    }

    for (const auto& card : dealerCards) {
        dealerCardsStr.push_back(card.value + "" + card.suit);
    }

    displayUI(dealerCardsStr, playerCardsStr, playerCards, playersTotal, deck, bet);

    return 0;
}