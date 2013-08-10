/*=============================================================================
 * TarotClub - beginner.js
 *=============================================================================
 * Beginner's level artificial intelligence
 *=============================================================================
 * TarotClub ( http://www.tarotclub.fr ) - This file is part of TarotClub
 * Copyright (C) 2003-2999 - Anthony Rabine
 * anthony@tarotclub.fr
 *
 * TarotClub is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TarotClub is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TarotClub.  If not, see <http://www.gnu.org/licenses/>.
 *
 *=============================================================================
 */

var CurrentGame = new TarotLib.Game();

// ****************************************************************************
// TAROTCLUB API CALLED BY C++
// ****************************************************************************

/**
 * @brief This function is called when the player has been accepted around the table
 *
 * @param[in] place The place assigned by the server around the table
 */
function EnterGame(place)
{
	CurrentGame.myPlace = place;
}

/**
 * @brief This function is called when the cards are delt to players
 *
 * @param[in] cards Cards of the bots (18 for 4 players), string format 
 */
function ReceiveCards(cards)
{
    CurrentGame.initialize();
    CurrentGame.myDeck.setCards(cards);
}

/**
 * @brief This function is called when the bot must declare a bid
 * 
 * @return The bid announced, use the definition forrmat
 * @see Variable 'Contract'
 */
function AnnounceBid()
{
   total = 0;
   
   systemPrint("The bot is announcing a bid.");
   
   // We start looking at bouts, each of them increase the total value of points
   if( TStats.hasVingtEtUn() === true ) {
      total += 9;
   }
   if( TStats.hasExcuse === true ) {
      total += 7;
   }
   if( TStats.hasPetit === true ) {
      if( TStats.getNumberOfAtouts === 5 ) {
         total += 5;
      } else if( TStats.getNumberOfAtouts === 6 || TStats.getNumberOfAtouts === 7 ) {
         total += 7;
      } else if( TStats.getNumberOfAtouts > 7 ) {
         total += 8;
      }
   }

   // Each atout counts two points
   // Each major atout counts one more point
   total += TStats.getNumberOfAtouts() * 2;
   total += TStats.getNumberOfAtoutsMajeurs() * 2;
   total += TStats.getNumberOfRois() * 6;
   total += TStats.getNumberOfDames() * 3;
   total += TStats.getNumberOfCavaliers() * 2;
   total += TStats.getNumberOfValets();
   total += TStats.getNumberOfMariages();
   total += TStats.getNumberOfLongues() * 5;
   total += TStats.getNumberOfCoupes() * 5;
   total += TStats.getNumberOfSingletons() * 3;
   total += TStats.getNumberOfSequences() * 4;

   // We decide on a bid depending of thresholds
   if( total <= 35 ) {
      cont = TarotLib.Contract.PASS;
   } else if( total >= 36  && total <= 50 ) {
      cont = TarotLib.Contract.TAKE;
   } else if( total >= 51  && total <= 65 ) {
      cont = TarotLib.Contract.GUARD;
   } else if( total >= 66  && total <= 75 ) {
      cont = TarotLib.Contract.GUARD_WITHOUT;
   } else {
      cont = TarotLib.Contract.GUARD_AGAINST;
   }
   return cont;
}

/**
 * @brief This function is called for the bot to build the discard
 * 
 * @param[in] dog Dog deck, string format
 */
function BuildDiscard(dog)
{

}

/**
 * @brief This function is called when a player as declared a handle
 * 
 * @param[in] handle declared, card list in string format
 * @param[in] team, Attack or Defense
 */
function ShowHandle(handle, team)
{
}

/**
 * @brief This function is called to indicate a new game
 * 
 * Informational purpose, the script should memorize the arguments for later use
 *
 * @param[in] taker Indicates the place of the taker
 * @param[in] contract Indicates the contract announed by the taker
 */
function StartGame(taker, contract)
{
	CurrentGame.taker = taker;
	CurrentGame.contract = contract;
}

/**
 * @brief This function is called when the bot must play a card
 * 
 * The counter in parameter is useful to calculate the current position
 * in the current turn and to know what are the cards played by the 
 * previous players. 
 *
 * Use this counter along with the main deck of cards to analyse the game.
 *
 * Notice that in case of a non-valid card returned, the game engine will take
 * a random valid card in the bot's deck.
 *
 * @param[in] gameCounter Current game counter, incremented at each player's turn  [1..72]
 * @return The card played, string format
 */
function PlayCard()
{
//	debugger; // enable this line to generate a breakpoint
	var cardName;
	
    if (CurrentGame.taker === CurrentGame.myPlace) {
        cardName = CurrentGame.playAttackStrategy();
	} else {
        cardName = CurrentGame.playDefenseStrategy();
	}

	return cardName;
}

/**
 * @brief This function is called when any card is played (even bot's ones)
 *
 * @param[in] cardName The name of the card, string format
 * @param[in] place The place of the player's card just played
 */
function PlayedCard(cardName, place)
{
    CurrentGame.setPlayedCard(cardName, parseInt(place));
}


// End of file