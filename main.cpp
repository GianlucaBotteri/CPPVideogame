#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TileMap.h"
#include "Hero.h"
#include "Mob.h"
#include "NPC.h"
#include "Projectile.h"
#include "Settings.h"
#include "StatusBar.h"


int main() {

    enum Gamestate {
        Initialize, Pause, Playing
    };

    Gamestate gameState = Initialize;

    Settings settings;

    const int lakeSX = 3;
    const int lake = 4;
    const int lakeDX = 5;
    const int transp = 8;
    bool isAnimating = false;
    bool startAnimation = false;
    bool halfAnimation = false;

    sf::Vector2u screenDimension(800, 600);

    // create the window
    sf::RenderWindow window(sf::VideoMode(screenDimension.x, screenDimension.y), "Videogame", sf::Style::None);

    sf::Vector2i positionWindow((sf::VideoMode::getDesktopMode().width / 2) - screenDimension.x / 2,
                                (sf::VideoMode::getDesktopMode().height / 2) - screenDimension.y / 2);
    window.setPosition(positionWindow);

    Hero hero(8, 100, 80, "Jon Snow", 200, 1000, 0);

    if (gameState == Initialize) {
        settings.showSplash(window);
        window.clear();
        settings.showOptions(window);
        gameState = Playing;
    }


    // center camera
    sf::View view;
    view.reset(sf::FloatRect(0, 0, screenDimension.x, screenDimension.y));
    view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
    sf::Vector2f position(screenDimension.x / 2, screenDimension.y / 2);
    sf::Vector2f viewSize = view.getSize();

    // Clock for projectiles
    sf::Clock clockProjectile;

    // Clock for angry enemies
    sf::Clock clockAngry;

    // load player texture
    sf::Texture texturePlayer;
    if (!texturePlayer.loadFromFile("TileP.png")) {
        return EXIT_FAILURE;
    }

    // load NPC texture
    sf::Texture textureSheep;
    if (!textureSheep.loadFromFile("Sheep.png")) {
        return EXIT_FAILURE;
    }

    // load enemy texture
    sf::Texture textureAssassin;
    if (!textureAssassin.loadFromFile("Assassin.png")) {
        return EXIT_FAILURE;
    }
    sf::Texture textureDarkLord;
    if (!textureDarkLord.loadFromFile("DarkLord.png")) {
        return EXIT_FAILURE;
    }

    // load fireball texture
    sf::Texture textureFireBall;
    if (!textureFireBall.loadFromFile("fireball.png")) {
        return EXIT_FAILURE;
    }

    // load profile texture
    sf::Texture textureProfile;
    if (!textureProfile.loadFromFile("Head.png")) {
        return EXIT_FAILURE;
    }

    // load name texture
    sf::Texture textureName;
    if (!textureName.loadFromFile("Name.png")) {
        return EXIT_FAILURE;
    }

    // load heart texture
    sf::Texture textureHeart;
    if (!textureHeart.loadFromFile("Hearts.png")) {
        return EXIT_FAILURE;
    }

    // load experience texture
    sf::Texture textureExp;
    if (!textureExp.loadFromFile("Experience.png")) {
        return EXIT_FAILURE;
    }

    // load money texture
    sf::Texture textureMoney;
    if (!textureMoney.loadFromFile("Money.png")) {
        return EXIT_FAILURE;
    }

    // load weapon texture
    sf::Texture textureWeapon;
    if (!textureWeapon.loadFromFile("Weapons.png")) {
        return EXIT_FAILURE;
    }

    // load potion texture
    sf::Texture texturePotion;
    if (!texturePotion.loadFromFile("Potions.png")) {
        return EXIT_FAILURE;
    }

    // load scroll texture
    sf::Texture textureScroll;
    if (!textureScroll.loadFromFile("Scrolls.png")) {
        return EXIT_FAILURE;
    }

    // Load music
    sf::Music music;
    if (!music.openFromFile("music.ogg")) {
        return EXIT_FAILURE;
    }

    // Play music
    music.play();
    music.setLoop(true);

    // Sound effects
    sf::SoundBuffer bufferShot;
    if (!bufferShot.loadFromFile("shot.ogg"))
        return -1;
    sf::Sound soundShot;
    soundShot.setBuffer(bufferShot);

    hero.sprite.setTexture(texturePlayer);

    NPC sheep(100, 5, 8, NPC::Animal, "Spettro", 0, false);
    sheep.rect.setPosition(200, 400);
    sheep.sprite.setTexture(textureSheep);

    // Vector of enemies
    std::vector<Mob> enemies;

    //First enemy
    Mob darkLord(60, 8, 10, Mob::Undead, 0.5, 2, false);
    darkLord.rect.setPosition(200, 400);
    darkLord.sprite.setTexture(textureDarkLord);
    enemies.push_back(darkLord);

    //Second enemy
    Mob assassin(40, 2, 9, Mob::Samurai, 0.5, 1, false);
    assassin.rect.setPosition(300, 500);
    assassin.sprite.setTexture(textureAssassin);
    enemies.push_back(assassin);

    // Vector of projectiles
    std::vector<Projectile> projectileArray;
    Projectile projectile;
    projectile.sprite.setTexture(textureFireBall);

    // Head sprite
    sf::Sprite headSprite;
    headSprite.setTexture(textureProfile);
    headSprite.setTextureRect(sf::IntRect(0, 0, 150, 164));
    headSprite.setPosition(0, 0);

    // Name sprite
    sf::Sprite nameSprite;
    nameSprite.setTexture(textureName);
    nameSprite.setTextureRect(sf::IntRect(0, 0, 150, 54));
    nameSprite.setPosition(0, 164);

    // Hearts sprite
    sf::Sprite heartsSprite;
    heartsSprite.setTexture(textureHeart);
    heartsSprite.setTextureRect(sf::IntRect(0, 0, 150, 36));
    heartsSprite.setPosition(0, 218);

    // Experience sprite
    sf::Sprite expSprite;
    expSprite.setTexture(textureExp);
    expSprite.setTextureRect(sf::IntRect(0, 0, 150, 59));
    expSprite.setPosition(0, 254);

    // Money sprite
    sf::Sprite moneySprite;
    moneySprite.setTexture(textureMoney);
    moneySprite.setTextureRect(sf::IntRect(0, 0, 150, 61));
    moneySprite.setPosition(0, 313);

    // Weapons sprite
    sf::Sprite weaponsSprite;
    weaponsSprite.setTexture(textureWeapon);
    weaponsSprite.setTextureRect(sf::IntRect(0, 0, 150, 76));
    weaponsSprite.setPosition(0, 374);

    // Potions sprite
    sf::Sprite potionsSprite;
    potionsSprite.setTexture(texturePotion);
    potionsSprite.setTextureRect(sf::IntRect(0, 0, 150, 70));
    potionsSprite.setPosition(0, 450);

    // Scrolls sprite
    sf::Sprite scrollsSprite;
    scrollsSprite.setTexture(textureScroll);
    scrollsSprite.setTextureRect(sf::IntRect(0, 0, 150, 80));
    scrollsSprite.setPosition(0, 520);


    // Define the level with an array of tile indices
    const int levelBackground[] =
            {
                    1340, 1340, lake, lake, lake, lake, lake, lake, lake, lake, lake, lake, lake, lake, lake, lake,
                    lake, lake, lakeDX,
                    8, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                    8, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                    8, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 0, 0, 0, 0, 0, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 0, 0, 0, 0, 0, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
            };

    // create the tilemap from the level definition
    TileMap mapBackground;
    if (!mapBackground.load("TileMap.png", sf::Vector2u(48, 48), levelBackground, 19, 13))
        return -1;

    const int levelVisible[] =
            {
                    1340, 1340, transp, transp, transp, transp, transp, transp, transp, transp, transp, transp,
                    transp, transp, transp, transp, transp, transp, transp,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 11, 12, 23, 23, 23, 23, 6, 23, 23, 23,
                    23, 23, 6, 23, 23, 10, 23, 23, 23, 13, 14, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 9, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 10, 23, 23, 6, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 9, 23, 23, 23, 23,
                    23, 23, 11, 12, 0, 0, 0, 0, 0, 0, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 13, 14, 0, 0, 0, 0, 0, 0, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 10, 10, 10, 23, 23,
                    23, 23, 23, 6, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 9, 9, 9, 23, 23,
                    23, 23, 23, 23, 23, 23, 23, 23, 23, 1, 2, 23, 23, 23, 23, 23, 23, 23, 23,
            };

    // create the tilemap from the level definition
    TileMap mapVisible;
    if (!mapVisible.load("TileMap.png", sf::Vector2u(48, 48), levelVisible, 19, 13))
        return -1;


    ExperienceBar experienceBar(&hero);
    HealthBar healthBar(&hero);

    // run the main loop
    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }


        // Should create a function to handle that?
        if (hero.rect.getPosition().x + 24 > screenDimension.x / 2)
            position.x = hero.rect.getPosition().x + 24;
        if (hero.rect.getPosition().y + 24 > screenDimension.y / 2)
            position.y = hero.rect.getPosition().y + 24;
        if (hero.rect.getPosition().x + viewSize.x / 2 > 888)
            position.x = 912 - (viewSize.x / 2);
        if (hero.rect.getPosition().y + viewSize.y / 2 > 600)
            position.y = 624 - (viewSize.y / 2);


        view.setCenter(position);

        window.setView(view);

        window.clear();

        // draw the map
        window.draw(mapBackground);
        window.draw(mapVisible);

        window.setView(window.getDefaultView());

        window.draw(headSprite);
        window.draw(nameSprite);
        window.draw(heartsSprite);
        window.draw(expSprite);
        window.draw(moneySprite);
        window.draw(weaponsSprite);
        window.draw(potionsSprite);
        window.draw(scrollsSprite);

        sf::Time elapsedProjectile = clockProjectile.getElapsedTime();

        sf::Time elapsedAngry = clockAngry.getElapsedTime();

        int enemyCounter = 0;


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            hero.setHasSword(true);
            startAnimation = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            gameState = Pause;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            hero.setHealth(hero.getHealth()-1);
            hero.notify(heartsSprite);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            hero.setExp(hero.getExp()+1);
            if(hero.getExp() > 19) {
                hero.setExp(0);
            }
            hero.notify(expSprite);
        }

        if (gameState == Playing) {
            if (startAnimation) {
                if (!isAnimating) {
                    hero.useWeapon(window);
                    isAnimating = true;
                    halfAnimation = true;
                } else {
                    if (halfAnimation) {
                        hero.sprite.setTextureRect(sf::IntRect(192, 0, 64, 64));
                        halfAnimation = false;
                        isAnimating = false;
                        startAnimation = false;
                    } /*else {
                    hero.sprite.setTextureRect(sf::IntRect(192, 128, 64, 64));
                    isAnimating = false;
                    startAnimation = false;
                }*/

                }
            }

            // Angry enemies
            int angryCounter = 0;
            for (auto itr = enemies.begin(); itr != enemies.end(); itr++) {
                if (enemies[angryCounter].isAngry() == true) {
                    if (elapsedAngry.asSeconds() >= 1) {
                        clockAngry.restart();
                        int randomNumber = rand();
                        int tempRand = (randomNumber % 2) + 1;
                        if (tempRand == 1) { // Fires and chases
                            projectile.setDamage(enemies[angryCounter].getStrength());
                            // Player to Right
                            if ((hero.rect.getPosition().x < enemies[angryCounter].rect.getPosition().x) &&
                                (abs(hero.rect.getPosition().y - enemies[angryCounter].rect.getPosition().y) <= 30)) {
                                soundShot.play();
                                projectile.setHostile(true);
                                projectile.setDirection(Character::Direction::Left);
                                projectile.rect.setPosition(enemies[angryCounter].rect.getPosition().x +
                                                            enemies[angryCounter].rect.getSize().x / 2 -
                                                            projectile.rect.getSize().x / 2,
                                                            enemies[angryCounter].rect.getPosition().y +
                                                            enemies[angryCounter].rect.getSize().y / 2 -
                                                            projectile.rect.getSize().y / 2);
                                projectileArray.push_back(projectile);
                                projectile.setHostile(false);

                                enemies[angryCounter].setDirection(Character::Direction::Left);
                            }

                            // Player to Left
                            if ((hero.rect.getPosition().x > enemies[angryCounter].rect.getPosition().x) &&
                                (abs(hero.rect.getPosition().y - enemies[angryCounter].rect.getPosition().y) <= 30)) {
                                soundShot.play();
                                projectile.setHostile(true);
                                projectile.setDirection(Character::Direction::Right);
                                projectile.rect.setPosition(enemies[angryCounter].rect.getPosition().x +
                                                            enemies[angryCounter].rect.getSize().x / 2 -
                                                            projectile.rect.getSize().x / 2,
                                                            enemies[angryCounter].rect.getPosition().y +
                                                            enemies[angryCounter].rect.getSize().y / 2 -
                                                            projectile.rect.getSize().y / 2);
                                projectileArray.push_back(projectile);
                                projectile.setHostile(false);

                                enemies[angryCounter].setDirection(Character::Direction::Right);
                            }

                            // Player to Top
                            if ((hero.rect.getPosition().y < enemies[angryCounter].rect.getPosition().y) &&
                                (abs(hero.rect.getPosition().x - enemies[angryCounter].rect.getPosition().x) <= 30)) {
                                soundShot.play();
                                projectile.setHostile(true);
                                projectile.setDirection(Character::Direction::Up);
                                projectile.rect.setPosition(enemies[angryCounter].rect.getPosition().x +
                                                            enemies[angryCounter].rect.getSize().x / 2 -
                                                            projectile.rect.getSize().x / 2,
                                                            enemies[angryCounter].rect.getPosition().y +
                                                            enemies[angryCounter].rect.getSize().y / 2 -
                                                            projectile.rect.getSize().y / 2);
                                projectileArray.push_back(projectile);
                                projectile.setHostile(false);

                                enemies[angryCounter].setDirection(Character::Direction::Up);
                            }

                            // Player to Bottom
                            if ((hero.rect.getPosition().y > enemies[angryCounter].rect.getPosition().y) &&
                                (abs(hero.rect.getPosition().x - enemies[angryCounter].rect.getPosition().x) <= 30)) {
                                soundShot.play();
                                projectile.setHostile(true);
                                projectile.setDirection(Character::Direction::Down);
                                projectile.rect.setPosition(enemies[angryCounter].rect.getPosition().x +
                                                            enemies[angryCounter].rect.getSize().x / 2 -
                                                            projectile.rect.getSize().x / 2,
                                                            enemies[angryCounter].rect.getPosition().y +
                                                            enemies[angryCounter].rect.getSize().y / 2 -
                                                            projectile.rect.getSize().y / 2);
                                projectileArray.push_back(projectile);
                                projectile.setHostile(false);

                                enemies[angryCounter].setDirection(Character::Direction::Down);
                            }
                        }
                        else if (tempRand == 2) { // Enemy Chases Player
                            if (hero.rect.getPosition().y < enemies[angryCounter].rect.getPosition().y) {
                                enemies[angryCounter].setDirection(Character::Direction::Up);
                            }
                            else if (hero.rect.getPosition().x > enemies[angryCounter].rect.getPosition().x) {
                                enemies[angryCounter].setDirection(Character::Direction::Right);
                            }
                            else if (hero.rect.getPosition().x < enemies[angryCounter].rect.getPosition().x) {
                                enemies[angryCounter].setDirection(Character::Direction::Left);
                            }
                            else if (hero.rect.getPosition().y > enemies[angryCounter].rect.getPosition().y) {
                                enemies[angryCounter].setDirection(Character::Direction::Down);
                            }
                        }
                        else { // Enemy Chases Player
                            if (hero.rect.getPosition().x < enemies[angryCounter].rect.getPosition().x) {
                                enemies[angryCounter].setDirection(Character::Direction::Left);
                            }
                            else if (hero.rect.getPosition().x > enemies[angryCounter].rect.getPosition().x) {
                                enemies[angryCounter].setDirection(Character::Direction::Right);
                            }
                            else if (hero.rect.getPosition().y < enemies[angryCounter].rect.getPosition().y) {
                                enemies[angryCounter].setDirection(Character::Direction::Up);
                            }
                            else if (hero.rect.getPosition().y > enemies[angryCounter].rect.getPosition().y) {
                                enemies[angryCounter].setDirection(Character::Direction::Down);
                            }
                        }

                    }
                }

                angryCounter++;
            }




            hero.update();
            hero.moveSprite(levelVisible);

            if (elapsedProjectile.asSeconds() >= 0.25) {
                clockProjectile.restart();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    projectile.rect.setPosition(
                            hero.rect.getPosition().x + hero.rect.getSize().x / 2 - projectile.rect.getSize().x / 2,
                            hero.rect.getPosition().y + hero.rect.getSize().y / 2 - projectile.rect.getSize().y / 2);
                    projectile.setDirection(hero.getDirection());
                    projectileArray.push_back(projectile);
                    soundShot.play();
                }
            }

            // Check projectile-enemy collision
            int pcounter = 0;
            for(auto itr = projectileArray.begin(); itr != projectileArray.end(); itr++){
                int ecounter = 0;
                for(auto itr2 = enemies.begin(); itr2 != enemies.end(); itr2++) {
                    if (projectileArray[pcounter].rect.getGlobalBounds().intersects(enemies[ecounter].rect.getGlobalBounds()) &&
                        projectileArray[pcounter].isHostile() == false) {
                        enemies[ecounter].setAlive(false);
                        projectileArray[pcounter].setDestroy(true);
                    }

                }
                pcounter++;
            }

            // Update enemies
            for (auto itr = enemies.begin(); itr != enemies.end(); itr++) {
                enemies[enemyCounter].update();
                enemies[enemyCounter].moveSprite(levelVisible);
                enemyCounter++;
            }

            // Update NPC
            sheep.update();
            sheep.moveSprite(levelVisible);
        }
        // Draw Player
        window.draw(hero.sprite);


        // Draw FireBalls
        int counter = 0;
        for (auto itr = projectileArray.begin(); itr != projectileArray.end(); itr++) {
            projectileArray[counter].update();
            window.draw(projectileArray[counter].sprite);
            counter++;
        }

        // Draw Enemies
        enemyCounter = 0;
        for (auto itr = enemies.begin(); itr != enemies.end(); itr++) {
            window.draw(enemies[enemyCounter].sprite);
            enemyCounter++;
        }



        // Draw NPC
        window.draw(sheep.sprite);

        // Delete dead enemies
        counter = 0;
        for (auto itr = enemies.begin(); itr != enemies.end(); itr++) {
            if (enemies[counter].isAlive() == false) {
                enemies.erase(itr);
                break;
            }
            counter++;
        }

        // Delete projectiles
        counter = 0;
        for (auto itr = projectileArray.begin(); itr != projectileArray.end(); itr++) {
            if (projectileArray[counter].isDestroy() == true) {
                projectileArray.erase(itr);
                break;
            }
            counter++;
        }

        window.display();
    }

    return 0;
}
