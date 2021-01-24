# DSteroids
A Nintendo DS Homebrew Game based on "Asteroids".

This Project makes use of [libNDS](https://github.com/devkitPro/libnds), as well as a modified Version of [GL2D](https://github.com/DeathCamel57/libgl2d_ds).
It is also the Base/Test for a Framework/Engine that I am currently developing.
[RGNDS::Engine](https://github.com/DoodlingTurtle/NDS_RGNDS_Engine)

The Engine is free to use for everyone. Simply add the repository as a Submodule to your GIT Project.

## The Plan
The Idea is, to make a Asteroids Game, that will allow the Player to purchase "Upgrades" for there Ship in between Levels.
The Upgrades are partially randomized, allowing for each Run to turn out slightly different.

## Current features
- Remapable Controls
- Endless Gamemode

## TODO
- Implement the Ship Upgrades

- Add more Game Mechanics to balance the game
  - ~~Cooldown for shots~~
    (the cooldown can be reduced via upgrade purchases)

  - Timer to count down the Score
    (the longer the Player takes to complete the Level, the less Points they will get)

- Add Music ~~and sound~~

- ~~Add a pause Menu
  (to allow for quiting the Game without having to force a Gameover)~~

- ~~Implement NDS sleep Mode~~

- ~~Implement saves~~
  (Scrapped, because there is no consistend save Method accross different Emulators for Homebrew)
  - ~~For the Highscore~~
  - ~~For the Controller Config~~
  - ~~(Maybe a Qucksave Mode, to Pause the current Run.)~~
  

## Changes
**alpha 0.0.3:**
+ [+] Added Sound Effects
+ [+] Added a "Credits" Screen (since the Sound Effects aren't by me)
+ [\*] Added a Cooldown to the Shots fired in the Maingame

**alpha 0.0.2:**
+ [+] Added a "Pause" gamestate
  * Pressing start during gameplay will pause the game
  * Closing the DS during gameplay will also open the pause menu
  * A running game can now be exited from the pause menu
- [*] Selected menu entrys are now marked with a blue background 

**alpha 0.0.1:**
- Initial version
