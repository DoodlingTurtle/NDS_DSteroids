# DSteroids
A Nintendo DS homebrew game based on "Asteroids".

This project makes use of [libNDS](https://github.com/devkitPro/libnds), as well as a modified version of [GL2D](https://github.com/DeathCamel57/libgl2d_ds).

It is also the base for a framework/engine I am currently developing.
[RGNDS::Engine](https://github.com/DoodlingTurtle/NDS_RGNDS_Engine)

Which is free to use for everyone. Simply add the repository as a Submodule to your GIT project.

## The Plan
The idea is, to make a Asteroids game, that will allow the player to purchase "Upgrades" for there ship in between levels.
The upgrades are partially randomized, allowing for each run to turn out slightly different.

## Current features
- Remapable controls
- Endless gamemode

# TODO
- Add a pause menu
  (to allow for quiting the game without having to force a gameover)

- Implement NDS sleep mode
  (probably a feature, that will be added to RGNDS::Engine)

- Implement saves
  - For the highscore 
  - For the controller config
  - (Maybe a qucksave mode, to pause the current run.)
  
- Add music and sound
- Implement the ship upgrades
- Add more game mechanics to balance the game
  - Timer to count down the score
    (the longer the player takes to complete the level, the less points they will get)
    
  - Cooldown for shots 
    (the cooldown can be reduced via upgrade purchases)