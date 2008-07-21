//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2008 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be ruseful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "translation.hpp"
#include "challenges/tollway_time.hpp"
#include "world.hpp"
#include "race_manager.hpp"

TollwayTime::TollwayTime() : Challenge("tollwaytime", _("Finish Tux Tollway track in 3:00"))
{
    setChallengeDescription(_("Finish 3 laps on the Tux Tollway\ntrack with 3 AI karts\nin under 3:00 minutes."));
    addUnlockTrackReward("canyon");
}   // TollwayTime

//-----------------------------------------------------------------------------
void TollwayTime::setRace() const {
    race_manager->setMajorMode(RaceManager::RM_SINGLE);
    race_manager->setMinorMode(RaceManager::RM_QUICK_RACE);
    race_manager->setTrack("tuxtrack");
    race_manager->setDifficulty(RaceManager::RD_EASY);
    race_manager->setNumLaps(3);
    race_manager->setNumKarts(4);
    race_manager->setNumPlayers(1);
}   // setRace

//-----------------------------------------------------------------------------
bool TollwayTime::raceFinished()
{
    std::string track_name = world->getTrack()->getIdent();
    if(track_name!="tuxtrack"      ) return false;    // wrong track
    Kart* kart=world->getPlayerKart(0);
    if(kart->getFinishTime()>180) return false;    // too slow
    if(kart->getLap()!=3       ) return false;    // wrong number of laps
    if(race_manager->getNumKarts()<4) return false; //not enough AI karts
    return true;
}   // raceFinished
//-----------------------------------------------------------------------------
