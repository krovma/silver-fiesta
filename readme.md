
# Epics
[Trello](https://trello.com/b/d367iSaP/untitled-and-will-be-titled-shooter-game)
# Time schedule

## Wk5 Gameplay lvl.1
**Entites**
* (x) Player can move and swap to slow mode (2h)
* (x) Player can shoot (3h)
* () Player can die (0.5h)
* () Player can respawn (1h)
* (x) Enemies can spawn (1h)
* (x) Enemies can shoot (2h)
* (x) Enemies can die   (0.5h)
* (x) Enemies can leave after a certain time (1h)

**Game states**
* (x) Switching in Main menu, game and editor (1h)
   ** game editor done
* () Game stops if the marked last enemy is killed (1h)
* () Game stops if player has no life (0.5h)
* (x) Game stops if player request to leave (0.1h)

**Tech**
* (x) Draw an static image as the background (0.2h)
* () Basic sfx  (3h)

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
