#include <iostream>
#include <string>
#include "card.h"


const std::string cardSuits[4] = {"HEART", "CLUB", "DIAMOND", "SPADE"};
const std::string cardValues[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

Card::Card(SUIT suit, VALUE value): m_suit(suit), m_value(value){}

SUIT Card::GetSuit() const 
{ 
    return m_suit; 
}

VALUE Card::GetValue() const
{
    return m_value;
}

void Card::PrintValue() const
{
    std::cout<<cardValues[m_value];
}
