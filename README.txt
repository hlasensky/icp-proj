/*! \mainpage ICP Project Index Page
 *
 * \author Tomáš Hlásenský (xhlase01)
 * \author Michael Babušík (xbabus01)
 *
 * We had lot of meetings were we discussed our project plans and goals. Functionality was done mainly together, when we used one week sprints.  
 * \section intro_sec Introduction
 *
 *  This project creates a game environment in Qt Creator, where simulations with RC rumbas, autonomous rumbas, and obstacles take place.
 *
 *	Types of rumbas and their behavior
 *
 *  RC rumbas: These are remote-controlled rumbas that the player can actively control. The game allows switching between different RC rumbas, with the active rumba controllable while other RC rumbas remain inactive and stationary.
 *  Autonomous rumbas: These rumbas move independently without player intervention. The game allows changing various attributes of autonomous rumbas, such as obstacle or other rumba detection distance, direction of rotation after obstacle or other rumba detection, and the angle by which the rumba rotates.
 *
 *	Obstacles and manipulation with them
 *
 *  Obstacles: Obstacles are static objects on the map that impede the movement of robots. The game allows placing obstacles anywhere on the map and changing their width and height. Obstacles can also be placed on top of each other. Once an obstacle is placed on a rumba, it immobilizes it and cannot move.
 *
 * \section implement_sec Implementation
 * 
 * Implementation was done with the Factory Method design pattern which creates infrastructure for making objects from the superclass MainWindow.
 * \cite guru2014

 * \subsection structure 1: Main structure
 *	- The application begins by creating an instance of the QApplication class, followed by creating the main MainWindow, which is displayed using the show() method, and finally running the application loop using a.exec(), which keeps the application active and responsive to events.
 *	- The constructor of the MainWindow class initializes the UI, sets the view as the viewport for graphicsView, prepares game states, and a timer for periodic scene updates.
 *	- Users can add rumbas and obstacles to the scene using buttons in the user interface. Objects are added using the on_numOfRumbas_valueChanged(), on_numOfObstacles_valueChanged(), and on_numOfRumbasRC_valueChanged() methods. After adding objects, they can be modified by clicking with the left mouse button.
 *	- RC rumbas and autonomous rumbas are created using the constructors of the RumbaRC and Rumba classes with attribute settings. A directional indicator is also created for RC rumbas for better orientation.
 *	- Main program loop: The updateScene() method traverses all objects in the scene and continuously checks for collisions. If a collision occurs, the step is reverted. Furthermore, the active element is set based on mouse clicks, and objects are color-coded red.
 *
 * \section usage_sec Usage
 *  To launch the application, use the command make run. After the window is displayed, you will have access to the control menu on the left side to add objects to the scene. Objects are added using the button with an plus next to the "Add/Remove" field, and the minus removes the last added object of the respective category. When adding an autonomous rumba, you can set the detection distance using a slider, rotation angle using a spinner, and rotation direction using a circular button.
 *  Controlling the RC rumba is possible using the keys W, A, S, D or the buttons displayed on the left side, which can be clicked with the left mouse button. Obstacles can also be adjusted using a slider and if its active it can be draged to the different coordinates using mouse.
 *   On the right side of the screen, there are buttons for the following actions:
 *	- Quit: Exits the game.
 *	- Pause: Pauses the game.
 *	- Save: Saves the current game state.
 *	- Load: Loads the last saved game state.
 *	- New Game: Clears the current game and displays an empty playing field.
 *
 *  In the top bar, there are two menus:
 *	- Game: Contains buttons for game actions (similar to those on the right side).
 *	- Examples: Contains buttons for displaying example games.
 * 
 * \section design_sec Design
 *
 * \subsection concept_sec Conceptual Design
 * \image html firstLook.png The initial layout design. width=600px
 *
 * \subsection final_sec Final Design
 * \image html finalLook.png The final layout design. width=600px
 *
 * \section concept_diagram_sec Conceptual diagram
 * \image html Conceptual_diagram.png width=800px
 *
 * \section test_sec Testing
 * \image html stressTest.png Stress testing with more than 500 Rumbas. width=800px
 *
 * \section dream_sec What could have been done
 * \todo Deleting active Rumbas, RumbasRC and Obstacles. 
 *
 * \todo Saving more games and could rewrite it or load specific game. Now is possible to save only one game and load it.
 *
 * \todo Drag and drop Rumbas and RumbasRC.
 *
 * \todo Also we thought about some interactive element like shooting others Rumbas and count their score. 
 * 
 * \section flaw_sec Known flaws
 *  \bug When dragging an Obstacle, you can place it on Rumba and RumbaRC objects, and they remain stacked under the Object.
 * 
 *  \bug When specifically pressing menu buttons, it is possible that the game will not show until the Resume button is pressed. 
 *  Specifically when Pause -> New Game -> Load Game or Examples are pressed in this order. After clicking the Resume button, the game shows and plays normally.
 * 
 *  \bug When adding objects to the scene, it is possible that the object would not find a place without collisions, so it is not going to be added, but the spinBox is going to show a higher number.
 *
 *  \bug When adding object to the scene, it will automaticly start game time. So it is not possible to presets the game scenario. 
 *  
 */

