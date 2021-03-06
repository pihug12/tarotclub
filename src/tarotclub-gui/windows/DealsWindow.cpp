/*=============================================================================
 * TarotClub - DealsWindow.cpp
 *=============================================================================
 * Display previously played deals
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

#include <QMessageBox>
#include "DealsWindow.h"
#include "Deck.h"
#include "System.h"
#include "time.h"

/*****************************************************************************/
DealsWindow::DealsWindow(QWidget *parent)
    : QDialog(parent)
{
    mComboBox = new QComboBox(this);
    mOkButton = new QPushButton(tr("&Close"), this);

    mView = new QGraphicsView(&mScene);
    mLayout = new QVBoxLayout(this);

    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(new QLabel(tr("Choose a previously played deal to display.")));
    layout1->addWidget(mComboBox);
    layout1->addStretch();

    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addStretch();
    layout2->addWidget(mOkButton);

    mLayout->addLayout(layout1);
    mLayout->addWidget(mView);
    mLayout->addLayout(layout2);

    mText = new QGraphicsSimpleTextItem();
    mScene.addItem(mText);
    QFont font;
    font.setBold(true);
    mText->setFont(font);
    mText->setBrush(QBrush(QColor(Qt::white)));
    mText->setX(0);
    mText->setY(50);
    mText->setText("Discard:");

    connect(mOkButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(mComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &DealsWindow::slotActivated);
}
/*****************************************************************************/
DealsWindow::~DealsWindow()
{

}
/*****************************************************************************/
void DealsWindow::slotActivated(int index)
{
    QString fileName = QString(System::GamePath().c_str()) + mList.at(index).fileName();
    if (mDeal.LoadGameDealLog(fileName.toStdString()))
    {
        qreal y = 0;
        qreal limit_x = 700;

        // Show discard
        DisplayDeck(mDeal.GetDiscard(), 100, 0);

        qreal x = 0;
        y = 115;
        // Show played tricks
        for (std::uint8_t i = 0U; i < Tarot::NumberOfCardsInHand(4U); i++)
        {
             Deck trick = mDeal.GetTrick(i, 4U);
             DisplayDeck(trick, x, y);

             // Space between horizontal tricks
             x += 60 + 35 * trick.Size();

             if (x >= limit_x)
             {
                 x = 0;
                 y += 115;
             }
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Loading error"), tr("Cannot load the file: ") + fileName);
    }
}
/*****************************************************************************/
void DealsWindow::DisplayDeck(const Deck &deck, qreal x, qreal y)
{
    qreal z = 0;
    for (Deck::ConstIterator iter = deck.begin(); iter != deck.end(); ++iter)
    {
       std::string name = "c" + iter->ToString();
       for (int j = 0U; j < mCardsPics.size(); j++)
       {
           QGraphicsSvgItem *item = mCardsPics[j];
           if (item->elementId().toStdString() == name)
           {
               item->setPos(x, y);
               item->setZValue(z++);
           }
       }
       x += 35;
    }

}
/*****************************************************************************/
void DealsWindow::Initialize()
{
    mDeck.load(QString(":cards/minideck.svg"));
    Deck deck;
    deck.CreateTarotDeck();
    mCardsPics.clear();

    mScene.setBackgroundBrush(QColor("#004f00"));

    qreal x = 0;
    qreal y = 115;
    for (Deck::ConstIterator iter = deck.begin(); iter != deck.end(); ++iter)
    {
        QGraphicsSvgItem *item = new QGraphicsSvgItem();
        item->setElementId("c" + QString(iter->ToString().c_str()));
        item->setSharedRenderer(&mDeck);
        item->setPos(x, y);
        item->show();

        mCardsPics.append(item);
        mScene.addItem(item);

        qreal limit = 14;
        if (iter->GetSuit() == Card::TRUMPS)
        {
            limit = 13;
        }

        if (iter->GetValue() == limit)
        {
            x = 0;
            y += 115;
        }
        else
        {
            x += 65;
        }
    }
}
/*****************************************************************************/
void DealsWindow::RefreshDeals()
{
    QDir dir(QString(System::GamePath().c_str()));
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    mList = dir.entryInfoList();
    mComboBox->clear();

    for (int i = 0; i < mList.size(); i++)
    {
        // Filename format is: deal_result_2014-10-12.153826.json
        // We have to extract the date and time "%Y-%m-%d.%H%M%S"

        // 1. get the filename and remove .json
        QString dateTime = mList.at(i).fileName();
        dateTime.chop(5);

        // 3. Fill the combo box
        mComboBox->addItem(dateTime.right(17));
    }

    // Display the last deal, if available
    int count = mComboBox->count();
    if (count > 0)
    {
        mComboBox->setCurrentIndex(count-1);
        // Manually call the combo box event to display the deal
        slotActivated(mComboBox->currentIndex());
    }
}

//=============================================================================
// Fin du fichier DealsWindow.cpp
//=============================================================================
