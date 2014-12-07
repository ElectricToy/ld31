Protect Your Family
===================

# Creature Variants

Fast but uses poor pathfinding
Slower but uses perfect pathfinding
"Carve through" power
Lift speed multiplier (when carrying person)
Toughness (amount of damage required to kill)

# Tasks

XXX Gridwise movement

XXX Inventory/Object pickup/drop/usage

XXX Initial setup
XXX Initial tools
XXX Family

Monsters
XXX Spawning
XXX Pathfinding
XXX AI
XXX     Move toward center area
XXX     If within X units of a live human, pursue, capture or kill that human
XXX     If pursuing, but > X + histeresis that human, stop pursuing.
        If within Y units of a turret, destroy the turret.
XXX Breaking
XXX Stealing people
XXX Item drop on death
XXX Should kill active player.

Humans
XXX Should return home if released
XXX If player dies, possess next available human

Art
    GO FOR HEAVIER OUTLINES
    Player
    Family
    Monsters
    Tools
    Logo

Tools
XXX Doors
XXX     Wood
XXX     Iron
XXX Walls
XXX     Gravel
XXX     Brick
XXX     Stone
XXX Torch
XXX Mine
XXX     Explosion emitter
    Turret

XXX Player can grind

HUD
    Inventory Display
    Action buttons with enablement and hotkey hints
        Take
        Drop
        Place ("Use?")
    Messages
    Electric toy logo
    Game logo (UL corner)

SFX
    Monster steps, grunts
    Grind sounds
    Human screams when picked up
    Explosions
    Door open/close
    Place things
    Thing destroyed by grinding

Waves

CHOOSE GAME NAME

Game Start/Over

Emscripten bugfixing (lighting), deployment

BUG
    Human lights flicker when they're being moved.
    Human lights still glow in exit after departure
XXX Monster will dither when holding a human, unsure of which exit to take.
XXX Monsters take humans to far portal.

Bonus
    Flash human when it becomes player
    Flash anything that receives damage
    Turrets
    Family Wander AI
    Pause
    Explosive Barrel
    Mine waits before exploding
