//  $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_RACEMANAGER_H
#define HEADER_RACEMANAGER_H

#include <vector>
#include <algorithm>

#include <string>
#include "cup_data.hpp"

/** The race manager has two functions:
    1) it stores information about the race the user selected (e.g. number
       of karts, track, race mode etc.)
    2) when a race is started, it creates the world, and keeps track of
       score during the race. When a race is finished, it deletes the world,
       and (depending on race mode) starts the next race by creating a new
       world).
    Information in the race manager is considered to be more static, sometimes
    the world has similar functions showing the current state. E.g.: the
    race manager keeps track of the number of karts with which the race was
    started, while world keeps track of the number of karts currently in the
    race (consider a race mode like follow the leader where karts can get
    eliminated).
    The race manager handles all race types as a kind of grand prix. E.g.:
    a quick race is basically a GP with only one track (so the race manager
    keeps track of scores even though no scores are used in a quick race)
    **/
class RaceManager
{
public:
    enum RaceModeType   { RM_GRAND_PRIX, RM_SINGLE, // The two current major modes
                          RM_QUICK_RACE, RM_TIME_TRIAL, RM_FOLLOW_LEADER };
    enum Difficulty     { RD_EASY, RD_MEDIUM, RD_HARD, RD_SKIDDING };

private:
    struct KartStatus
    {
        std::string m_ident;            // The .tkkf filename without the .tkkf
        int         m_score;            // score for this kart
        int         m_last_score;       // needed for restart race
        double      m_overall_time;     // sum of times of all races
        double      m_last_time;        // needed for restart
        int         m_prev_finish_pos;  // previous finished position
        int         m_player_id;        // player controling the kart, for AI: -1

        KartStatus(const std::string& ident, const int& prev_finish_pos, 
                   const int& player_id) :
                   m_ident(ident), m_score(0), m_last_score(0), 
                   m_overall_time(0.0f), m_last_time(0.0f),
                   m_prev_finish_pos(prev_finish_pos), m_player_id(player_id)
                {}
        
    };   // KartStatus

    std::vector<KartStatus>          m_kart_status;
    Difficulty                       m_difficulty;
    RaceModeType                     m_major_mode, m_minor_mode;
    typedef std::vector<std::string> PlayerKarts;
    PlayerKarts                      m_player_karts;
    std::vector<std::string>         m_tracks;
    std::vector<int>                 m_num_laps;
    std::vector<int>                 m_score_for_position;
    int                              m_track_number;
    CupData                          m_cup;
    int                              m_num_karts;
    unsigned int                     m_num_finished_karts;
    unsigned int                     m_num_finished_players;
    int                              m_coin_target;

    void startNextRace();    // start a next race

    friend bool operator< (const KartStatus& left, const KartStatus& right)
    {
        return (left.m_score < right.m_score);
    }

public:
    bool                             m_active_race; //True if there is a race

public:
    
    RaceManager();
    ~RaceManager();

    void         setPlayerKart(unsigned int player, const std::string& kart);
    void         setNumPlayers(int num);
    void         reset();
    void         RaceFinished(const Kart* kart, float time);
    void         setTrack(const std::string& track);
    void         setGrandPrix(const CupData &cup){ m_cup = cup;                     }
    void         setDifficulty(Difficulty diff);
    void         setNumLaps(int num)            { m_num_laps.clear();
                                                  m_num_laps.push_back(num);        }
    void         setMajorMode(RaceModeType mode){ m_major_mode = mode;              }
    void         setMinorMode(RaceModeType mode){ m_minor_mode = mode;              }
    void         setNumKarts(int num)           { m_num_karts = num;                }
    void         setCoinTarget(int num)         { m_coin_target = num;              }
    RaceModeType getMajorMode()           const { return m_major_mode;              }
    RaceModeType getMinorMode()           const { return m_minor_mode;              }
    unsigned int getNumKarts()            const { return m_num_karts;               }
    unsigned int getNumPlayers()          const { return (int)m_player_karts.size();}
    int          getNumLaps()             const { return m_num_laps[m_track_number];}
    Difficulty   getDifficulty()          const { return m_difficulty;              }
    const std::string& 
                 getTrackName()           const { return m_tracks[m_track_number];  }
    const CupData 
                *getGrandPrix()           const { return &m_cup;                    }
    unsigned int getFinishedKarts()       const { return m_num_finished_karts;      }
    unsigned int getFinishedPlayers()     const { return m_num_finished_players;    }
    const std::string&  
                 getKartName(int kart)    const { return m_kart_status[kart].m_ident;}
    const std::string& 
                 getHerringStyle()        const { return m_cup.getHerringStyle();   }
    int          getKartScore(int krt)    const { return m_kart_status[krt].m_score;}
    int          getKartPrevScore(int krt)const { return m_kart_status[krt].m_last_score;}
    int          getkartPlayerId(int krt) const { return m_kart_status[krt].m_player_id;}
    int          getPositionScore(int p)  const { return m_score_for_position[p-1]; }
    double       getOverallTime(int kart) const { return m_kart_status[kart].m_overall_time;}
    int          getCoinTarget()          const { return m_coin_target;             }
    bool         raceHasLaps()            const { return m_minor_mode!=RM_FOLLOW_LEADER;}
    int          allPlayerFinished()      const {return 
                                          m_num_finished_players==m_player_karts.size();    }
    int          raceIsActive()           const { return m_active_race;             }
    bool         isPlayer(int kart)       const {return m_kart_status[kart].m_player_id>-1; }

    void         setMirror() {/*FIXME*/}
    void         setReverse(){/*FIXME*/}

    void startNew();         // start new race/GP/...
    void next();             // start the next race or go back to the start screen
    void rerunRace();        // Rerun the same race again
    void exit_race();        // exit a race (and don't start the next one)
};

extern RaceManager *race_manager;
#endif

/* EOF */
