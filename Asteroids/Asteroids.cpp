// Programmer: Zachary Leali
// Name: hw6
// Date: 11/20/2019
// Description:  There are many things that I added to the original design.  I created three levels, two of which are asteroids and the last is a boss fight.  I added sounds to the bullet fire,
//               the explosions of the asteroids, the explosions of the spaceship, and the explosions of the ufos/enemies.  I added a way for the user to see their score.  I added a way for the boss to randomly fire red bullets and
//               if the player gets hit by the bullet they die, as well as dying if hit by the boss itself.  I also created a health bar system for the boss and as the health of the boss gets lower, it becomes more challenging.
//               I added voice sounds when the ufo's are about to come on the screen, when the level changes, when the boss level is reached, and when the boss gets defeated.  I added a start screen where the user has to press enter to start and
//               I added a game over screen which tells the user to press R to restart.  If the boss is killed, the restart screen appears but does not say game over and the score does not get reset back to 0.  I added background music as well as
//               three more classes which were for the boss, the boss's bullets, and the second kind of ufo for level 2.  I added the changing of backgrounds depending on what level you are on.  Lastly, I added the changing of the look of the asteroids
//               depending on what level you are on.  I believe that is it.

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <list>
#include <cmath>

using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f; // variable to change the direction of the bullet

// The Animation class makes it so the objects on the screen can move around.
class Animation
{
public:
	float Frame, speed; // variables for setting number of frames and the speed of the frame
	Sprite sprite; // creating a sprite
	std::vector<IntRect> frames; // creating a vector of IntRects named frames that stores the number of frames equal to the count.

	Animation() {}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++) // creating x number of frames for object
		{
			frames.push_back(IntRect(x + i * w, y, w, h));
		}

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}

	// updates the animation of the object every frame
	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n)
		{
			Frame -= n;
		}

		if (n > 0)
		{
			sprite.setTextureRect(frames[int(Frame)]);
		}

	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}

};

// The base class for all other classes after this
class Entity
{
public:
	float x, y, dx, dy, R, angle; // parameters for the settings function that allows the user to create objects while setting their animation, angle, radius, and location in the window.
	bool life; // a boolean variable to decide whether the object has been destroyed or not.
	std::string name; // a variable to set the name of the objects created using the derived classes.
	Animation anim;

	// sets the life of the object to true.
	Entity()
	{
		life = 1;
	}

	void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
	{
		anim = a;
		x = X;
		y = Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	//draws the animated sprites to the window.
	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
	}

	virtual ~Entity() {};
};

class boss : public Entity
{
public:
	boss()
	{
		name = "boss";
	}
	void update()
	{
		if (x == 100)
		{
			right_move = true;
			left_move = false;
		}
		if (x == 1100)
		{
			left_move = true;
			right_move = false;
		}
		if (right_move)
		{
			x += 2;
		}
		if (left_move)
		{
			x -= 2;
		}
	}
private:
	bool right_move = false;
	bool left_move = false;

};
class ufo : public Entity // <div>Icons made by <a href="https://www.flaticon.com/authors/freepik" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>
{
public:
	ufo()
	{
		name = "ufo";
	}
	void update()
	{
		x += 3;

		if (x > W)
		{
			x = 0;
		}

		if (y > H)
		{
			y = 0;
		}

		if (y < 0)
		{
			y = H;
		}
	}
};

class ufo2 : public Entity
{
public:
	ufo2()
	{
		name = "ufo2";
	}
	void update()
	{
		x += 3;

		if (x > W)
		{
			x = 0;
		}

		if (y > H)
		{
			y = 0;
		}

		if (y < 0)
		{
			y = H;
		}
	}
};

// inherits Entity class and creates each asteroid object
class asteroid : public Entity
{
public:
	asteroid()
	{
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "asteroid";
		count_of_asteroid++; // keeping track of how many asteroids were created.
	}

	~asteroid()
	{
		count_of_asteroid--; // keeping track of how many asteroids were deleted.
	}


	// updates the location of the asteroids on the screen.
	void  update()
	{
		x += dx;
		y += dy;

		if (x > W)
		{
			x = 0;
		}
		if (x < 0)
		{
			x = W;
		}
		if (y > H)
		{
			y = 0;
		}
		if (y < 0)
		{
			y = H;
		}
	}

	// returns the number of asteroids on the screen.
	static unsigned int getCount()
	{
		return count_of_asteroid;
	}
private:
	static unsigned int count_of_asteroid;



};


//inherits Entity class and creates objects named bullet.
class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	// updates the location of the bullet allowing the bullet.
	void  update()
	{
		dx = cos(angle*DEGTORAD) * 6;
		dy = sin(angle*DEGTORAD) * 6;
		x += dx;
		y += dy;

		if (x > W || x<0 || y>H || y < 0)
		{
			life = 0;
		}
	}

};

class bullet_boss : public Entity
{
public:
	bullet_boss()
	{
		name = "bullet_boss";
	}

	void  update()
	{
		dx = cos(angle*DEGTORAD) * 3;
		dy = sin(angle*DEGTORAD) * 3;
		x += dx;
		y += dy;

		if (x > W || x<0 || y>H || y < 0)
		{
			life = 0;
		}
	}

};

// inherits Entity class and creates the main player object.
class player : public Entity
{
public:
	bool thrust;

	player()
	{
		name = "player";
	}

	// updates the location of the player when the user presses certain buttons on the keyboard.
	void update()
	{
		if (thrust)
		{
			dx += cos(angle*DEGTORAD)*0.2;
			dy += sin(angle*DEGTORAD)*0.2;
		}
		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		// the player can only go so fast on the screen
		int maxSpeed = 15;
		float speed = sqrt(dx*dx + dy * dy);
		if (speed > maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x > W)
		{
			x = 0;
		}
		if (x < 0)
		{
			x = W;
		}

		if (y > H)
		{
			y = 0;
		}

		if (y < 0)
		{
			y = H;
		}

	}

};

// tests whether two objects collide by testing whether a number computed is less than the radius of the two objects added and squared.
bool isCollide(Entity *a, Entity *b)
{
	return (b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y) < (a->R + b->R)*(a->R + b->R);
}

unsigned int asteroid::count_of_asteroid{ 0 };

int main() {
	srand(time(0)); // starts the time at 0.

	bool start = true; // variable to allow for the game to start
	bool spaceship_spawn = false; // variable for knowing whether the player's spaceship is on the screen or not.
	bool continue_game = false; // variable to continue the game when dying.
	bool level_1 = true; // running level 1
	bool level_2 = false; // running level 2
	bool level_3 = false; // running level 3
	bool boss_battle = false; // making sure the boss is only spawned once during level 3
	int boss_health = 200; // the level 3 bosses health
	bool choose_level = false; // decides whether to continue in levels or go back to level 1.

	Clock clock; // keeping track of time as to allow the ufo's to spawn at specific times.
	Time time;
	float time2;

	bool ufo_spawn = false; // allows for only one ufo to be on the screen
	bool alarm = false; // allows for the warning for the player before the ufo comes on the screen

	// player score in game
	int score = 0;
	std::string health_string;
	std::string score_string;

	RenderWindow app(VideoMode(W, H), "Asteroids!"); // creating the window
	app.setFramerateLimit(60); // setting the fps to 60.

	// background song
	Music game_play_song;
	if (!game_play_song.openFromFile("sounds/background_music.ogg"))
	{
		return EXIT_FAILURE;
	}

	// creating all the textures for all of the animation objects in the game.
	Texture t1, t2, t3, t4, t5, t6, t7;
	Texture ufo_texture, level_two_texture, level_two_texture_small;
	Texture starship_texture;
	Texture boss_texture, red_bullet_texture;
	Texture level_2_texture, level_3_texture;
	Texture level_3rock_texture, level_3rock_small_texture;
	if (!t1.loadFromFile("images/spaceship.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t2.loadFromFile("images/level_1_background.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_2_texture.loadFromFile("images/level_2_background.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_3_texture.loadFromFile("images/level_3_background.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t3.loadFromFile("images/explosions/type_C.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t4.loadFromFile("images/rock.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t5.loadFromFile("images/fire_blue.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t6.loadFromFile("images/rock_small.png"))
	{
		return EXIT_FAILURE;
	}
	if (!t7.loadFromFile("images/explosions/type_B.png"))
	{
		return EXIT_FAILURE;
	}
	if (!ufo_texture.loadFromFile("images/UFO.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_two_texture.loadFromFile("images/rock_level2.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_two_texture_small.loadFromFile("images/rock_small_level2.png"))
	{
		return EXIT_FAILURE;
	}
	if (!starship_texture.loadFromFile("images/starship.png"))
	{
		return EXIT_FAILURE;
	}
	if (!boss_texture.loadFromFile("images/boss_ship.png"))
	{
		return EXIT_FAILURE;
	}
	if (!red_bullet_texture.loadFromFile("images/fire_red.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_3rock_texture.loadFromFile("images/rock_level3.png"))
	{
		return EXIT_FAILURE;
	}
	if (!level_3rock_small_texture.loadFromFile("images/rock_small_level3.png"))
	{
		return EXIT_FAILURE;
	}

	// setting the backgrounds and the spaceship to smooth
	t1.setSmooth(true);
	level_2_texture.setSmooth(true);
	level_3_texture.setSmooth(true);
	t2.setSmooth(true);

	// creating the sprites for the backgrounds.
	Sprite background(t2);
	Sprite level_2_background(level_2_texture);
	Sprite level_3_background(level_3_texture);

	// Creating all of the Animation objects from all of the textures and setting there width,height,frame count, and frame speed.
	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);
	Animation UFO(ufo_texture, 0, 0, 80, 80, 1, 0.2);
	Animation level_2_rock(level_two_texture, 0, 0, 64, 64, 16, 0.2);
	Animation level_2_rock_small(level_two_texture_small, 0, 0, 64, 64, 16, 0.2);
	Animation starship(starship_texture, 0, 0, 80, 80, 1, 0.2);
	Animation boss_ship(boss_texture, 0, 0, 300, 300, 1, 0.2);
	Animation boss_bullet(red_bullet_texture, 0, 0, 32, 64, 16, 0.8);
	Animation level_3_rock(level_3rock_texture, 0, 0, 64, 64, 16, 0.2);
	Animation level_3_rock_small(level_3rock_small_texture, 0, 0, 64, 64, 16, 0.2);

	// creating sound buffers for the sound effects in the game.
	SoundBuffer ufo_encounter, explosion_sound, ufo_explode_buffer, shoot_buffer;
	SoundBuffer level_1_buffer, level_2_buffer, boss_encounter_buffer, mission_complete_buffer;
	if (!level_1_buffer.loadFromFile("sounds/level_1_speech.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!level_2_buffer.loadFromFile("sounds/level_2_speech.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!boss_encounter_buffer.loadFromFile("sounds/boss_encounter.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!mission_complete_buffer.loadFromFile("sounds/mission_complete.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!ufo_encounter.loadFromFile("sounds/ufo_alarm.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!explosion_sound.loadFromFile("sounds/Explosion1.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!ufo_explode_buffer.loadFromFile("sounds/ufo_explosion.ogg"))
	{
		return EXIT_FAILURE;
	}
	if (!shoot_buffer.loadFromFile("sounds/fire_sound.ogg"))
	{
		return EXIT_FAILURE;
	}

	// Creating the sounds for the sound effects and setting their volume.
	Sound level_1_sound, level_2_sound, boss_encounter, mission_complete;

	level_1_sound.setBuffer(level_1_buffer);
	level_1_sound.setVolume(200.0);
	level_2_sound.setBuffer(level_2_buffer);
	level_2_sound.setVolume(200.0);
	boss_encounter.setBuffer(boss_encounter_buffer);
	boss_encounter.setVolume(200.0);
	mission_complete.setBuffer(mission_complete_buffer);
	mission_complete.setVolume(200.0);


	Sound ufo_on_screen, explosion, ufo_explosion, shoot_sound;

	ufo_on_screen.setBuffer(ufo_encounter);
	ufo_on_screen.setVolume(200.0);
	explosion.setBuffer(explosion_sound);
	ufo_explosion.setBuffer(ufo_explode_buffer);
	shoot_sound.setBuffer(shoot_buffer);
	shoot_sound.setVolume(100.0);

	// creating and setting the font for the text in the game.
	Font score_count;
	if (!score_count.loadFromFile("Fonts/begokv15__2015___free.otf"))
	{
		return EXIT_FAILURE;
	}
	// Creating texts for game and setting their string,color,size,and position.
	Text player_score, start_game, asteroids_title, game_over, restart_game, boss_health_points, continue_gameplay;
	player_score.setFont(score_count);
	player_score.setString("0");
	player_score.setFillColor(Color::White);
	player_score.setCharacterSize(20);
	player_score.setPosition(10, 10);

	start_game.setFont(score_count);
	start_game.setString("press enter to start");
	start_game.setCharacterSize(25);
	start_game.setPosition(1200 / 2.0f, (800 / 2.0f));
	start_game.setOrigin(start_game.getLocalBounds().width / 2.0f, start_game.getLocalBounds().height / 2.0f);

	asteroids_title.setFont(score_count);
	asteroids_title.setString("asteroids");
	asteroids_title.setCharacterSize(25);
	asteroids_title.setPosition(1200 / 2.0f, (400 / 2.0f));
	asteroids_title.setOrigin(asteroids_title.getLocalBounds().width / 2.0f,
		asteroids_title.getLocalBounds().height / 2.0f);

	game_over.setFont(score_count);
	game_over.setString("game over");
	game_over.setCharacterSize(50);
	game_over.setPosition(1200 / 2.0f, (800 / 2.0f));
	game_over.setOrigin(game_over.getLocalBounds().width / 2.0f, game_over.getLocalBounds().height / 2.0f);

	restart_game.setFont(score_count);
	restart_game.setString("press  r to continue");
	restart_game.setCharacterSize(25);
	restart_game.setFillColor(Color::Black);
	restart_game.setPosition(1200 / 2.0f, (1200 / 2.0f));
	restart_game.setOrigin(game_over.getLocalBounds().width / 2.0f, game_over.getLocalBounds().height / 2.0f);

	boss_health_points.setFont(score_count);
	boss_health_points.setString("200");
	boss_health_points.setFillColor(Color::White);
	boss_health_points.setCharacterSize(20);
	boss_health_points.setPosition(10, 10);

	continue_gameplay.setFont(score_count);
	continue_gameplay.setString("ready for next fight?");
	continue_gameplay.setFillColor(Color::White);
	continue_gameplay.setCharacterSize(25);
	continue_gameplay.setPosition(1200 / 2.0f, (800 / 2.0f));
	continue_gameplay.setOrigin(continue_gameplay.getLocalBounds().width / 2.0f, continue_gameplay.getLocalBounds().height / 2.0f);





	// creating a list of entities
	std::list<Entity *> entities;
	// creating the player object and setting its position on screen as well as its radius.
	player *p = new player();
	p->settings(sPlayer, 600, 400, 0, 20);
	entities.push_back(p); // adds the object to the end of the list of entities.


	// creating all of the asteroid objects.
	for (int i = 0; i < 15; i++)
	{
		asteroid *a = new asteroid();
		a->settings(sRock, 0, rand() % H, rand() % 360, 25);
		entities.push_back(a); // adds the objects to the end of the list of entities.
	}




	/////main loop/////
	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed) // if the exit button is pressed the game closes
			{
				app.close();
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Space) // if space is pressed on the keyboard then a bullet object is created at the position of the player object.
				{
					bullet *b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
					shoot_sound.play();
				}
			}
		}


		// allows the user to press enter to start the game.
		if (start == true)
		{
			game_play_song.setLoop(true);
			if (game_play_song.getLoop())
			{
				game_play_song.play();
			}
			clock.restart();
			player_score.setPosition(10, 10);
			player_score.setOrigin(0, 0);
			spaceship_spawn = false;
			continue_game = false;
			app.clear();
			app.draw(background);
			app.draw(asteroids_title);
			app.draw(start_game);
			app.display();
			while (start)
			{
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					level_1_sound.play();
					start = false;
					app.clear();
				}
			}
		}

		// Beginning of level one of game
		if (level_1 == true)
		{
			time = clock.getElapsedTime();
			time2 = time.asSeconds();
			if ((time2 <= 5.0) && (time2 >= 4.8) && ufo_spawn == false && alarm == false)
			{
				ufo_on_screen.play();
				game_play_song.setVolume(20.0);
				alarm = true;
			}
			if ((time2 <= 10.05) && (time2 >= 10.0) && ufo_spawn == false)
			{
				game_play_song.setVolume(100);
				ufo_on_screen.stop();
				alarm = true;
				ufo_spawn = true;
				ufo *u = new ufo();
				u->settings(UFO, 0, rand() % H, -90, 25);
				entities.push_back(u);
			}

			// if the user presses any of these keys, then the player object on the screen moves in the direction or the angle changes.
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p->angle += 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p->angle -= 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				p->thrust = true;
			}
			else {
				p->thrust = false;
			}

			// explosion and reset of **ufo is hit by bullet** and **ufo if hit by player**
			if (ufo_spawn == true)
			{
				for (auto b : entities)
				{
					for (auto u : entities)
					{
						if (u->name == "ufo" && b->name == "bullet")
						{
							if (isCollide(u, b))
							{
								choose_level = false;
								score = score + 50;
								score_string = std::to_string(score);
								player_score.setString(score_string);
								ufo_explosion.play();
								clock.restart();
								b->life = false;
								u->life = false;
								ufo_spawn = false;
								alarm = false;
								Entity *e = new Entity();
								e->settings(sExplosion, u->x, u->y);
								e->name = "explosion";
								entities.push_back(e);
								u->settings(UFO, 0, rand() % H, -90, 25);
								u->dx = 0;
								u->dy = 0;
							}
						}
						if (b->name == "player" && u->name == "ufo")
						{
							if (isCollide(b, u)) {
								game_play_song.stop();
								ufo_on_screen.stop();
								choose_level = true;
								score = 0;
								score_string = std::to_string(score);
								player_score.setString(score_string);
								alarm = false;
								ufo_explosion.play();
								u->life = false;
								b->life = false;
								Entity *e = new Entity();
								e->settings(sExplosion_ship, b->x, b->y);
								e->name = "explosion";
								entities.push_back(e);
								p->settings(sPlayer, 600, 400, 0, 20);
								p->dx = 0;
								p->dy = 0;
								app.clear();
								app.draw(background);
								app.draw(game_over);
								app.draw(restart_game);
								player_score.setPosition(1200 / 2.0f, 300);
								player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
									player_score.getLocalBounds().height / 2.0f);
								app.draw(player_score);
								app.display();
								while (spaceship_spawn == false) // if the player gets hit by the ufo, the screen shows the players score and if the user presses R then the game resets to the main screen.
								{
									if (Keyboard::isKeyPressed(Keyboard::R))
									{
										score = 0;
										continue_game = true;
										choose_level = true;
										level_1 = true;
										level_2 = false;
										level_3 = false;
										score_string = std::to_string(score);
										player_score.setString(score_string);
										start = true;
										spaceship_spawn = true;
										app.clear();
									}
								}

							}
						}
					}
				}
			}

			// explosion if **asteroid hit by bullet** and
			for (auto a : entities)
			{
				for (auto b : entities)
				{
					if ((a->name == "asteroid" && b->name == "bullet"))
					{
						if (isCollide(a, b))
						{
							choose_level = false;
							score = score + 10;
							score_string = std::to_string(score);
							player_score.setString(score_string);
							explosion.play();
							a->life = false;
							b->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							for (int i = 0; i < 2; i++) { // if the asteroid is hit by a bullet then the asteroid is destroyed and then two more asteroid objects are created at the location of the explosion and they are saved as the smaller asteroids
								if (a->R == 15) continue;
								Entity *e = new asteroid();
								e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
								entities.push_back(e);
							}
						}
					}
					if (a->name == "player" && b->name == "asteroid")
					{
						if (isCollide(a, b))
						{
							game_play_song.stop();
							ufo_on_screen.stop();
							choose_level = true;
							score_string = std::to_string(score);
							player_score.setString(score_string);
							ufo_explosion.play();
							b->life = false;
							a->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0;
							p->dy = 0;
							app.clear();
							app.draw(background);
							app.draw(game_over);
							app.draw(restart_game);
							player_score.setPosition(1200 / 2.0f, 300);
							player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
								player_score.getLocalBounds().height / 2.0f);
							app.draw(player_score);
							app.display();
							while (spaceship_spawn == false)
							{
								if (Keyboard::isKeyPressed(Keyboard::R))
								{
									score = 0;
									choose_level = true;
									level_1 = true;
									level_2 = false;
									level_3 = false;
									continue_game = true;
									score_string = std::to_string(score);
									player_score.setString(score_string);
									start = true;
									spaceship_spawn = true;
									app.clear();
								}
							}
						}
					}
				}
			}


			// allows for the animation of the player object when moving forward.
			if (p->thrust)
			{
				p->anim = sPlayer_go;
			}
			else
			{
				p->anim = sPlayer;
			}


			// searches through all of the entitites in the list and if the name of the object in the list is "explosion" then the function isEnd() is called and the life is set to 0.
			for (auto e : entities)
			{
				if (e->name == "explosion")
				{
					if (e->anim.isEnd())
					{
						e->life = 0;
					}
				}
			}

			// allows for more asteroids to appear after all of the previous ones have been destroyed.
			if (asteroid::getCount() == 0)
			{
				ufo_on_screen.stop();
				if (choose_level == false)
				{
					level_2_sound.play();
					level_1 = false;
					level_2 = true;
					level_3 = false;
					ufo_spawn = false;
					alarm = false;
					for (int i = 0; i < 15; i++)
					{
						asteroid *a = new asteroid();
						a->settings(level_2_rock, 0, rand() % H, rand() % 360, 25);
						entities.push_back(a);
					}
				}
				else if (choose_level == true)
				{
					for (int i = 0; i < 15; i++)
					{
						level_1 = true;
						level_2 = false;
						level_3 = false;
						asteroid *a = new asteroid();
						a->settings(sRock, 0, rand() % H, rand() % 360, 25);
						entities.push_back(a);
					}
				}
				clock.restart();
			}
		}

		//-----------------------------------------------------------------------------------------------------------------------------
		if (level_2 == true)
		{
			for (auto a : entities)
			{
				if (a->name == "ufo" && a->life == true)
				{
					a->life = false;
				}
			}
			time = clock.getElapsedTime();
			time2 = time.asSeconds();
			if ((time2 <= 5.0) && (time2 >= 4.8) && ufo_spawn == false && alarm == false)
			{
				game_play_song.setVolume(20.0);
				ufo_on_screen.play();
				alarm = true;
			}
			if ((time2 <= 10.05) && (time2 >= 10.0) && ufo_spawn == false)
			{
				game_play_song.setVolume(100.0);
				ufo_on_screen.stop();
				alarm = true;
				ufo_spawn = true;
				ufo2 *u = new ufo2();
				u->settings(starship, 0, rand() % H, -90, 25);
				entities.push_back(u);
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p->angle += 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p->angle -= 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				p->thrust = true;
			}
			else
			{
				p->thrust = false;
			}

			// explosion and reset of **ufo is hit by bullet** and **ufo if hit by player**
			if (ufo_spawn == true)
			{
				for (auto b : entities)
				{
					for (auto u : entities)
					{
						if (u->name == "ufo2" && b->name == "bullet")
						{
							if (isCollide(u, b))
							{
								choose_level = false;
								score = score + 50;
								score_string = std::to_string(score);
								player_score.setString(score_string);
								ufo_explosion.play();
								clock.restart();
								b->life = false;
								u->life = false;
								ufo_spawn = false;
								alarm = false;
								Entity *e = new Entity();
								e->settings(sExplosion, u->x, u->y);
								e->name = "explosion";
								entities.push_back(e);
								u->settings(starship, 0, rand() % H, -90, 25);
								u->dx = 0;
								u->dy = 0;
							}
						}
						if (b->name == "player" && u->name == "ufo2")
						{
							if (isCollide(b, u)) {
								game_play_song.stop();
								choose_level = true;
								ufo_on_screen.stop();
								score = 0;
								score_string = std::to_string(score);
								player_score.setString(score_string);
								alarm = false;
								ufo_explosion.play();
								u->life = false;
								b->life = false;
								Entity *e = new Entity();
								e->settings(sExplosion_ship, b->x, b->y);
								e->name = "explosion";
								entities.push_back(e);
								p->settings(sPlayer, 600, 400, 0, 20);
								p->dx = 0;
								p->dy = 0;
								app.clear();
								app.draw(background);
								app.draw(game_over);
								app.draw(restart_game);
								player_score.setPosition(1200 / 2.0f, 300);
								player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
									player_score.getLocalBounds().height / 2.0f);
								app.draw(player_score);
								app.display();
								while (spaceship_spawn == false)
								{
									if (Keyboard::isKeyPressed(Keyboard::R))
									{
										score = 0;
										continue_game = true;
										choose_level = true;
										level_1 = true;
										level_2 = false;
										level_3 = false;
										score_string = std::to_string(score);
										player_score.setString(score_string);
										start = true;
										spaceship_spawn = true;
										app.clear();
									}
								}
							}
						}
					}
				}
			}

			// explosion if **asteroid hit by bullet** and
			for (auto a : entities)
			{
				for (auto b : entities)
				{
					if ((a->name == "asteroid" && b->name == "bullet"))
					{
						if (isCollide(a, b))
						{
							choose_level = false;
							score = score + 10;
							score_string = std::to_string(score);
							player_score.setString(score_string);
							explosion.play();
							a->life = false;
							b->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							for (int i = 0; i < 2; i++)
							{
								if (a->R == 15) continue;
								Entity *e = new asteroid();
								e->settings(level_2_rock_small, a->x, a->y, rand() % 360, 15);
								entities.push_back(e);
							}
						}
					}
					if (a->name == "player" && b->name == "asteroid")
					{
						if (isCollide(a, b))
						{
							game_play_song.stop();
							choose_level = true;
							ufo_on_screen.stop();
							score_string = std::to_string(score);
							player_score.setString(score_string);
							ufo_explosion.play();
							b->life = false;
							a->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0;
							p->dy = 0;
							app.clear();
							app.draw(background);
							app.draw(game_over);
							app.draw(restart_game);
							player_score.setPosition(1200 / 2.0f, 300);
							player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
								player_score.getLocalBounds().height / 2.0f);
							app.draw(player_score);
							app.display();
							while (spaceship_spawn == false)
							{
								if (Keyboard::isKeyPressed(Keyboard::R))
								{
									score = 0;
									continue_game = true;
									choose_level = true;
									level_1 = true;
									level_2 = false;
									level_3 = false;
									score_string = std::to_string(score);
									player_score.setString(score_string);
									start = true;
									spaceship_spawn = true;
									app.clear();
								}
							}
						}
					}
				}
			}

			if (p->thrust)
			{
				p->anim = sPlayer_go;
			}
			else
			{
				p->anim = sPlayer;
			}

			for (auto e : entities)
			{
				if (e->name == "explosion")
				{
					if (e->anim.isEnd())
					{
						e->life = 0;
					}
				}
			}

			if (asteroid::getCount() == 0)
			{
				ufo_on_screen.stop();
				if (choose_level == false)
				{
					boss_encounter.play();
					level_1 = false;
					level_2 = false;
					level_3 = true;
				}
				else if (choose_level == true)
				{
					for (int i = 0; i < 15; i++)
					{
						asteroid *a = new asteroid();
						a->settings(sRock, 0, rand() % H, rand() % 360, 25);
						entities.push_back(a);
					}
				}
				clock.restart();
			}
		}

		//-----------------------------------------------------------------------------------------------------------
		if (level_3 == true)
		{
			time = clock.getElapsedTime();
			for (auto a : entities)
			{
				if (a->name == "ufo2" && a->life == true)
				{
					a->life = false;
				}
			}
			if (boss_battle == false)
			{
				boss_health_points.setPosition(10, 30);
				boss_health_points.setOrigin(0, 0);
				boss *b = new boss();
				b->settings(boss_ship, 100, 150, -90, 125);
				entities.push_back(b);
				boss_battle = true;
			}

			for (auto b : entities)
			{
				if (boss_health >= 150)
				{
					if (b->name == "boss" && time.asMilliseconds() % 20 == 0)
					{
						bullet_boss *c = new bullet_boss();
						c->settings(boss_bullet, b->x, b->y, rand() % 180, 10);
						entities.push_back(c);
					}
				}
				if (boss_health < 150 && boss_health >= 100)
				{
					if (b->name == "boss" && time.asMilliseconds() % 15 == 0)
					{
						bullet_boss *c = new bullet_boss();
						c->settings(boss_bullet, b->x, b->y, rand() % 180, 10);
						entities.push_back(c);
					}
				}
				if (boss_health < 100)
				{
					if (b->name == "boss" && time.asMilliseconds() % 10 == 0)
					{
						bullet_boss *c = new bullet_boss();
						c->settings(boss_bullet, b->x, b->y, rand() % 180, 10);
						entities.push_back(c);
					}
					if (asteroid::getCount() == 0)
					{
						for (int i = 0; i < 5; i++)
						{
							asteroid *a = new asteroid();
							a->settings(level_3_rock, 0, rand() % H, rand() % 360, 25);
							entities.push_back(a);
						}
					}
				}
				for (auto a : entities)
				{
					if (b->name == "asteroid" && a->name == "player")
					{
						if (isCollide(b, a))
						{
							game_play_song.stop();
							score_string = std::to_string(score);
							player_score.setString(score_string);
							ufo_explosion.play();
							b->life = false;
							a->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0;
							p->dy = 0;
							app.clear();
							app.draw(background);
							app.draw(game_over);
							app.draw(restart_game);
							player_score.setPosition(1200 / 2.0f, 300);
							player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
								player_score.getLocalBounds().height / 2.0f);
							app.draw(player_score);
							app.display();
							while (spaceship_spawn == false)
							{
								if (Keyboard::isKeyPressed(Keyboard::R))
								{
									score = 0;
									boss_health = 200;
									boss_battle = false;
									continue_game = true;
									choose_level = true;
									level_1 = true;
									level_2 = false;
									level_3 = false;
									score_string = std::to_string(score);
									player_score.setString(score_string);
									start = true;
									spaceship_spawn = true;
									app.clear();
								}
							}
						}
					}
					if (b->name == "asteroid" && a->name == "bullet")
					{
						if (isCollide(b, a))
						{
							score = score + 10;
							score_string = std::to_string(score);
							player_score.setString(score_string);
							explosion.play();
							a->life = false;
							b->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							for (int i = 0; i < 2; i++) {
								if (b->R == 15) continue;
								Entity *e = new asteroid();
								e->settings(level_3_rock_small, b->x, b->y, rand() % 360, 15);
								entities.push_back(e);
							}
						}
					}
					if (b->name == "boss" && a->name == "bullet")
					{
						if (isCollide(b, a))
						{
							a->life = false;
							boss_health -= 1;
							health_string = std::to_string(boss_health);
							boss_health_points.setString(health_string);
						}
					}
					if (b->name == "player" && a->name == "bullet_boss")
					{
						if (isCollide(b, a))
						{
							game_play_song.stop();
							ufo_explosion.play();
							a->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0;
							p->dy = 0;
							app.clear();
							app.draw(background);
							app.draw(game_over);
							app.draw(restart_game);
							player_score.setPosition(1200 / 2.0f, 300);
							player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
								player_score.getLocalBounds().height / 2.0f);
							app.draw(player_score);
							app.display();
							while (spaceship_spawn == false)
							{
								if (Keyboard::isKeyPressed(Keyboard::R))
								{
									score = 0;
									boss_health = 200;
									boss_battle = false;
									continue_game = true;
									choose_level = true;
									level_1 = true;
									level_2 = false;
									level_3 = false;
									score_string = std::to_string(score);
									player_score.setString(score_string);
									start = true;
									spaceship_spawn = true;
									app.clear();
								}
							}

						}
					}
					if (b->name == "boss" && a->name == "player")
					{
						if (isCollide(b, a))
						{
							game_play_song.stop();
							ufo_explosion.play();
							a->life = false;
							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);
							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0;
							p->dy = 0;
							app.clear();
							app.draw(background);
							app.draw(game_over);
							app.draw(restart_game);
							player_score.setPosition(1200 / 2.0f, 300);
							player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
								player_score.getLocalBounds().height / 2.0f);
							app.draw(player_score);
							app.display();
							while (spaceship_spawn == false)
							{
								if (Keyboard::isKeyPressed(Keyboard::R))
								{
									score = 0;
									boss_health = 200;
									boss_battle = false;
									continue_game = true;
									choose_level = true;
									level_1 = true;
									level_2 = false;
									level_3 = false;
									score_string = std::to_string(score);
									player_score.setString(score_string);
									start = true;
									spaceship_spawn = true;
									app.clear();
								}
							}
						}
					}
				}
				if (b->name == "boss" && boss_health == 0)
				{
					mission_complete.play();
					score = score + 1000;
					score_string = std::to_string(score);
					player_score.setString(score_string);
					b->life = false;
					Entity *e = new Entity();
					e->settings(sExplosion, b->x, b->y);
					e->name = "explosion";
					entities.push_back(e);
					app.clear();
					app.draw(background);
					app.draw(continue_gameplay);
					app.draw(restart_game);
					player_score.setPosition(1200 / 2.0f, 300);
					player_score.setOrigin(player_score.getLocalBounds().width / 2.0f,
						player_score.getLocalBounds().height / 2.0f);
					app.draw(player_score);
					app.display();
					while (spaceship_spawn == false)
					{
						if (Keyboard::isKeyPressed(Keyboard::R))
						{
							boss_health = 200;
							boss_battle = false;
							continue_game = true;
							choose_level = true;
							level_1 = true;
							level_2 = false;
							level_3 = false;
							score_string = std::to_string(score);
							player_score.setString(score_string);
							start = true;
							spaceship_spawn = true;
							app.clear();
						}
					}
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p->angle += 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p->angle -= 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				p->thrust = true;
			}
			else
			{
				p->thrust = false;
			}

			if (p->thrust)
			{
				p->anim = sPlayer_go;
			}
			else
			{
				p->anim = sPlayer;
			}

			for (auto e : entities)
			{
				if (e->name == "explosion")
				{
					if (e->anim.isEnd())
					{
						e->life = 0;
					}
				}
			}
		}

		// updates all of the objects and their animation
		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity *e = *i;

			e->update();
			e->anim.update();

			if ((e->life == false && e->name != "player") || (continue_game == true && e->name != "player")) // deletes an object if its life is false but not if it is the player
			{
				i = entities.erase(i);
				delete e;
			}
			else
			{
				i++; // else continues through list.
			}
		}

		//////draw//////
		if (level_1 == true)
		{
			app.draw(background);
		}
		if (level_2 == true)
		{
			app.draw(level_2_background);
		}
		if (level_3 == true)
		{
			app.draw(level_3_background);
		}

		// draws all of the objects created in the list of entities to the screen.
		for (auto i : entities)
		{
			i->draw(app);
		}
		app.draw(player_score);
		if (level_3 == true)
		{
			app.draw(boss_health_points);
		}
		app.display(); // displays everything drawn to the window.
	}

	return 0;
}

