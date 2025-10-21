
//  project3
//
//  Created by Catherine Wang on 5/10/24.
//

// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <list>
class Map;
#include "Actor.h"

// You may add data members and other member functions to this class.


class Game
{
public:
  
    
    Game(int goblinSmellDistance);
    bool newPosition(int& r, int& c, char dir);
    ~Game() {
        delete goldenIdol;
        delete p;
    }
    void play();
    
    Object* getGoldenIdol() {return goldenIdol;}
    
    void displayStats();
    
    void monstersMayMove(Player* p, Map* m);
    
private:
    Player* p;
    Map* m;
    Object* goldenIdol;
    int m_gsd;
};

#endif // GAME_INCLUDED
