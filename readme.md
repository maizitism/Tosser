# Tosser (SFML 3) - Paper Ball Arcade Game

A small arcade game built in **C++** with **SFML 3** where you throw a paper ball into a trash can under changing wind conditions.  
Designed as a compact, polished physics/aiming project with a menu + tweakable options.

---

## Features

- **Arcade throw system**: fixed number of throws per round
- **Wind**: affects the ball mid-flight and changes every **3 throws**
- **Dynamic trash can**:
  - moves farther away as you score
  - scales down with depth
  - subtle **wobble** to increase difficulty
- **Trajectory preview** while aiming
- **Menu + Options** UI (separate UI module)
  - change physics constants at runtime via sliders / text input
- **Audio**
  - looping background music
  - SFX for **score** and **miss**
- **Game Over** screen shows **final score**

---

## How to Play

### Controls
- **Hold `Space`**: charge throw power (trajectory is shown while charging)
- **Release `Space`**: throw the ball
- **Esc** (during gameplay): return to menu
- **Enter / Space** (on game over): return to menu
- **Mouse**: navigate Menu / Options

### Objective
Score as many baskets as possible within the available throws.

### Scoring
- A score is awarded when the ball intersects the trash can opening.
- The ball “sinks” slightly into the trash can on a successful hit before resetting.

### Miss
- If the ball resets without scoring, it counts as a miss (miss SFX plays).
- The round ends when **throws run out**.

### Wind
- Wind is shown on-screen as **Wind: <- / -> magnitude**
- Wind applies as a horizontal acceleration during the ball’s flight.
- Wind changes automatically every **3 throws**.

---

## Options

The Options menu allows you to tweak gameplay feel live, including:

- `Gravity` - vertical acceleration
- `DepthFactor` - perspective compression strength
- `t_clamp` - minimum perspective scale clamp
- `vp_x`, `vp_y` - vanishing point offsets

Use sliders or click the value field to type a number and press **Enter**.

---

## Project Structure (High Level)

- `Game.*` - main loop, gameplay rules, rendering order, scoring, round state
- `Ball.*` - flight simulation, reset timing, sink animation on scoring
- `Trashcan.*` - positioning, scaling, wobble, opening rectangle hit zone
- `Trajectory.*` - trajectory preview point rendering (aim assistance)
- `UI.*` - menu + options UI, sliders, text input, hover/click behavior
- `Audio.*` - music + SFX loading/playing
- `PowerMeter.*` — power/charge UI shown while aiming
- `GradientBar.*` — drawable used by the power meter to render a vertical color gradient, which acts as the “meter body” that the marker slides over
- `Math.hpp` — small inline math helpers shared across the project
- `Constants.hpp` - tweakable runtime constants used across modules

---

## Assets / Resources Used

This project uses external assets for visuals, audio, and UI fonts.  
All asset paths are relative to the executable working directory.
### Fonts
- `ASSETS/ui.ttf`  
  Used for menu text, HUD text, and option labels/values.

### Images
- `ASSETS/blurred_office_background.jpg`  
  Background image.
- `ASSETS/paper_ball.png`  
  Ball sprite.
- `ASSETS/trashcan.png`  
  Trash can sprite.

### Audio
- `ASSETS/audio/menu.mp3`  
  Looping background music.
- `ASSETS/audio/score.wav`  
  Played on a successful score.
- `ASSETS/audio/miss.wav`  
  Played when a throw ends without scoring.

---

## Build / Run Notes

### Dependencies
- **SFML 3** (Graphics + Window + System + Audio)
    - Shipped with the project in the `LIB\SFML3.0.2` folder (no installation or solution modification required)

---

## Gameplay Notes / Implementation Details

- **Perspective compression (“depth”)** uses a k-law scaling:
  - points farther along the flight path scale down and move toward a vanishing point
- The trash can uses a similar depth scaling logic plus wobble
- The opening hit zone is based on a percentage-based rectangle inside the trash can sprite bounds

---

## Credits / Attribution

- Background image: [Source](https://storage.googleapis.com/dwwgjgxuiowsae/free-blurred-home-office-background.html), Thomas Lintz
- Paper ball sprite: [Source](https://www.vecteezy.com/png/48712076-crumpled-paper-ball-transparent-background), QasimVisuals
- Trash can sprite:  [Source](https://www.pngall.com/trash-can-png/download/10696/), PNG All 
- UI font: [Download](https://www.freefonts.io/consolas-font/), Luc(as) de Groot, © 2018 Microsoft Corporation
- Music:
    - Menu music: [Source](https://www.youtube.com/watch?v=xy_NKN75Jhw), Kevin MacLeod - Elevator Music
- SFX:
    - Scoring sound: [Source](https://www.youtube.com/watch?v=FZfPRsz0zyU), The Sound Pack Tree - Sound Effect: Drop Piece Of Paper
    - Miss sound: [Source](https://www.youtube.com/watch?v=slZTkRNVgZc), Skype - OLD_HANGUP

---

## Screenshots / Demo (Optional)

Add screenshots or a short GIF/video here to show:
- Menu
- Options sliders
- Gameplay throw + wind HUD
- Game over final score