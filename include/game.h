#pragma once 

#include <stdio.h>
#include <vector>
#include "player.h"
#include "dealer.h"
#include "deck.h"

enum OUTCOME {WIN, LOSE, BLACKJACK, PUSH, BUST, NONE};

class Game
{
    public:
    Game();
    void Play();
   
    
    private:
        Player m_player;
        Dealer m_dealer;
        Deck m_deck;
        int m_startHands = 0;
        std::vector<bool> m_isHandBust;
        bool m_continuePlay = true;

        void DealInitialCards();
        void PlayRound();
        int InputBet(int handNumber);
        void InputStartHands();
        void ContinuePlay();
        bool ShouldDealerPlay();
        void DelayConsoleOutput(int milliseconds = 1000) const;
        OUTCOME DetermineOutcome(const Hand& playerHand, const Hand& dealerHand);
        int CalculateReward(OUTCOME outcome, int bet, int handBumber);
};