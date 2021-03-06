/*=============================================================================
 * TarotClub - LobbyDock.h
 *=============================================================================
 * Lobby window, includes a chat area and the tables available on the server
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

#ifndef LOBBY_DOCK_H
#define LOBBY_DOCK_H

#include <QDockWidget>
#include <QTcpSocket>
#include <QMap>
#include "ClientConfig.h"
#include "ui_LobbyUI.h"

/*****************************************************************************/
class LobbyDock : public QDockWidget
{
    Q_OBJECT

public:
    LobbyDock(QWidget *parent);

    void Initialize();
    void SetPlayersNames(const QMap<uint32_t, Identity> &players);
    void SetTableStatus(std::uint32_t tableId, bool status);
    void DisconnectedFromServer();
    void SetTables(const QMap<QString, uint32_t> &tableList);
    void SetServersList(const std::vector<ServerInfo> &servers);
    void SystemMessage(const QString &message);

signals:
    void sigEmitMessage(const QString &);
    void sigConnect(QString, std::uint16_t);
    void sigDisconnect();
    void sigJoinTable(std::uint32_t);
    void sigQuitTable(std::uint32_t);

public slots:
    void slotChatMessage(QString message);

private slots:
    void slotConnect();
    void slotDisconnect();
    void slotJoin();
    void slotQuit();
    void slotReturnPressed();

private:
    Ui::LobbyUI  ui;

    QMap<QString, std::uint32_t> mTableList;    // key = table name, value = table ID
    QMap<std::uint32_t, Identity> mPlayerList; // key = user name, value = uuid
    std::vector<ServerInfo> mServerList;
    bool mConnected;

    void UpdateServersList();
};


#endif // LOBBY_DOCK_H

//=============================================================================
// End of file LobbyDock.h
//=============================================================================
