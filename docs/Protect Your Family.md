Protect Your Family
===================

# Creature Variants

Fast but uses poor pathfinding
Slower but uses perfect pathfinding
"Carve through" power
Lift speed multiplier (when carrying person)
Toughness (amount of damage required to kill)

# Tasks

Make nice map area more useful -- good for treasure, e.g.

Fix computeStageDimensions. Will need fairly drastic fix, I think.

More monster types

Art
    Monsters
    Variety -- at least fast+weak/slow+strong
    Logo

HUD
    Electric toy logo (UL corner)
    Game logo (UL corner)

SFX
    Monster steps, grunts
    Grind sounds
    Human screams when picked up
    Explosions
    Door open/close
    Place things
    Thing destroyed by grinding
    Monster grunts when changing destination significantly
    New wave
    Countdown

Music

Bonus
    Pause button/options/restart
    Difficulty selection
    Explosive Barrel
    Clock font monospace to avoid shifting
    Flash human when it becomes player
    Newly placed items should block light (update static blockers in TileGrid in that area).
    Inventory Display


DONE

XXX Countdown to first wave.
XXX    Mine waits before exploding
XXX Emscripten bugfixing (lighting), deployment
XXX    Auto-firing turret
XXX Human lights flicker when they're being moved.
XXX Human lights still glow in exit after departure
XXX Remove or reduce human non-player lights.
XXX Monster will dither when holding a human, unsure of which exit to take.
XXX Monsters take humans to far portal.
XXX Family Wander AI
XXX Game Start

XXX Prevent initial spawn for 2-3 minutes
XXX Waves
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


XXX Gridwise movement

XXX Inventory/Object pickup/drop/usage
XXX Initial setup
XXX Initial tools
XXX Family
XXX Humans
XXX Should return home if released
XXX If player dies, possess next available human
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
XXX Action buttons with enablement and hotkey hints
XXX        Take
XXX        Drop
XXX       Place ("Use?")
XXX Messages
XXX Startup
XXX Game over/Restart
XXX Player can grind
XXX    Player
XXX    Family
XXX    Tools
XXX Placement (blocks esp)
XXX Tools
XXX    Turret


Object ordering

Lighting

Turret Arrow         1000

HeldHuman
Monster             100

HeldItem

Human               0
Turret
Torch
Mine
Door
Block
TileGrid

XXX TEST Monsters should disappear after a while.

XXX Actor sort
XXX Announcement for family member taken, rescued.

XXX Turrets shouldn't fire until placed at least once. Their weird initial angle communicates it pretty well.

XXX Get rid of bodies (darken them)
XXX Too easy - reduce missile/weapon count - tighter waves
XXX Bug: turrets bug when spent
XXX Turret text
XXX CHOOSE GAME NAME

