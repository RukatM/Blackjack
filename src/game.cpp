#include <iostream>
#include <chrono>
#include <thread>
#include "game.h"
#include "input.h"

Game::Game()
{
    m_player = Player();
    m_dealer = Dealer();
    m_deck = Deck();
}

void Game::Play() 
{
    std::cout<<"Witaj w grze w blackjacka!\n";
    m_player.InputMoney();
    while(m_continuePlay)
    {
        InputStartHands();
        DealInitialCards();
        PlayRound();
        ContinuePlay();
    }
}

void Game::DealInitialCards()
{   m_player.ResetHands();
    m_dealer.ResetHand();
    for(int i = 0; i < m_startHands; ++i)
    {
        int bet = InputBet(i+1);
        m_player.AddHand(bet);
    }
    
    bool secondHiddenCard = false;

    for(int i = 0; i < 2; ++i)
    {
        std::cout<<"\nRozdaję kar†ę "<<i+1<<std::endl;

        for(int j = 0; j < m_startHands; ++j)
        {
            Card playerCard = m_deck.DrawCard();
            m_player.AddCard(playerCard, j);
            std::cout<<"Ręka "<<j + 1<<": ";
            m_player.GetHand(j).PrintHand();
            DelayConsoleOutput();
        }

        std::cout<<"\nRozdaję kar†ę krupierowi"<<std::endl;
        Card dealerCard = m_deck.DrawCard();
        m_dealer.AddCard(dealerCard);
        
        if(i == 1) secondHiddenCard = true;
        
        m_dealer.GetHand().PrintHand(secondHiddenCard);

        DelayConsoleOutput();
    }
}

int Game::InputBet(int handNumber)
{
    while (true)
    {
        std::cout << "Podaj wartość zakładu ręki nr. " << handNumber << ":\n";
        int bet = ReadInt();

        if (m_player.PlaceBet(bet))
            return bet;                 

        std::cout << "Nieprawidłowy zakład (saldo: " << m_player.GetMoney() << ").\n";
        std::cout << "1 -> dopłać pieniądze, 2 -> podaj nowy zakład:\n";
        if (ReadInt() == 1) m_player.InputMoney();
    }
}

void Game::InputStartHands()
{
    while (true)
    {
        std::cout << "Podaj liczbę zakładów:\n";
        m_startHands = ReadInt();       

        if(m_startHands > 5) 
        {
            std::cout<<"Liczba zakładów za duża. Maskymalnie 5. Spróbuj jeszcze raz.\n\n";
        }
        else if(m_startHands <= 0)
        {
            std::cout<<"Liczba zakładów musi być większa niż 0. Sprobuj jeszcze raz.\n\n";
        }
        else return;
        
        
    }
}

void Game::PlayRound()
{
    for(size_t i = 0; i < m_player.GetNumberOfHands(); ++i)
    {
        const Hand& playerHand = m_player.GetHand(i);
        
        if(playerHand.IsBlackjack() == true) 
        {
            std::cout<<"\nRęka "<<i + 1<<": ";
            playerHand.PrintHand();
            std::cout << "BLACKJACK\n" <<"\n";
            continue;
        }

        bool isDouble = false;
        while(true)
        {
            std::cout<<"\nRęka "<<i + 1<<": ";
            playerHand.PrintHand();
            

            if(playerHand.IsBust())
            {
                std::cout << "\nBUST\n" <<"\n";
                break;
            }
            
            if(isDouble) break; 

            bool canDouble = (playerHand.GetNumberofCards() == 2 && m_player.GetMoney() >= playerHand.GetBet());
            if(canDouble) std::cout<<"\nWbierz ruch: 1 -> HIT, 2 -> STAND, 3 -> DOUBLE:\n";
            else std::cout<<"\nWbierz ruch: 1 -> HIT, 2 -> STAND:\n";
            
            int choice = 0;
            while (true)
            {
                choice = ReadInt();
                if (choice == 1 || choice == 2 || (choice == 3 && canDouble)) break;
                std::cout << "Niepoprawny wybór.\n";
            }

            if (choice == 2) 
            { 
                std::cout << "STAND\n\n"; 
                break; 
            }
            if (choice == 3) 
            {
                isDouble = true;
                m_player.PlaceBet(playerHand.GetBet());
                m_player.Double(i);
                std::cout << "DOUBLE\n\n";
            } 
            DelayConsoleOutput();
            m_player.AddCard(m_deck.DrawCard(), i);
        }
        DelayConsoleOutput();
    }
    
    
    const Hand& dealerHand = m_dealer.GetHand();
    std::cout<<"\nKrupier odkrywa kartę...\n";
    DelayConsoleOutput(2000);
    dealerHand.PrintHand();    
    DelayConsoleOutput(2000);
    
    if(ShouldDealerPlay())
    {
        while(m_dealer.ShouldHit())
        {
        m_dealer.AddCard(m_deck.DrawCard());
        std::cout<<"\n";
        dealerHand.PrintHand();    
        DelayConsoleOutput(2000);
        }

        for(size_t i = 0; i < m_player.GetNumberOfHands(); ++i)
        {
        const Hand& playerHand = m_player.GetHand(i);
        OUTCOME outcome = DetermineOutcome(playerHand, dealerHand);
        int reward = CalculateReward(outcome, playerHand.GetBet(), i+1);
        if(reward > 0) m_player.RecieveReward(reward);
        DelayConsoleOutput();
        } 

        DelayConsoleOutput();
    }
    std::cout<<"\nTwoje saldo wynosi: "<<m_player.GetMoney()<<"\n";    
}

void Game::ContinuePlay()
{
    while (true)
    {
        int choice = 0;
        std::cout << "Czy chcesz kontynuować?\n1 -> TAK! \n2 -> NIE :( \n3 -> Dopłać pieniądze i kontynuuj\n";
        choice = ReadInt();       

        if(choice == 1 || choice == 2)
        {
            if (choice == 2) m_continuePlay = false;   
            break;
        }
        else if(choice == 3)
        {
            m_player.InputMoney();
            std::cout<<"\nTwoje Saldo wynosi: "<<m_player.GetMoney()<<"\n\n";
            break;
        }
        else
        {
             std::cout<<"Wybrano niedozwoloną opcję. Sprobuj jeszcze raz.\n";
        }
    }
}

bool Game::ShouldDealerPlay()
{
    for (size_t i = 0; i < m_player.GetNumberOfHands(); ++i)
    {
        const Hand& h = m_player.GetHand(i);
        if (!h.IsBust() && !h.IsBlackjack()) return true;
    }
    return false;
}

void Game::DelayConsoleOutput(int milliseconds) const
{
    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

OUTCOME Game::DetermineOutcome(const Hand& playerHand, const Hand& dealerHand)
{
        if(playerHand.IsBust()) return BUST;
        if(playerHand.IsBlackjack()) return dealerHand.IsBlackjack() ? PUSH : BLACKJACK;
        if(dealerHand.IsBlackjack()) return LOSE;
        if(dealerHand.IsBust()) return WIN;
        if(playerHand.GetScore() > dealerHand.GetScore()) return WIN;
        if(playerHand.GetScore() == dealerHand.GetScore()) return PUSH;
        else return LOSE;       
}

int Game::CalculateReward(OUTCOME outcome, int bet, int handNumber)
{
    int reward = 0;
    
    switch (outcome)
    {
        case WIN:
            reward = bet*2;
            std::cout<<"\nRęka "<<handNumber<<" WYGRAŁEŚ!\n";
            break;
        
        case BLACKJACK:
            reward = bet + bet * 3 / 2;
            std::cout<<"\nRęka "<<handNumber<<" BLACKJACK\n";
            break;

        case PUSH:
            reward = bet;
            std::cout<<"\nRęka "<<handNumber<<" PUSH\n";
            break;

        case BUST:
            reward = 0;
            std::cout<<"\nRęka "<<handNumber<<" Przegrałeś\n";
            break;

        case LOSE:
            reward = 0;
            std::cout<<"\nRęka "<<handNumber<<" Przegrałeś\n";
            break;

        case NONE:
            break;

        default:
            break;
    }
    return reward;
}
