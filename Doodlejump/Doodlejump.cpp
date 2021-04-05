// Programmer: Zachary Leali
// Date: 11/1/2019
// Name: DoodleJump
// Description:  This program creates a window that displays the game DoodleJump and has been reformatted and changed slightly.

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>
using namespace sf;
using namespace std;

class point
{
private:
    int m_x;
    int m_y;
public:
    void setx(int a)
    {
        m_x = a;
    }
    void sety(int b)
    {
        m_y = b;
    }
    int getx() const
    {
        return m_x;
    }
    int gety() const
    {
        return m_y;
    }
};

int main() {
    // generates random like numbers
    srand(time(0));

    bool active = true;
    bool spawn = true;
    bool invincible = false;

    // creates a window with the size 400x533 and the window has the title "Doodle Game!"
    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    // this sets the frame rate limit of the window to 60 frames per second.
    app.setFramerateLimit(60);

    Clock clock2;
    Time elapsed;


    // creates three texture objects
    Music game_play_song;

    SoundBuffer game_over_sound;
    SoundBuffer enemy_hit;

    Texture t1, t2, t3;
    Texture ogre, wolf, fireball, level2;
    Texture power;

    Font font1;


    if(!game_over_sound.loadFromFile("sounds/Scream.ogg")) // https://www.freesoundeffects.com/free-sounds/human-sound-effects-10037/  ----  Free for personal use non-commercial use  --- creator not specified
    {
        return EXIT_FAILURE;
    }
    if (!game_play_song.openFromFile("sounds/song1.ogg")) // https://soundcloud.com/phloethik303/mad-notes-mixtape-7back-to-the-funk  ----- free to download and use for non-commercial use as long as I cite it and provide who created the song ---- Artist: PHIoEthik(Mad Notes MixTapes)
    {
        return EXIT_FAILURE;
    }
    game_play_song.setVolume(50.0);

    if(!enemy_hit.loadFromFile("sounds/explosion.ogg"))  // https://www.freesoundeffects.com/   ----  Free for personal use non-commercial use  --- creator not specified
    {
        return EXIT_FAILURE;
    }

    if (!level2.loadFromFile("images/level2.png")) // https://www.deviantart.com/moonglowlilly/art/Fantasy-Bg-Stock-48-368101344 -------  Fair use --------- artist: Moonglowlilly
    {
        return EXIT_FAILURE;
    }

    if (!fireball.loadFromFile("images/fireball.png"))  // http://coolvibe.com   ---- for your personal, non-commercial use only   ---- artist: Wang Xiaoyu, China
    {
        return EXIT_FAILURE;
    }

    if (!ogre.loadFromFile("images/ogre.png")) // https://paintingvalley.com/ogre-painting#ogre-painting-14.jpg -----------  Personal use CC BY-NC 4.0 Licence --------- artist: not specified
    {
        return EXIT_FAILURE;
    }
    if (!wolf.loadFromFile("images/wolf.png")) // https://www.deviantart.com/zoppy/art/Night-Werewolf-348063640 -----  Fair use   ------ artist: Zoppy
    {
        return EXIT_FAILURE;
    }

    if(!font1.loadFromFile("fonts/halfelven.otf")) // https://www.1001freefonts.com/ -------- free for personal use -------
    {
        return EXIT_FAILURE;
    }

    if(!t1.loadFromFile("images/map1.png")) // https://www.deviantart.com/moonglowlilly/art/Fantasy-Bg-388-368640628 -------  Fair use --------- artist: Moonglowlilly
    {
        return EXIT_FAILURE;
    }

    if(!t2.loadFromFile("images/mainplatform.png")) //  created myself using 3D paint on windows.
    {
        return EXIT_FAILURE;
    }

    if(!t3.loadFromFile("images/wizard.png")) // https://gmispace.com/AYVBmgI+20MAJWM1Qrlfl6pOhEmaAAl4-1X5g-fantasy-human-wizard  -------  free for personal use -------- artist: not specified
    {
        return EXIT_FAILURE;
    }


    if(!power.loadFromFile("images/potion.png")) // https://www.pinterest.com/pin/347551296219777675/  -------- free for personal use  -------- artist: GravityArchangel
    {
        return EXIT_FAILURE;
    }

    Sound enemy_hit_sound;
    Sound game_over_scream;
    enemy_hit_sound.setBuffer(enemy_hit);
    game_over_scream.setBuffer(game_over_sound);

    // creates a sprite object for the three Texture objects
    Sprite sBackground(t1), sPlat(t2), sPers(t3), enemy(ogre), attack(fireball), enemy2(ogre),enemy3(wolf), level_two(level2), power_up(power);


    // creates a list of size 20 that stores the platforms in the game
    point plat[20];


    Text game_over;
    Text score;
    int score_count = 0;
    string score_string;

    game_over.setFont(font1);
    game_over.setString("Game Over");
    game_over.setCharacterSize(60);
    game_over.setFillColor(Color::White);
    game_over.setStyle(Text::Bold | Text::Underlined);
    game_over.setPosition(400/2.0f,(533.0/2.0f));
    game_over.setOrigin(game_over.getLocalBounds().width/2.0f,game_over.getLocalBounds().height/2.0f);

    score.setFont(font1);
    score.setString("0");
    score.setCharacterSize(20);
    score.setFillColor(Color::White);
    score.setPosition(score.getLocalBounds().width+3,score.getLocalBounds().height);

    // generates the platforms in the game
    for (int i=0;i<10;i++)
    {
        plat[i].setx(rand()%400);
        plat[i].sety(rand()%533);
    }
    int count = 0;
    int weaponx = 0, weapony = 0;
    float enemyx = rand()%400, enemyy = -1, enemy2x = rand()%400, enemy2y = -150;
    float enemy3x = rand()%400, enemy3y = -50;
    float potionx = rand()%400 ,potiony = -25;

    // the location of the platforms in the game
    int x=100,y=100,h=200;
    float dx=0,dy=0;

    game_play_song.play();
    // makes it so the game stays open until the user closes the game window
    //***************************************************************************************************************************************************************
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                app.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) // moves the character in the positive x direction if the right key is pressed
        {
            x += 3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) // moves the character in the negative x direction if the left key is pressed
        {
            x -= 3;
        }

        // if the player doesnt reach a platform the sprite keeps bouncing at the bottom of the window
        // if this if statement wasn't used, the sprite would keep falling out of sight in the window
        dy += 0.2;
        y += dy;

        if(400/enemyx < 1.05)
        {
            enemyx = rand()%400;
        }
        if(400/enemy2x < 1.05)
        {
            enemy2x = rand()%400;
        }
        if(400/enemy3x < 1.05)
        {
            enemy3x = rand()%400;
        }
        if(400/potionx < 1.05)
        {
            potionx = rand()%400;
        }

        // moves the sprite in the positive y direction if a platform is reached
        if (y < h)
        {
            score_count += 1;
            score_string = to_string(score_count);
            score.setString(score_string);
            enemyy += 0.3;
            enemy2y += 0.3;
            enemy3y += 0.3;
            potiony += 0.3;
            for (int i = 0; i < 10; i++)
            {
                y = h;
                plat[i].sety(plat[i].gety() - dy);
                if(plat[i].gety() > 533)
                {
                    plat[i].sety(0);
                    plat[i].setx(rand() % 400);
                }
            }
        }
        // allows the sprite to be able to bounce on the platforms
        // if the for statement wasn't included, the sprite would just fall through each platform
        for (int i = 0; i < 10; i++)
        {
            if ((x + 50 > plat[i].getx()) && (x + 20 < plat[i].getx() + 68) && (y + 70 > plat[i].gety()) && (y + 70 < plat[i].gety() + 14) && (dy > 0))
            {
                dy = -10;
            }
        }


        // allows the sprite to be able to move around the window
        // if this wasn't there, the sprite would be immobile
        sPers.setPosition(x,y);
        enemy.setPosition(enemyx,enemyy);
        enemy2.setPosition(enemy2x,enemy2y);
        enemy3.setPosition(enemy3x,enemy3y);
        power_up.setPosition(potionx,potiony);


        if(score_count <= 1500)
       {
           app.draw(sBackground);
           app.draw(enemy);
           app.draw(enemy2);
           if(invincible == false)
           {
               app.draw(power_up);
           }
           enemyy += 1.0;
           enemy2y += 1.0;
           if(invincible == false)
           {
               potiony += 1.0;
           }
       }
       else if (score_count > 1500)
       {
           app.draw(level_two);
           app.draw(enemy3);
           app.draw(power_up);
           enemy3y += 1.0;
           if(invincible == false)
           {
                potiony += 1.0;
           }
       }
        app.draw(sPers);
        app.draw(score);

        if(enemyy > 533)
        {
            enemyy = -50;
        }
        if(enemy2y > 533)
        {
            enemy2y = -75;
        }
        if(enemy3y > 533)
        {
            enemy3y = -50;
        }
        if(potiony > 533)
        {
            potiony = -250;
        }

        elapsed = clock2.getElapsedTime();

        if(spawn)
        {
            weapony = sPers.getPosition().y;
            spawn = false;
        }

        if(elapsed.asMilliseconds()%1 == 0)
        {
            weaponx = sPers.getPosition().x;
            weapony -= 5;
            attack.setPosition(weaponx,weapony);
        }

        app.draw(attack);

        if(weapony < 0)
        {
            spawn = true;
        }

        for (int i = 0; i < 10; i++)
        {
            sPlat.setPosition(plat[i].getx(), plat[i].gety());
            app.draw(sPlat);
        }

        if(sPers.getGlobalBounds().contains(power_up.getPosition().x,power_up.getPosition().y))
        {
            potiony = -200;
            invincible = true;
        }

        if(invincible)
        {
            count += 1;
            if(count > 500)
            {
                invincible = false;
            }
        }


        if(enemy.getGlobalBounds().contains(weaponx,weapony))
        {
            enemy_hit_sound.play();
            enemyx = rand()%400;
            enemyy = -300;
            weaponx = 0;
            weapony = 0;
        }

        if(enemy2.getGlobalBounds().contains(weaponx,weapony))
        {
            enemy_hit_sound.play();
            enemy2x = rand()%400;
            enemy2y = -450;
            weaponx = 0;
            weapony = 0;
        }

        if(enemy3.getGlobalBounds().contains(weaponx,weapony))
        {
            enemy_hit_sound.play();
            enemy3x = rand()%400;
            enemy3y = -50;
            weaponx = 0;
            weapony = 0;
        }

        if(Music::Stopped)
        {
            game_play_song.play();
        }

        // displays the window(game)
        app.display();
        if (y > 500 ||
           (sPers.getGlobalBounds().contains(enemy.getPosition().x,enemy.getPosition().y) && !invincible) ||
           (sPers.getGlobalBounds().contains(enemy2.getPosition().x,enemy2.getPosition().y) && !invincible) ||
           (sPers.getGlobalBounds().contains(enemy3.getPosition().x,enemy3.getPosition().y) && !invincible))
        {
            game_play_song.stop();
            game_over_scream.play();
            dy = -10;
            app.draw(game_over);
            app.display();
            while(active)
            {
                if(Keyboard::isKeyPressed(Keyboard::R))
                {
                    game_over_scream.stop();
                    active = false;
                }
            }
            app.clear();
            active = true;
            score_count = 0;
            x = 100;
            y = 100;
            h = 200;
            dy = 0;
            enemyx = rand()%400;
            enemyy = -25;
            enemy2x = rand()%400;
            enemy2y = -75;
            enemy3x = rand()%400;
            enemy3y = -50;
            weaponx = 0;
            weapony = 0;
            invincible = false;
            game_play_song.play();
            clock2.restart();
            for(int i=0;i<10;i++)
            {
                plat[i].setx(rand()%400);
                plat[i].sety(rand()%533);
            }
        }
        app.clear();
    }

    return 0;
}
