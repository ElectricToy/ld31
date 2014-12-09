Protect Your Family
===================

# Creature Variants

Fast but uses poor pathfinding
Slower but uses perfect pathfinding
"Carve through" power
Lift speed multiplier (when carrying person)
Toughness (amount of damage required to kill)

# Tasks

BUG: Repeatedly mashing the fire key while pointing any direction but right with a turret makes it jump around silly.


Post Mortem
    Xcode/clang has gotten far more unstable
    Restart of idea
    Statistics: changes over the weekend.


Add embedded version. This almost works, but keyboard doesn't. DEBUG preventDefault() in an iFrame.

Port to iOS
    Controls
    Remove key hints

Maybe Android version
Play LD games

Distribute X number of items randomly in suitable tiles, rather than asking each tile, to keep things more consistent.

BUG: Turrets should stop working after game over. (Else they kill the last human's attacker and you have a live human after the end.)

Make "raked" map area more useful -- good for treasure, e.g.

More monster types -- at least fast+weak/slow+strong

Show no-go and no-place areas

Art
    Animations    
    Logo

HUD
    Electric toy logo (UL corner)
    Game logo (UL corner)

SFX
    Torch fizzle
    A monster appears
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
    New item provided

Music
    Clock or drum like
    Faded enough to not grate
    Ominous, I think. But with folksy, plaintiff element, like uilleann pipes or other Irish/medieval instrument.
    Pretty long: at least 2–3 minutes ideally. But watch out for megabytage.
    provide mute through pause

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

Fix scaling problem

    Desired behavior:
        When stage aspect is < desired, we scale up stage height while keeping width constant. (the current behavior)
        When it is > desired (window too wide) we scale down stage width while keeping height constant.
        If stage aspect is 0, we do the current behavior, for legacy.

XXX Mines no longer hurt carried humans.
XXX Mines go green when about to explode. Oops.
XXX Make flash of some kind when a new object is "provided."
XXX Humans shouldn't wander over other humans, or items if they can help it.
XXX Fork "original" web version.
XXX Fix scaling problem
XXX Reduce the initial number of at-home items to make a more adaptive situation.

XXX Fix computeStageDimensions. Will need fairly drastic fix, I think.

