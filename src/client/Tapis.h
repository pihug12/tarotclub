/*=============================================================================
 * TarotClub - Tapis.h
 *=============================================================================
 * visual game contents
 *=============================================================================
 * TarotClub ( http://www.tarotclub.fr ) - This file is part of TarotClub
 * Copyright (C) 2003-2999 - Anthony Rabine
 * anthony@tarotclub.fr
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 *=============================================================================
 */

#ifndef TAPIS_H
#define TAPIS_H

// Qt includes
#include <QtSvg>
#include <QtGui>
#include <QVector>
#include <QList>
#include <QTemporaryFile>

// Game includes
#include "../defines.h"
#include "../Card.h"
#include "TextBox.h"

/*****************************************************************************/
class GfxCard : public QGraphicsSvgItem
{

public:
   GfxCard ( const QString & fileName, QGraphicsItem * parent = 0 );

   enum { Type = UserType + 1 };
   int type() const;
};


/*****************************************************************************/
class Tapis : public QGraphicsView
{

   Q_OBJECT

private:
   Filter filter;
   QVector<GfxCard *> cardsPics;
   QGraphicsScene scene;

   // Graphiques
   PlayerBox      *btNord;
   PlayerBox      *btOuest;
   PlayerBox      *btSud;
   PlayerBox      *btEst;
   PlayerBox      *btNordOuest;

   TextBox        *enchNord;
   TextBox        *enchOuest;
   TextBox        *enchSud;
   TextBox        *enchEst;
   TextBox        *enchNordOuest;

   QGroupBox      *groupBoutons;
   QPushButton    *boutonPasse;
   QPushButton    *boutonPrise;
   QPushButton    *boutonGarde;
   QPushButton    *boutonGardeSans;
   QPushButton    *boutonGardeContre;
   QCheckBox      *chelem;
   QPushButton    *boutonAccepterChien;
   QPushButton    *boutonPresenterPoignee;

protected:
   void  mousePressEvent( QMouseEvent *e );
   void  mouseMoveEvent( QMouseEvent * e );
   void  resizeEvent( QResizeEvent * );

public:
   Tapis(QWidget *parent);

   void setCursorType( CursorType t );
   void setText(Place p, const QString &txt);
   void setAvatar(Place p, const QString &file);
   void setFilter( Filter );
   void setNbPlayers(int n);
   void setBackground(const QString &fichier);
   void setAccepterChienVisible(bool v);
   void setCardScale(float factor);

   GfxCard *getGfxCard(int i);
   Card *getObjectCard(GfxCard *gc);

   int loadCards(GameOptions *opt);
   void colorisePreneur( Place );
   Place retournePlace( Place origine, Place place_absolue );
   void printNames( Identity *identities, Place place );
   void afficheSelection( Place );
   void afficheCarte( GfxCard *, Place );
   void cacheEncheres();
   void cacheBoutons();
   void showAvatars( bool b, int nb_players );
   void razTapis();

public slots:
   void slotBoutton1();
   void slotBoutton2();
   void slotBoutton3();
   void slotBoutton4();
   void slotBoutton5();
   void slotAccepteChien();
   void slotAfficheBoutons( Contrat contrat );
   void slotAfficheEnchere(Place enchereur,Contrat cont);

signals:
   void sgnlViewportClicked();
   void sgnlClickCard( GfxCard * );
   void sgnlMoveCursor( GfxCard * );
   void sgnlContrat(Contrat c);
   void sgnlAccepteChien();
};

#endif // TAPIS_H

//=============================================================================
// End of file Tapis.h
//=============================================================================
