
# Epics
[Trello](https://trello.com/b/d367iSaP/untitled-and-will-be-titled-shooter-game)
# Time schedule

## Wk5 Gameplay lvl.1
**Entites**
* () Player can move and swap to slow mode
* () Player can shoot and use bomb
* () Player can die
* () Player can respawn
* () Enemies can spawn
* () Enemies can shoot
* () Enemies can die
* () Enemies can leave after a certain time

**Game states**
* () Switching in Main menu, game and editor
* () Game stops if the marked last enemy is killed
* () Game stops if player has no life
* () Game stops if player request to leave

**Tech**
* () Draw an static image as the background
* () Basic sfx

# Rendering Order

## Layers
```
1. +-------- UI ---------+
2. +- Fullscreen effects-+
   +----- Particles -----+
   +------- Items -------+
   +------ Bullets ------+
   +----- Character -----+

3. +--Background effects-+
   +------Background-----+
```

## Order
```
   Layer          Texture
1. Background   -> 1 (Background)
2. Character    -> 2 (Dynamic)
3. Bullets      -> 2 (Dynamic)
4. Items        -> 2 (Dynamic)
5. Particles    -> 2 (Dynamic)
6. BG Effect    -> 1 (Background)
7. Merge        -> 0 (Output)
8. FS Effect    -> 0 (Output)
9. UI           -> 0 (Output)
```
