![Example room](images/first.png) ![The menu](images/map.png)

This is an unfinished platformer video game inspired by such works as Seiklus, An Untitled Story, and Mole's Quest.

The game is Arity.exe

# Controls

Press the left and right cursor keys to move
Press the spacebar to jump
Press the down key in front of a save point to save
Press the up key in front of a save point to teleport between saves
Press escape to open the menu

In the teleport map, the red rooms contain save points. The arrow keys move the yellow cursor. When the cursor has an X through it, press space to teleport to the coresponding save point. In order to teleport to a save point, you just first have used it to save.

The player has a double jump, and can triple jump by pressing space again at the very top of the second jump's arc (within a frame or two).

I guess when I started this I wasn't aware of the conflicts between the spacebar and cursor keys.

# Features

## Falling Platforms

![Falling platforms](images/first.png)
![Falling platforms](images/plat1.png)
![Falling platforms](images/plat2.png)

In some rooms, platforms descend from the ceiling, and the player is required to use them to reach the top of the room. I struggled to make this fun. Originally, the platforms started at random locations, but even with no other pressures reaching a specific location at the top of a room could be tedious due to bad luck. I attempted to fix the issue by instead having the platform spawn location move back and forth across the width of the room with some random variation, which makes them a little more consistent, but can still be a problem depending on the size of the target and the amount of random variation. 

It never occurred to me that they might work better with designed patterns, but I think that might still present issues. A sufficiently complex pattern will take some time to reset if the player misses, which adds boring dead time to the game. The pattern could be repeated with some phase shift to reduce the effective reset time, but as the instances get closer together they would begin to resemble a simple grid. The actual pattern would cease to matter as every case would just be a matter of climbing a simple ladder. The motion of the platforms would add nothing to the game except some delays.

This mechanic was inspired by a game called Mole's Quest my friend had on his parents' pentium II computer. The player had to traverse a sequence of alternative rising and falling platforms, some of which contained obstacles and some of which contained collectible objects. If you hit the bottom of the top of the screen, you die and have to start over. Mole's Quest used a simple grid of moving platforms on a relatively small screen and didn't really have any geniune platforming elements. You just jump left or right, and depending on the timing you either land on the next platform or hit the edge and die. The concept doesn't seem to translate that well to this kind of platformer.

## Power Ups

![A jump upgrade](images/powerup.png)
![A health upgrade](images/powerup2.png)

There are collectible power ups that permanently increase the player's health, first jump height, or double jump height. At some point I thought that there might be a bit of a metroidvania aspect with some locations requiring a minimum jump height to access. At the same time, the triple jump mechanic provides a (finnicky) means of bypassing jump height requirements. The jump height upgrades ultimately don't add much. Each upgrade is pretty small, so you need a lot to see any significant change in capability. At the same time, the variations in jump height make it difficult to get the hang of the triple jump timing, which is constantly changing as you collect upgrades.

## Turrets

![Some guns](images/guns1.png)
![Some guns](images/guns2.png)
![Some guns](images/guns3.png)

These basically exist to make health matter and to create a failure condition. I struggled to make this fun. They shoot bullets directly at the player with different speeds and sizes. Unfortunately when the player is a pixel and the bullets are fast pixels, its' hard to make any predictions about collision. In most cases you only get hit if you stop moving or get unlucky. I played around with different configurations some, but never found anything that was fun.

Believe it or not this mechanic was inspired by An Untitled Story. One of the bosses can fire a lot of projectiles at the player. When I first played AUS I was still coming to terms with difficulty in games, and the gravity hell bits frustrated me enough to stick with me.

## Bosses

![The blue boss](images/boss1.png)
![The blue boss](images/boss2.png)

I think the bosses are the closest I got to making the falling platforms relevant. You have to touch a point on the ground, then within a limited time touch a weak spot on the boss in order to damage. Destroy all the weak points to defeat the boss.

## Devmode

With the menu open, enter 

up up down down left right left right z x

When entered correctly, a sound will play and the text "KONAMI CODE IS ACTIVATE" will appear at the top of the screen. This gives unlimited jumps and exposes the entire map. You'll also be able to press up to teleport from anywhere and to any save point.

## Level Editor

![The level editor](images/puredit.png)

There is one (puredit.exe). I remember it being kind of unstable and don't really remember anything about it.

