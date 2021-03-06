/*=============================================================================
 * TarotClub - ClientConfig.cpp
 *=============================================================================
 * Option file parameters management for the client configuration
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

#include <sstream>
#include "JsonReader.h"
#include "JsonWriter.h"
#include "ClientConfig.h"
#include "Log.h"
#include "System.h"

static const std::string CLIENT_CONFIG_VERSION  = "4"; // increase the version to force any incompatible update in the file structure
const std::string    ClientConfig::DEFAULT_CLIENT_CONFIG_FILE  = "tarotclub.json";

/*****************************************************************************/
ClientConfig::ClientConfig()
{
    // Initialize supported languages
    mLang.push_back("fr");
    mLang.push_back("en");
    mLoaded = false;
    mOptions = GetDefault();
}
/*****************************************************************************/
ClientOptions ClientConfig::GetOptions()
{
    if (!mLoaded)
    {
        mOptions = GetDefault();
    }
    return mOptions;
}
/*****************************************************************************/
std::string ClientConfig::GetLocale()
{
    if (!mLoaded)
    {
        mOptions = GetDefault();
    }
    if (mOptions.language >= mLang.size())
    {
        mOptions.language = 0U;
    }
    return mLang[mOptions.language];
}
/*****************************************************************************/
void ClientConfig::SetOptions(const ClientOptions &newOptions)
{
    mOptions = newOptions;
}
/*****************************************************************************/
bool ClientConfig::Load(const std::string &fileName)
{
    JsonValue json;

    bool ret = JsonReader::ParseFile(json, fileName);
    if (ret)
    {
        std::string stringval;
        std::int32_t intval;
        std::uint32_t unsignedVal;

        if (json.GetValue("version", stringval))
        {
            if (stringval == CLIENT_CONFIG_VERSION)
            {
                bool boolval;
                // The general strategy is to be tolerant on the values.
                // If they are not in the acceptable range, we set the default value
                // without throwing any error

                if (json.GetValue("show_avatars", boolval))
                {
                    mOptions.showAvatars = boolval;
                }

                if (json.GetValue("background_color", stringval))
                {
                    mOptions.backgroundColor = stringval;
                }

                if (json.GetValue("language", intval))
                {
                    mOptions.language = intval;
                    if (mOptions.language >= mLang.size())
                    {
                        mOptions.language = 0;
                    }
                }

                if (json.GetValue("delay_between_players", unsignedVal))
                {
                    if (unsignedVal > 9000U)
                    {
                        unsignedVal = DEFAULT_DELAY;
                    }
                    mOptions.timer = unsignedVal;
                }

                if (json.GetValue("delay_before_cleaning", unsignedVal))
                {
                    mOptions.delayBeforeCleaning = static_cast<std::uint16_t>(unsignedVal);
                    if (mOptions.delayBeforeCleaning > 5000U)
                    {
                        mOptions.delayBeforeCleaning = 5000U;
                    }
                }

                if (json.GetValue("click_to_clean", boolval))
                {
                    mOptions.clickToClean = boolval;
                }

                if (json.GetValue("cards_order", stringval))
                {
                    mOptions.cardsOrder = stringval;
                }

                // Player's identity
                if (json.GetValue("identity:name", stringval))
                {
                    mOptions.identity.nickname = stringval;
                }
                if (json.GetValue("identity:avatar", stringval))
                {
                    mOptions.identity.avatar = stringval;
                }
                if (json.GetValue("identity:gender", stringval))
                {
                    if (stringval == "female")
                    {
                        mOptions.identity.gender = Identity::cGenderFemale;
                    }
                    else
                    {
                        mOptions.identity.gender = Identity::cGenderMale;
                    }

                }

                // Load bot configuration
                for (std::uint32_t i = 1U; i < 4U; i++)
                {
                    Place p(i);
                    std::string botPos = p.ToString();
                    if (json.GetValue(botPos + ":name", stringval))
                    {
                        mOptions.bots[p].identity.nickname = stringval;
                    }
                    if (json.GetValue(botPos + ":avatar", stringval))
                    {
                        mOptions.bots[p].identity.avatar = stringval;
                    }
                    if (json.GetValue(botPos + ":gender", stringval))
                    {
                        if (stringval == "female")
                        {
                            mOptions.bots[p].identity.gender = Identity::cGenderFemale;
                        }
                        else
                        {
                            mOptions.bots[p].identity.gender = Identity::cGenderMale;
                        }

                    }

                    if (json.GetValue(botPos + ":bot_file_path", stringval))
                    {
                        mOptions.bots[p].scriptFilePath = stringval;
                    }
                }

                JsonValue servers = json.FindValue("servers");
                mOptions.serverList.clear();
                for (JsonArray::iterator iter = servers.GetArray().begin(); iter != servers.GetArray().end(); ++iter)
                {
                    if (iter->IsObject())
                    {
                        ServerInfo srv;

                        bool ret = iter->GetValue("address", srv.address);
                        if (ret) { ret = iter->GetValue("game_port", srv.game_tcp_port); }

                        if (ret)
                        {
                            mOptions.serverList.push_back(srv);
                        }
                    }
                }
            }
            else
            {
                TLogError("Wrong client configuration file version");
                ret = false;
            }
        }
        else
        {
            TLogError("Cannot read client configuration file version");
            ret = false;
        }
    }
    else
    {
        TLogError("Cannot open client configuration file");
    }

    if (!ret)
    {
        // Overwrite old file with default value
        mOptions = GetDefault();
        ret = Save(fileName);
    }

    mLoaded = true;
    return ret;
}
/*****************************************************************************/
bool ClientConfig::Save(const std::string &fileName)
{
    bool ret = true;

    JsonObject json;

    json.AddValue("version", CLIENT_CONFIG_VERSION);
    json.AddValue("show_avatars", mOptions.showAvatars);
    json.AddValue("background_color", mOptions.backgroundColor);
    json.AddValue("language", mOptions.language);
    json.AddValue("delay_between_players", mOptions.timer);
    json.AddValue("delay_before_cleaning", mOptions.delayBeforeCleaning);
    json.AddValue("click_to_clean", mOptions.clickToClean);
    json.AddValue("cards_order", mOptions.cardsOrder);

    JsonObject identity;
    identity.AddValue("name", mOptions.identity.nickname);

    // Make sure to use unix path separator
    Util::ReplaceCharacter(mOptions.identity.avatar, "\\", "/");

    identity.AddValue("avatar", mOptions.identity.avatar);
    std::string text;
    if (mOptions.identity.gender == Identity::cGenderMale)
    {
        text = "male";
    }
    else
    {
        text = "female";
    }
    identity.AddValue("gender", text);
    json.AddValue("identity", identity);

    // List of bots
    for (std::uint32_t i = 1U; i < 4U; i++)
    {
        Place p(i);
        JsonObject botObj;
        botObj.AddValue("name", mOptions.bots[p].identity.nickname);

        // Make sure to use unix path separator
        Util::ReplaceCharacter(mOptions.bots[p].identity.avatar, "\\", "/");

        botObj.AddValue("avatar", mOptions.bots[p].identity.avatar);
        std::string text;
        if (mOptions.bots[p].identity.gender == Identity::cGenderMale)
        {
            text = "male";
        }
        else
        {
            text = "female";
        }

        botObj.AddValue("gender", text);

        // Make sure to use unix path separator
        Util::ReplaceCharacter(mOptions.bots[p].scriptFilePath, "\\", "/");
        botObj.AddValue("bot_file_path", mOptions.bots[p].scriptFilePath);
        json.AddValue(p.ToString(), botObj);
    }

    // List of servers
    JsonArray servers; // = json.CreateArrayPair("servers");
    for (std::uint32_t i = 0U; i < mOptions.serverList.size(); i++)
    {
        ServerInfo inf =  mOptions.serverList[i];
        JsonObject srv; // = servers->CreateObject();
        srv.AddValue("address", inf.address);
        srv.AddValue("game_port", inf.game_tcp_port);
        servers.AddValue(srv);
    }
    json.AddValue("servers", servers);

    if (!JsonWriter::SaveToFile(json, fileName))
    {
        ret = false;
        TLogError("Saving client's configuration failed.");
    }
    return ret;
}
/*****************************************************************************/
ClientOptions ClientConfig::GetDefault()
{
    const ServerInfo DefaultServers[1U] = {
        {"tarotclub.fr", 4269 }
    };

    const std::uint32_t NumberOfServers = (sizeof(DefaultServers) / sizeof(ServerInfo));

    ClientOptions opt;

    opt.timer = DEFAULT_DELAY;
    opt.showAvatars = AVATARS_DEF;
    opt.backgroundColor = "#004f00";
    opt.language = 0;
    opt.deckFilePath = "default";
    opt.cardsOrder = "TCHDS";
    opt.delayBeforeCleaning = CLIENT_TIMER_DEF;
    opt.clickToClean = true;

    opt.identity.nickname = "Fry";
    opt.identity.avatar = ":/avatars/A02.png";
    opt.identity.gender = Identity::cGenderMale;

    for (std::uint32_t i = 0U; i < NumberOfServers; i++)
    {
        opt.serverList.push_back(DefaultServers[i]);
    }

    opt.bots[Place(Place::WEST)].identity.nickname = "Leela";
    opt.bots[Place(Place::WEST)].identity.avatar   = ":/avatars/FD05.png";
    opt.bots[Place(Place::WEST)].identity.gender   = Identity::cGenderFemale;
    opt.bots[Place(Place::WEST)].scriptFilePath    = System::ScriptPath();

    opt.bots[Place(Place::NORTH)].identity.nickname = "Bender";
    opt.bots[Place(Place::NORTH)].identity.avatar  = ":/avatars/N03.png";
    opt.bots[Place(Place::NORTH)].identity.gender  = Identity::cGenderMale;
    opt.bots[Place(Place::NORTH)].scriptFilePath   = System::ScriptPath();

    opt.bots[Place(Place::EAST)].identity.nickname = "Amy";
    opt.bots[Place(Place::EAST)].identity.avatar   = ":/avatars/FE02.png";
    opt.bots[Place(Place::EAST)].identity.gender   = Identity::cGenderFemale;
    opt.bots[Place(Place::EAST)].scriptFilePath    = System::ScriptPath();

    return opt;
}

//=============================================================================
// End of file ClientConfig.cpp
//=============================================================================
