# DSteroids
A Nintendo DS Homebrew Game based on "Asteroids".

This Project makes use of [libNDS](https://github.com/devkitPro/libnds), as well as a modified Version of [GL2D](https://github.com/DeathCamel57/libgl2d_ds).
It is also the Base/Test for a Framework/Engine that I am currently developing.
[RGNDS::Engine](https://github.com/DoodlingTurtle/NDS_RGNDS_Engine)

The Engine is free to use for everyone. Simply add the repository as a Submodule to your GIT Project.

## The Plan
The idea is, a Asteroids game, that will allow the player to purchase upgrades for there ship in between Levels.
These upgrades are payed for with the point the player earns during the main game.
The Upgrades are partially randomized, allowing for each run to turn out slightly different.

## Current features
- Remapable controls
- Endless gamemode
- Ship upgrades (changes how the ship controls in between levels)

## Changes
**beta 0.1.0**
- [\*] cost of upgrades is now dependent on the current game level
- [\*] ship upgrades are now partially randomized
- [\*] countdown for the score in maingame
- [\*] tweaks to the prices of upgrades
- [\*] BUGFIX: char limit for cost display in upgrade screen was to low

**alpha 0.0.4:**
* [+]Added ship upgrades (purchaseable in between levels)
* [\*]Improved the display of the shield (now a sprite, instead of a circle made of polygons)

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

## TODO

- find better sound effects for the shots
- improve performance on higher levels. (The screen starts to "Glitch out" if to many drawing calls are made in one frame)
- replace some polygon graphics with sprites to reduce polygon count
  - alternatively reduce the number of segments on smaler asteroids

- ~~Add more Game Mechanics to balance the game~~
  - ~~Timer to count down the Score~~
    ~~(the longer the Player takes to complete the Level, the less Points they will get)~~

- ~~Cooldown for shots~~
    ~~(the cooldown can be reduced via upgrade purchases)~~

- Add Music ~~and sound~~

- ~~Implement the Ship Upgrades~~

- ~~Add a pause Menu
  (to allow for quiting the Game without having to force a Gameover)~~

- ~~Implement NDS sleep Mode~~

- ~~Implement saves~~
  (Scrapped, because there is no consistend save Method accross different Emulators for Homebrew)
  - ~~For the Highscore~~
  - ~~For the Controller Config~~
  - ~~(Maybe a Qucksave Mode, to Pause the current Run.)~~
  
