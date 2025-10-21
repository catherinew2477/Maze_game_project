//
//  Weapon.cpp
//  project3
//
//  Created by Catherine Wang on 5/14/24.
//

#include "Map.h"
#include "GameObject.h"
#include "Game.h"
#include "utilities.h"
using namespace std;


int Object::posRow() const {
    return m_posRow;
}
int Object::posCol() const {
    return m_posCol;
}



string Weapon::getWeaponName(Weapon* w) {
    if (ShortSword* ssPtr = dynamic_cast<ShortSword*>(w)) {
        return "short sword";
    }
    if (LongSword* lsPtr = dynamic_cast<LongSword*>(w)) {
        return "long sword";
    }
    if (MagicAx* lsPtr = dynamic_cast<MagicAx*>(w)) {
        return "magic ax";
    }
    if (MFSleep* lsPtr = dynamic_cast<MFSleep*>(w)) {
        return "magic fangs of sleep";
    }
    return "mace";
}
