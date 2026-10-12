#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

enum SUIT { HEART, CLUB, DIAMOND, SPADE}; 
enum VALUE {ACE, TWO, THREE, FOUR, FIVE, SIX,SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
const std::string cardSuits[4] = {"HEART", "CLUB", "DIAMOND", "SPADE"};
const std::string cardValues[13] = {"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING"};

class Card{
    public: 
    Card(SUIT suit, VALUE value): m_suit(suit), m_value(value){}

    VALUE GetValue(){
        return m_value;
    }
    SUIT GetSuit(){
        return m_suit;
    } 

    private:
    SUIT m_suit;
    VALUE m_value;


};

std::vector<Card> GenerateDeck()
{
    std::vector<Card> deck;
    deck.reserve(52);

    for(int i = 0; i < 4; ++i)
    {
        SUIT suit = static_cast<SUIT>(i);
        for(int j = 0; j < 13; ++j)
        {
            VALUE value = static_cast<VALUE>(j);
            
            deck.push_back(Card(suit,value));
        }
    }
    return deck;
}

void shuffleDeck(std::vector<Card>& deck, unsigned seed)
{
    auto rng = std::default_random_engine(seed);
    std::shuffle(std::begin(deck), std::end(deck), rng);
}


int main()
{
    std::vector<Card> deck = GenerateDeck();
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count(); 
    shuffleDeck(deck, seed);
    
    for (int i = 0; i < deck.size(); ++i){
        std::cout<<cardSuits[deck[i].GetSuit()]<<" "<<cardValues[deck[i].GetValue()]<<std::endl;
    }

    std::cout<<std::endl;


    shuffleDeck(deck, seed);

    for (int i = 0; i < deck.size(); ++i){
        std::cout<<cardSuits[deck[i].GetSuit()]<<" "<<cardValues[deck[i].GetValue()]<<std::endl;
    }
    return 0;
}
