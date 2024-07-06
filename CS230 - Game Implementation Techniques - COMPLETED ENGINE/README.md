# Welcome!

## Course Details
**Name:** CS230: Game Implementation Techniques
**Instructor:** Doug Schilling
**Semester:** Spring 2021
**Description:** This presents game implementation techniques and engine architecture. Students investigate foundational concepts of game architecture, such as game-system component separation and game flow, while learning about essential elements such
as the game state manager, input/output handler, and frame rate controller. This course introduces Windows programming, state machines, and collision detection algorithms, which
students will integrate into their own remakes of classic games. As part of their implementation, students create and expand their own collision, vector, and matrix libraries, enabling them to incorporate basic physics engines. Students survey concepts in space partitioning, particle systems, map editors, and other elements as a bridge to more advanced concepts in implementation techniques and engine architecture.
*Description copied from the 2020-2021 DigiPen Course Catalog*

# Looking Into the Project
Over the course of the semester, under the guidance of Doug Schilling, each programmer created an engine. The project covered basic engine archetecture, creation frameworks, data management, physics, player input, and behaviors. The project contains code from Doug Schilling (and other DigiPen faculty staff), but the comment blocks at the top of each file give credit throughout.

# Downloading the Release Build
To experience the prototype first hand, you will have to download the entire folder and have a copy of Visual Studio 2021 (or later) downloaded. Unzip the folder wherever, double-click the .sln file to open the solution, and click the green button in the top banner to run the program (or press F5).

# Different Gamestates
**Omega:** The program launches in this state (can also be accessed by pressing 4). You can move your ship (red/orange triangle mesh) around with the ARROW KEYS (movement is impacted by physics). Asteriods (grey deformed meshes) will be coming from the corners of the screen. If an asteroid touches your ship, you lose a life and the gamestate restarts. You can destroy asteroids with bullets by pressing SPACE. All objects around confined to the 'space' (black territory) and will be reflected when run into (using vector math). The center of the screen holds some UI that will dynamically update to represent relevant information within the game.

**Asteroids:** A simplified verison of Omega (reached byu pressing 3). There is no center island and instead of be confined, all objects will 'wrap' around to the opposing edge when it goes off screen.

**Sandbox:** The proof of concept for the ship (reached by pressing 2) - the triangle mesh will move to your mouse position.

**Demo:** A simple demonstration of user input and collision deteciton/resolution. Move the monkey with the arrow keys and avoid the jumping Earth. If the Earth hits the monkey, you lose a life.

*You can exit the application at any time with the ESC key*