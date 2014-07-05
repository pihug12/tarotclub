/*=============================================================================
 * TarotClub - ICardEvent.h
 *=============================================================================
 * Virtual class used to signal events related to graphic cards
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

#ifndef ICARDEVENT_H
#define ICARDEVENT_H

#include <cstdint>

class ICardEvent
{

public:
    virtual ~ICardEvent() {}

    /**
     * @brief CardClicked
     * @param card
     */
    virtual void CardClicked(std::uint8_t card, bool selected) = 0;

    /**
     * @brief CardHoverEnter
     * @param card
     */
    virtual void CardHoverEnter(std::uint8_t card) = 0;

    /**
     * @brief CardHoverLeave
     * @param card
     */
    virtual void CardHoverLeave(std::uint8_t card) = 0;
};

#endif // ICARDEVENT_H

//=============================================================================
// End of file ICardEvent.h
//=============================================================================