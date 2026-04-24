#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>

using namespace sf;
using namespace std;


// Checking if two rectangles (x1, y1, w1, h1) and (x2, y2, w2, h2) are colliding
bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
        return true;
    }
    return false;
}

int main() {
    float pipeSpeed = 3.0f;

    // Creating Window
    RenderWindow window(VideoMode(1000, 600), "Flappy Bird");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    srand(time(0));

    // Logo
    Image icon;
    if (!icon.loadFromFile("resources/Logo.png")) {
        return -1;
    }
    // Setting Logo
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    // Sounds & Buffers
    struct Sounds {
        SoundBuffer scoreBuffer;
        SoundBuffer flapBuffer;
        SoundBuffer hitBuffer;
        Sound score;
        Sound flap;
        Sound hit;
    } sounds;

    // Loading Sounds From Resources
    sounds.scoreBuffer.loadFromFile("resources/Score.wav");
    sounds.flapBuffer.loadFromFile("resources/Flap.wav");
    sounds.hitBuffer.loadFromFile("resources/Crash.wav");
    sounds.score.setBuffer(sounds.scoreBuffer);
    sounds.flap.setBuffer(sounds.flapBuffer);
    sounds.hit.setBuffer(sounds.hitBuffer);

    // Textures (Bird has 3)
    struct Textures {
        Texture flappy[3];
        Texture pipe;
        Texture background;
        Texture gameover;
    } textures;



    // loading Textures from Resources
    textures.background.loadFromFile("resources/Background.png");
    textures.pipe.loadFromFile("resources/Pipe.png");
    textures.gameover.loadFromFile("resources/GameOver.png");
    textures.flappy[0].loadFromFile("resources/Flap-1.png");
    textures.flappy[1].loadFromFile("resources/Flap-2.png");
    textures.flappy[2].loadFromFile("resources/Flap-3.png");

    // Struct Flappy
    struct Flappy {
        double v = 0;          // v = Vertical Speed
        int frame = 0;         // frame = Texture to be Printed
        Sprite sprite;         // Object
    } flappy;                  // Instance of Flappy Struct

    // Pipes Struct
    struct Pipe {
        Sprite sprite;
        bool scored = false;
        bool isUpper = false;
        float prevX = 0;
    };
    
    // Initial Position & Scale
    flappy.sprite.setPosition(250, 300);
    flappy.sprite.setScale(2, 2);

    // Dynamic Vector Array to Represent Pipes
    vector<Pipe> pipes;

    //ENUMs for Gamestate & Menustate
    enum GameState { waiting,            // = Game not Started Yet
                     started,            // = Playing 
                     gameover            // = Game OVer
                    };

    enum MenuState { MainMenu,           // = Menu State
                     Instructions,       // = Instructions Page
                     ExitConfirmation,   // = Yes/No Dialogue Box
                     None                // = None
                    };


    // Game Settings Struct
    struct Game {
        int score = 0;                  // Current Score    
        int highscore = 0;              // Highest Score
        int frames = 0;                 // Total Frames Passed
        GameState gameState = waiting;  // In the 'Waiting' State
        MenuState menuState = MainMenu; // Start at MainMenu
        Sprite background[4];           // Array of 4 Bgs
        Sprite gameover;                // Sprite for GameOver
        Font font;                      // Font for the Game
        Text scoreText;                 // Text Displaying Current Score
        Text highscoreText;             // Text Displaying Highest Score 
    } game;

    // Loading Font
    game.font.loadFromFile("resources/Font.ttf");

    // Multiple Functionalities

    // Setup backgrounds
    const int backgroundNum = 4;
    for (int i = 0; i < backgroundNum; i++) {
        game.background[i].setTexture(textures.background);
        game.background[i].setScale(1.6, 1.6);
        game.background[i].setPosition(i * 280, 0);
    }

    // Gameover Settings
    game.gameover.setTexture(textures.gameover);
    game.gameover.setOrigin(192 / 2, 42 / 2);
    game.gameover.setPosition(500, 250);
    game.gameover.setScale(2, 2);

    // Score Text Settings
    game.scoreText.setFont(game.font);
    game.scoreText.setFillColor(Color::White);
    game.scoreText.setCharacterSize(75);
    game.scoreText.setPosition(500, 50);
    game.scoreText.setOrigin(0, 0);


    // HighScore Text Settings
    game.highscoreText.setFont(game.font);
    game.highscoreText.setFillColor(Color::Black);
    game.highscoreText.setCharacterSize(30);
    game.highscoreText.setPosition(30, 80);

    // Menu Buttons Settings
    Text title("FLAPPY BIRD", game.font, 80);
    title.setFillColor(Color::Yellow);
    title.setPosition(280, 100);

    Text startText("Start", game.font, 50);
    Text instructionsText("Instructions", game.font, 50);
    Text exitText("Exit", game.font, 50);

    startText.setFillColor(Color::White);
    instructionsText.setFillColor(Color::White);
    exitText.setFillColor(Color::White);

    startText.setPosition(450, 250);
    instructionsText.setPosition(390, 320);
    exitText.setPosition(470, 390);

    // Instructions String Array (5 instructions)
    vector<string> instructionsLines = {
        "1. Press SPACE to flap your wings.",
        "2. Avoid hitting the pipes.",
        "3. Pass through pipes to score points.",
        "4. Press R to restart after game over.",
        "5. Have fun and try to beat high score!"
    };

    // Instructions Text Array
    vector<Text> instructionTexts;
    for (int i = 0; i < (int)instructionsLines.size(); i++) {
        Text line(instructionsLines[i], game.font, 30);
        line.setFillColor(Color::White);
        line.setPosition(100, 150 + i * 50);
        instructionTexts.push_back(line);
    }

    // Back Button Inside Instructions Box
    Text backText("Back", game.font, 40);
    backText.setFillColor(Color::White);
    backText.setPosition(450, 500);

    // Gameover Settings
    Text restartText("Restart", game.font, 50);
    Text menuText("Menu", game.font, 50);

    restartText.setFillColor(Color::White);
    menuText.setFillColor(Color::White);

    restartText.setPosition(400, 320);
    menuText.setPosition(440, 400);

    // Helper Function To Check If Mouse Is Over Text
    auto isMouseOverText = [&](Text &text) {
        FloatRect bounds = text.getGlobalBounds();
        Vector2i mousePos = Mouse::getPosition(window);
        return bounds.contains((float)mousePos.x, (float)mousePos.y);
    };

    // Bird Position For Scoring
    const float birdX = 250.0f;

    // Main Game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {

            // Closing Window
            if (event.type == Event::Closed) {
                window.close();
            }

            // Mouse Clicks For Menus
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (game.menuState == MainMenu) {
                    if (isMouseOverText(startText)) {
                        // Starting The Game
                        game.menuState = None;
                        game.gameState = waiting;
                        flappy.sprite.setPosition(250, 300);
                        flappy.v = 0;
                        game.score = 0;
                        pipes.clear();
                    }
                    else if (isMouseOverText(instructionsText)) {
                        game.menuState = Instructions;
                    }
                    else if (isMouseOverText(exitText)) {
                        game.menuState = ExitConfirmation;
                    }
                }
                else if (game.menuState == Instructions) {
                    if (isMouseOverText(backText)) {
                        game.menuState = MainMenu;
                    }
                }
                if (game.gameState == gameover && game.menuState == None) {
                    if (isMouseOverText(restartText)) {
                        game.gameState = waiting;
                        flappy.sprite.setPosition(250, 300);
                        flappy.v = 0;
                        game.score = 0;
                        pipes.clear();
                    } else if (isMouseOverText(menuText)) {
                        game.gameState = waiting;
                        game.menuState = MainMenu;
                        flappy.sprite.setPosition(250, 300);
                        flappy.v = 0;
                        game.score = 0;
                        pipes.clear();
                    }
                }
            }

            // Keyboard Input For Game Control
            if (event.type == Event::KeyPressed) {
                if (game.menuState == None) {
                    if (event.key.code == Keyboard::Space) {
                        if (game.gameState == waiting) {
                            game.gameState = started;
                        }
                        if (game.gameState == started) {
                            flappy.v = -8;
                            sounds.flap.play();
                        }
                    }
                    // Restart With "R" Button
                    else if (event.key.code == Keyboard::R && game.gameState == gameover) {
                        game.gameState = waiting;
                        flappy.sprite.setPosition(250, 300);
                        flappy.v = 0;
                        game.score = 0;
                        pipes.clear();
                        game.menuState = None; // Going Back To Menu After Game Over
                    }
                    // Menu With "ESC" Button
                    else if (event.key.code == Keyboard::Escape) {
                        if (game.menuState == None) {
                            game.menuState = MainMenu;
                        }
                    }
                }
                else {
                    // Menu With "ESC" Button
                    if (event.key.code == Keyboard::Escape) {
                        game.menuState = MainMenu;
                    }
                }
            }
        }

        // Updating Score
        game.scoreText.setString(to_string(game.score));
        FloatRect scoreBounds = game.scoreText.getLocalBounds();
        game.scoreText.setOrigin(scoreBounds.width / 2, 0);
        game.scoreText.setPosition(500, 50);
        game.highscoreText.setString("HIGH SCORE " + to_string(game.highscore));

        // Pipe Speed Control
        pipeSpeed = 3.0f + (game.score * 0.3f);
        if (pipeSpeed > 10.0f) pipeSpeed = 10.0f;

        // Flapping Wings
        if (game.gameState == waiting || game.gameState == started) {
            
            // Changing Texture Every 6 Frames
            if (game.frames % 6 == 0) {
                flappy.frame = (flappy.frame + 1) % 3;
            }
        }
        // Setiing Bird's Texture Based On Current Frame
        flappy.sprite.setTexture(textures.flappy[flappy.frame]);


        // Getting Flappy's Current Position (Top-Left Corner Of The Sprite)
        float fx = flappy.sprite.getPosition().x;
        float fy = flappy.sprite.getPosition().y;

        // Computing Flappy's Width & Height After Applying Sprite Scaling
        // 34 & 24 Are The Original Pixel Dimensions Of The Flappy Sprite
        float fw = 34 * flappy.sprite.getScale().x;
        float fh = 24 * flappy.sprite.getScale().y;


        // Moving Flappy
        if (game.gameState == started) {
            flappy.sprite.move(0, flappy.v);
            flappy.v += 0.5;
        }

        // Ceiling & Floor Constraints
        if (game.gameState == started) {
            if (fy < 0) {
                flappy.sprite.setPosition(250, 0);
                flappy.v = 0;
            }
            else if (fy > 600) {
                flappy.v = 0;
                game.gameState = gameover;
                sounds.hit.play();
            }
        }

        // Counting Score
        if (game.gameState == started) {
            for (auto& pipe : pipes) {
                if (!pipe.scored && !pipe.isUpper) {
                    float pipeLeftPrev = pipe.prevX;
                    float pipeLeftNow = pipe.sprite.getPosition().x;
                    if (pipeLeftPrev >= birdX && pipeLeftNow < birdX) {
                        pipe.scored = true;
                        game.score++;
                        sounds.score.play();
                        if (game.score > game.highscore) {
                            game.highscore = game.score;
                        }
                    }
                }
                pipe.prevX = pipe.sprite.getPosition().x;
            }
        }

        // Generating Pipe
        if (game.gameState == started && game.frames % 150 == 0) {
            int r = rand() % 275 + 75;
            int gap = 150;

            // Lower Pipes
            Pipe pipeL;
            pipeL.sprite.setTexture(textures.pipe);
            pipeL.sprite.setPosition(1000, r + gap);
            pipeL.sprite.setScale(2, 2);
            pipeL.isUpper = false;
            pipeL.scored = false;
            pipeL.prevX = 1000;

            // Upper Pipes
            Pipe pipeU;
            pipeU.sprite.setTexture(textures.pipe);
            pipeU.sprite.setPosition(1000, r);
            pipeU.sprite.setScale(2, -2);
            pipeU.isUpper = true;
            pipeU.scored = false;
            pipeU.prevX = 1000;

            // Adding Lower & Upper Sprites to Pipes Array
            pipes.push_back(pipeL);
            pipes.push_back(pipeU);
        }

        // Moving Pipes
        if (game.gameState == started) {
            for (auto& pipe : pipes) {
                pipe.sprite.move(-pipeSpeed, 0); // Dynamic Speed
            }
        }

        // Remove Offscreen Pipes
        if (game.frames % 100 == 0) {
            pipes.erase(
                remove_if(pipes.begin(), pipes.end(), [](const Pipe& pipe) {
                    return pipe.sprite.getPosition().x < -184;
                }),
                pipes.end()
            );
        }

        // Collision Detection
        if (game.gameState == started) {
            for (auto& pipe : pipes) {
                float px, py, pw, ph;
                if (pipe.sprite.getScale().y > 0) {
                    px = pipe.sprite.getPosition().x;
                    py = pipe.sprite.getPosition().y;
                    pw = 52 * pipe.sprite.getScale().x;
                    ph = 320 * pipe.sprite.getScale().y;
                }
                else {
                    pw = 52 * pipe.sprite.getScale().x;
                    ph = -320 * pipe.sprite.getScale().y;
                    px = pipe.sprite.getPosition().x;
                    py = pipe.sprite.getPosition().y - ph;
                }
                if (collides(fx, fy, fw, fh, px, py, pw, ph)) {
                    game.gameState = gameover;
                    sounds.hit.play();
                }
            }
        }

        // Clearing Window
        window.clear();

        // Drawing Background
        for (int i = 0; i < backgroundNum; i++) {
            window.draw(game.background[i]);
        }

        // Drawing Game Flappy & Pipes
        window.draw(flappy.sprite);
        for (auto& pipe : pipes) {
            window.draw(pipe.sprite);
        }

        // Drawing Score (Current & High)
        if (game.menuState == None) {
            window.draw(game.scoreText);
            window.draw(game.highscoreText);
        }

        // Drawing Gameover Options
        if (game.gameState == gameover && game.menuState == None) {
            // Semi-Transparent Overlay
            RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            window.draw(game.gameover);

            restartText.setFillColor(isMouseOverText(restartText) ? Color::Green : Color::White);
            menuText.setFillColor(isMouseOverText(menuText) ? Color::Yellow : Color::White);

            window.draw(restartText);
            window.draw(menuText);

        }

        // Drawing MainMenu
        if (game.menuState == MainMenu) {
            // Semi-Transparent Overlay
            RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            // Drawing Menu Texts
            window.draw(title);

            // Highlighting On Hover
            startText.setFillColor(isMouseOverText(startText) ? Color::Yellow : Color::White);
            instructionsText.setFillColor(isMouseOverText(instructionsText) ? Color::Black : Color::White);
            exitText.setFillColor(isMouseOverText(exitText) ? Color::Red : Color::White);

            window.draw(startText);
            window.draw(instructionsText);
            window.draw(exitText);

        }
        else if (game.menuState == Instructions) {
            window.draw(flappy.sprite);
            for (auto& pipe : pipes) {
                window.draw(pipe.sprite);
            }

            // Semi-Transparent Overlay
            RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(Color(0, 0, 0, 180));
            window.draw(overlay);

            // Drawing Instructions Text
            for (auto& t : instructionTexts) {
                window.draw(t);
            }

            // Back Button Highlighting
            backText.setFillColor(isMouseOverText(backText) ? Color::Yellow : Color::White);
            window.draw(backText);
        }

        // Creating Exit Confirmation Dialogue Box
        else if (game.menuState == ExitConfirmation) {
            RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(Color(0, 0, 0, 200));
            window.draw(overlay);

            Text confirmText("Do You Really Want to Exit?", game.font, 50);
            confirmText.setFillColor(Color::White);
            confirmText.setPosition(250, 200);
            window.draw(confirmText);

            Text yesText("Yes", game.font, 50);
            Text noText("No", game.font, 50);

            yesText.setPosition(400, 350);
            noText.setPosition(600, 350);

            yesText.setFillColor(isMouseOverText(yesText) ? Color::Red : Color::White);
            noText.setFillColor(isMouseOverText(noText) ? Color::Yellow : Color::White);

            window.draw(yesText);
            window.draw(noText);

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (isMouseOverText(yesText)) {
                    window.close();
                } else if (isMouseOverText(noText)) {
                    game.menuState = MainMenu;
                }
            }
            
        }

        // Displaying Updated Window
        window.display();

        // Incrementing Frame Count to Track Elapsed Game Time.
        // It is Used to keep Track of Time, ExecuteTimed Events like Pipe
        // Generation, and Maintain Smooth Animations throughout the Game.
        game.frames++;
    }

    return 0;
}
