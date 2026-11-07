// SimAntics primitives the narrator listens to (the game's behavior bytecode).
#pragma once

#include <cstdint>

#include "memory.h"

namespace game {

// A balloon the game showed over an object (opcode 0x70), as its raw operands and the object id
// they name. Filled by the hook; read and cleared by the narrator.
struct Balloon {
    int objectId = 0;           // the object the effect shows over
    int mode = 0;               // operand +4: 0 start, 2/3/4 stop, ...
    int effect = -1;            // index in the global effect names (STR# 0x8f), -1 when the name is the object's own
    uint16_t flags = 0;         // operand +0xe
    int sheet = -1;             // icon texture sheet (STR# 0x95 index, 0-based) when the icon is a sheet cell
    int iconValue = 0;          // the icon variable's value: a sheet cell or an object id
    bool iconObject = false;    // the icon is a thumbnail of the Sim with neighbor id iconValue
    int iconThing = 0;          // the icon is a thumbnail of this lot object (object id)...
    uint32_t iconGuid = 0;      // ...or of this object definition
    uint32_t group = 0;         // the running tree's resource group (its own string tables)
    int runner = 0;             // the object running the tree (its id: My data 11)
    int stackObject = 0;        // the tree's stack object (the other Sim of a social), 0 none
    uint8_t operands[24] = {};  // the instruction object: a vtable, then the operands
};

void InitSimAnticsHooks();
// Takes the balloons recorded since the last call (at most 32 are kept).
int TakeBalloons(Balloon* out, int max);

}  // namespace game
