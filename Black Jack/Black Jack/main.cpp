#include <iostream>
#include <array>
#include <numeric>
#include <random>
#include <string>



enum class CardSuit
{
    suit_club,
    suit_diamond,
    suit_heart,
    suit_spade,

    max_suits
};

enum class CardRank
{
    rank_2,
    rank_3,
    rank_4,
    rank_5,
    rank_6,
    rank_7,
    rank_8,
    rank_9,
    rank_10,
    rank_jack,
    rank_queen,
    rank_king,
    rank_ace,

    max_ranks
};

struct Card
{
    CardRank rank{};
    CardSuit suit{};
};

void printCard(const Card & card)
{
    switch (card.rank)
    {
    case CardRank::rank_2:      std::cout << '2';   break;
    case CardRank::rank_3:      std::cout << '3';   break;
    case CardRank::rank_4:      std::cout << '4';   break;
    case CardRank::rank_5:      std::cout << '5';   break;
    case CardRank::rank_6:      std::cout << '6';   break;
    case CardRank::rank_7:      std::cout << '7';   break;
    case CardRank::rank_8:      std::cout << '8';   break;
    case CardRank::rank_9:      std::cout << '9';   break;
    case CardRank::rank_10:     std::cout << 'T';   break;
    case CardRank::rank_jack:   std::cout << 'J';   break;
    case CardRank::rank_queen:  std::cout << 'Q';   break;
    case CardRank::rank_king:   std::cout << 'K';   break;
    case CardRank::rank_ace:    std::cout << 'A';   break;
    default:
        std::cout << '?';
        break;
    }

    switch (card.suit)
    {
    case CardSuit::suit_club:       std::cout << 'C';   break;
    case CardSuit::suit_diamond:    std::cout << 'D';   break;
    case CardSuit::suit_heart:      std::cout << 'H';   break;
    case CardSuit::suit_spade:      std::cout << 'S';   break;
    default:
        std::cout << '?';
        break;
    }
}

std::array<Card, 52> createDeck() {

    std::array<Card, 52> deck{};

    int i{ 0 };
    int j{ 0 };

    for (auto& element : deck) {
        element.rank = static_cast<CardRank>(i);
        element.suit = static_cast<CardSuit>(j);
        ++i;
        if (i == static_cast<int>(CardRank::max_ranks)) {
            ++j;
            i = 0;
        }
        
    }

    return deck;
}

void printDeck(const std::array<Card,52>& deck) {
    for (auto& element : deck) {
        printCard(element);
        std::cout << ' ';
    }
}

void shuffleDeck(std::array<Card, 52>& deck) {
    std::mt19937 seed{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
    std::shuffle(deck.begin(), deck.end(), seed);
}

int getCardValue(const Card& card) {
    
    if (card.rank <= CardRank::rank_10) {
        return static_cast<int>(card.rank) + 2;
    }

    switch (card.rank) {
    case(CardRank::rank_jack):
    case(CardRank::rank_queen):
    case(CardRank::rank_king):
        return 10;
    case(CardRank::rank_ace):
        return 11;
    default:
        return 0;
    }
}


bool playBlackjack(std::array<Card,52>& deck) {
    
    //current location in deck
    int deckCardNumber{ 0 };
    int user_card_num{ 0 };
    int dealer_card_num{ 0 };
    int user_score{ 0 };
    

    //dealer's hand
    std::array<Card, 21> dealer_hand{};
    std::cout << "Dealer's Hand: \n";
    for (; dealer_card_num < 2; ++dealer_card_num) {
        dealer_hand[dealer_card_num] = deck[deckCardNumber];
        ++deckCardNumber;
    }
    printCard(dealer_hand[0]); std::cout << " ??\n";
   

    //user's hand

    std::array<Card, 21> user_hand{};
    std::cout << "Your Hand: \n";
    for (; user_card_num < 2; ++user_card_num) {
        user_hand[user_card_num] = deck[deckCardNumber];
        printCard(user_hand[user_card_num]); 
        std::cout << " ";
        ++deckCardNumber;
        user_score += getCardValue(user_hand[user_card_num]);
    }
    std::cout << "\nYour Score Currently is: " << user_score;
    

    bool under_21{ true };
    while (under_21) {
        int ace_counter{ 0 };
        std::cout << "\nhit or stay? ";

        std::string HitorStay{};
        getline(std::cin >> std::ws, HitorStay);

        if (HitorStay == "Hit" || HitorStay == "hit") {
            ++user_card_num;
            user_hand[user_card_num] = deck[deckCardNumber];
            ++deckCardNumber;
            
            int temp_user_score{ 0 };
            for (int i{ 0 }; i < user_card_num; ++i) {
                printCard(user_hand[i]);
                std::cout << " ";
                temp_user_score += getCardValue(user_hand[i]);
                if (user_hand[i].rank == CardRank::rank_ace) {
                    ++ace_counter;
                }
            }
            
            user_score = temp_user_score;
            if (user_score > 21 && ace_counter >= 1) {
                for (int i{ 0 }; ace_counter > 0; --ace_counter) {
                    user_score -= 10;
                }
            }
            std::cout << "\nYour score is: "<< user_score;
            if (user_score > 21) {
                    std::cout << "\nBusted!";
                    return false;
                
            }
            else if (user_score == 21) {

                std::cout << "\nYou Win!";
                return true;
                    
            }
        }
        else if (HitorStay == "Stay" || HitorStay == "stay") {
            under_21 = false;
        }
    }

    bool dealer_under_21{ true };
    while (dealer_under_21) {
        int dealer_ace_counter{ 0 };
        int dealer_score{ 0 };
        std::cout << "\nDealer's Hand: \n";
        for (int i{ 0 }; i < dealer_card_num; ++i) {
            printCard(dealer_hand[i]);
            std::cout << " ";
            dealer_score += getCardValue(dealer_hand[i]);
            if (dealer_hand[i].rank == CardRank::rank_ace) {
                ++dealer_ace_counter;
            }
        }

        if (dealer_score > 21 && dealer_ace_counter >= 1) {
            for (int i{ 0 }; dealer_ace_counter > 0; --dealer_ace_counter) {
                dealer_score -= 10;
            }
        }

        std::cout << "\nDealer's Score is: " << dealer_score;


        if (dealer_score > 21) {
            std::cout << "\nDealer Bust! You Win!";
            return true;
        } 
        else if (dealer_score == 21) {
            std::cout << "\nYou Lost!";
            return true;
        } 
        else if (dealer_score >= 18 && dealer_score < 21) {
            if (user_score > dealer_score) {
                std::cout << "\nYou Win!";
                return true;
            }
            else if (user_score == dealer_score) {
                std::cout << "\nTie!";
                return true;
            }
            else if (user_score < dealer_score) {
                std::cout << "\n You Lose!";
                return false;
            }
        }
        else if (dealer_score < 18) {
            ++dealer_card_num;
            dealer_hand[dealer_card_num] = deck[deckCardNumber];
            ++deckCardNumber;
        }
    }

    return true;
}


int main() {

    std::array<Card, 52> deck{ createDeck() };
    bool again{ true };
    std::string yesorno{};

    while (again) {
        shuffleDeck(deck);
        playBlackjack(deck);
        std::cout << "\nWould you like to play again? yes/no\n";
        getline(std::cin >> std::ws, yesorno);
        if (yesorno == "yes") {

        }
        else if (yesorno == "no") {
            again = false;
        }
    }
    
	return 0;
}