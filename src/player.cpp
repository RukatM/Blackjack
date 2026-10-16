#include <iostream>
#include "player.h"
#include "input.h"

Player::Player(int money):m_money(money){}

int Player::GetMoney() const
{
    return m_money;
}

void Player::RecieveReward(int reward)
{
    m_money += reward;
}

const Hand &Player::GetHand(size_t index) const
{
    return m_hands.at(index);
}

size_t Player::GetNumberOfHands() const
{
    return m_hands.size();
}

void Player::InputMoney()
{
    while (true)
    {
        std::cout << "\nPodaj liczbę pieniędzy do wpłaty:\n";
        int deposit = ReadInt();

        if (deposit > 0)
        {
            m_money += deposit;
            return;                      
        }
        std::cout << "Kwota musi być większa od 0.\n";
    }
}

bool Player::PlaceBet(int bet)
{
    if (bet <= 0 || bet > m_money) return false;  
    m_money -= bet;
    return true;
}

void Player::AddHand(int bet)
{
    m_hands.push_back(Hand(bet));
}

void Player::AddCard(const Card &card, size_t index)
{
    m_hands.at(index).AddCard(card);
}

void Player::Double(size_t index)
{
    m_hands.at(index).Double();
}

void Player::ResetHands()
{
    m_hands.clear();
}
