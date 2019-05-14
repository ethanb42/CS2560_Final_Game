// SFML_Shooter.cpp : A basic 2D shooter 
#include "pch.h"
#include <SFML/Graphics.hpp>   //using namespace sf 
#include <time.h> 
#include <iostream>
#include <cstdbool>

//dimensions for window size and background 
int num_vertBox = 18, num_horzBox = 32;
int size = 16; //number of pixels 
int w = size * num_horzBox;  //background number of pixels in width 
int h = size * num_vertBox;    //background number of pixels in height 

//The direction of player one
int direction;    //direction the player is moving

//The direction of player two
int directionTwo; //direction the second player is moving

//Portal: An object capable of teleportation
//A portal teleports a player to a random location 
//A portal teleports a bullet to a random location and random direction
struct Portal {
	int x, y;
};
//Create a portal
Portal portal;



//The Struct of a player in the game
struct Player
{
	int x, y;           //each sprite needs an x,y coordinate 
	int alive = true;  //A boolean to keep track of a playeres status
	int dirTemp = -1;  //the last location a player moved
};

//Create the two players
Player p1;  //player one
Player p2;  //player two


//This represents the bullets fired by each player
struct Bullet
{
	int x, y;            //the coordinates for the bullet 
	int direction = -1;  //the direction of the bullet
	bool fired = false;  //the bullet was fired boolean
};

//Create an array of bullets for both players
Bullet b1[3];
Bullet b2[3];


//Updates player ones bullets location and checks for collisions
void bulletOneMove() {
	//For the 3 bullets in player one's clip
	for (int i = 0; i < 3; i++) {

		//Check if the bullet has not been fired first
		//If it wasn't the bullet will continue to follow the player
		if (b1[i].fired == false) {
			//do nothing
			b1[i].x = p1.x;
			b1[i].y = p1.y;
			b1[i].direction = p1.dirTemp;

		}
		//The bullet has been fired
		else {
			//move up
			if (b1[i].direction == 0) {
				b1[i].y -= 1;
			}
			//move down
			if (b1[i].direction == 1) {
				b1[i].y += 1;
			}
			//move left
			if (b1[i].direction == 2) {
				b1[i].x -= 1;
			}
			//move right
			if (b1[i].direction == 3) {
				b1[i].x += 1;
			}

			////////////////////////////
			//////COLLISION CHECKS//////
			////////////////////////////

			//Order of checking does matter and can change the game mechanics
			//It is kind of like order of opperations

			//1st: We check if the bullet fired interacted with a portal
			if (b1[i].x == portal.x && b1[i].y == portal.y) {
				int random1 = rand() % num_horzBox; //randomly generated x-location in the window
				//Both the bullet and the portal teleport to this new x-value
				b1[i].x = random1;
				portal.x = random1;

				int random2 = rand() % num_vertBox; //randomly generated y-location in the window
				//both the bullet and the portal teleport to this new y-value
				b1[i].y = random2;
				portal.y = random2;
				//The bullet randomly changes direction too
				b1[i].direction = rand() % 4;
			}


			//2nd: Check for collision with player
			if (b1[i].x == p2.x && b1[i].y == p2.y) {
				//Player two was hit
				std::cout << "Player 2 Hit!" << std::endl;
				//Update the bullet
				b1[i].fired = false; //The bullet is no longer fired
				//return the bullet to its player
				b1[i].x = p1.x;
				b1[i].y = p1.y;
				b1[i].direction = p1.dirTemp;
				//Kill the player
				p2.alive = false;
			}

			//3rd: Check for collision with enemy bullet
			//loop through all the enemies bullets
			for (int j = 0; j < 3; j++) {
				//if the bullets collided
				if (b1[i].x == b2[j].x && b1[i].y == b2[j].y) {
					//the bullets get reset back to their players
					b2[j].x = p2.x;
					b2[j].y = p2.y;
					b1[i].x = p1.x;
					b1[i].y = p1.y;

					//Both bullets data is updated
					b1[i].direction = p1.dirTemp;
					b2[j].direction = p2.dirTemp;
					b1[i].fired = false;
					b2[j].fired = false;
				}
			}

			//4th: Check for collision with wall
			//Right
			if (b1[i].x > num_horzBox - 1) {
				b1[i].fired = false;
				b1[i].x = p1.x;
				b1[i].y = p1.y;
				b1[i].direction = p1.dirTemp;
			}

			//Left
			if (b1[i].x < 0) {
				b1[i].fired = false;
				b1[i].x = p1.x;
				b1[i].y = p1.y;
				b1[i].direction = p1.dirTemp;
			}

			//Bottom
			if (b1[i].y > num_vertBox - 1) {
				b1[i].fired = false;
				b1[i].x = p1.x;
				b1[i].y = p1.y;
				b1[i].direction = p1.dirTemp;
			}
			//Top
			if (b1[i].y < 0) {
				b1[i].fired = false;
				b1[i].x = p1.x;
				b1[i].y = p1.y;
				b1[i].direction = p1.dirTemp;
			}

		}
	}
}

// Updates player two's bullets location and checks for collisions
void bulletTwoMove() {
	//For all of player two's bullets
	for (int i = 0; i < 3; i++) {
		//Check if the bullet is fired
		if (b2[i].fired == false) {
			//It wasn't so it followed the player (stays in their "clip")
			b2[i].x = p2.x;
			b2[i].y = p2.y;
			b2[i].direction = p2.dirTemp;

		}
		else {
			//Move up
			if (b2[i].direction == 0) {
				b2[i].y -= 1;
			}
			//move down
			if (b2[i].direction == 1) {
				b2[i].y += 1;
			}
			//move left
			if (b2[i].direction == 2) {
				b2[i].x -= 1;
			}
			//move rigth
			if (b2[i].direction == 3) {
				b2[i].x += 1;
			}

			//1st: Check for portal collision first
			if (b2[i].x == portal.x && b2[i].y == portal.y) {
				int random1 = rand() % num_horzBox;  //randomly generated x-location in the window
				b2[i].x = random1;
				portal.x = random1;
				int random2 = rand() % num_vertBox;  //randomly generated y-location in the window
				b2[i].y = random2;
				portal.y = random2;
				//Randomly changes the bullets direction
				b2[i].direction = rand() % 4;

			}
			//2nd: Check for collision with player
			//Checks if a bullet hit the player
			if (b2[i].x == p1.x && b2[i].y == p1.y) {
				//Player two was hit
				std::cout << "Player 1 Hit!" << std::endl;
				//Update the bullets data
				b2[i].fired = false;
				b2[i].x = p2.x;
				b2[i].y = p2.y;
				b2[i].direction = p2.dirTemp;
				//Kill the player
				p1.alive = false;
			}

			//3rd: Check for collision with enemy bullet
			for (int j = 0; j < 3; j++) {
				if (b2[i].x == b1[j].x && b2[i].y == b1[j].y) {
					//the bullets collided
					//set both bullets back to the players
					b2[i].x = p2.x;
					b2[i].y = p2.y;

					b1[j].x = p1.x;
					b1[j].y = p1.y;

					b1[j].direction = p1.dirTemp;
					b2[i].direction = p2.dirTemp;

					//set their fired boolean to false
					b2[i].fired = false;
					b1[j].fired = false;

				}
			}
			//$th: Check for collision with wall
			//Right
			if (b2[i].x > num_horzBox - 1) {
				b2[i].fired = false;
				b2[i].x = p2.x;
				b2[i].y = p2.y;
				b2[i].direction = p2.dirTemp;
			}

			//Left
			if (b2[i].x < 0) {
				b2[i].fired = false;
				b2[i].x = p2.x;
				b2[i].y = p2.y;
				b2[i].direction = p2.dirTemp;
			}

			//Bottom
			if (b2[i].y > num_vertBox - 1) {
				b2[i].fired = false;
				b2[i].x = p2.x;
				b2[i].y = p2.y;
				b2[i].direction = p2.dirTemp;
			}
			//Top
			if (b2[i].y < 0) {
				b2[i].fired = false;
				b2[i].x = p2.x;
				b2[i].y = p2.y;
				b2[i].direction = p2.dirTemp;
			}

		}
	}
}

//Calls both bullet moves
void bulletMoves() {
	bulletOneMove();
	bulletTwoMove();
}


//moves the players
void move() {

	//1st: We want to determine our action from the users input for both players

	//We want to fire the bullet if the players hit their shoot key
	//Player 1: Fire
	if (direction == 4) {
		//Check if we have a bullet to fire
		//If we do it is fired
		for (int i = 0; i < 3; i++) {
			if (b1[i].fired == false) {
				b1[i].fired = true;
				//stops the for loop when we found a suitible bullet
				i = 3;
			}

		}

	}

	//Player 2: Fire
	if (directionTwo == 4) {
		for (int i = 0; i < 3; i++) {
			if (b2[i].fired == false) {
				b2[i].fired = true;

				i = 3;
			}
		}
	}


	//We want to move up if the mapped up key is pressed

	//Player 1: Press up
	if (direction == 0) {
		p1.y -= 1;
	}
	//Player 2: Press up
	if (directionTwo == 0) {
		p2.y -= 1;
	}

	//We want to move down if the mapped down key is pressed
	//Player 1: Press down
	if (direction == 1) {
		p1.y += 1;
	}

	//Player 2: Press down
	if (directionTwo == 1) {
		p2.y += 1;
	}

	//We want to move left if the mapped left key is pressed
	//Player 1: Press left
	if (direction == 2) {
		p1.x -= 1;
	}
	//Player 2: Press right
	if (directionTwo == 2) {
		p2.x -= 1;
	}

	//We want to move righ tif the mapped right key is pressed
	//Player 1: Press right
	if (direction == 3) {
		p1.x += 1;
	}
	//Player 2: Press right
	if (directionTwo == 3) {
		p2.x += 1;
	}


	//2nd: DO COLLISION CHECKS FOR PLAYER MOVEMENT

	//Boundary checking for both of the players
	//Players can not leave the screen

	//LEFT and RIGHT 
	if (p1.x > num_horzBox - 1) {
		p1.x -= 1;
	}
	if (p2.x > num_horzBox - 1) {
		p2.x -= 1;
	}

	if (p1.x < 0) {
		p1.x += 1;
	}
	if (p2.x < 0) {
		p2.x += 1;
	}

	//TOP and BOTTOM 
	if (p1.y > num_vertBox - 1) {
		p1.y -= 1;
	}
	if (p2.y > num_vertBox - 1) {
		p2.y -= 1;
	}

	if (p1.y < 0) {
		p1.y += 1;
	}
	if (p2.y < 0) {
		p2.y += 1;
	}

	//3rd: Record the last direction to each players bullets

	//I need to know the last looking location to determine the shooting path because I set direction to -1 when they are not moving
	for (int i = 0; i < 3; i++) {
		if (direction != -1 && direction != 4) {
			if (b1[i].fired == false) {
				b1[i].direction = direction;
			}
			p1.dirTemp = direction;
		}
		else if (b1[i].fired == false) {
			b1[i].direction = p1.dirTemp;
		}
	}

	for (int i = 0; i < 3; i++) {

		if (directionTwo != -1 && directionTwo != 4) {
			if (b2[i].fired == false) {
				b2[i].direction = directionTwo;
			}
			p2.dirTemp = directionTwo;
		}
		else if (b2[i].fired == false) {
			b2[i].direction = p2.dirTemp;
		}
	}

	//4th: Check for a portal
	if (portal.x == p1.x&& portal.y == p1.y) {
		//teleport the player randomly
		p1.x = rand() % num_horzBox;
		p1.y = rand() % num_vertBox;
		//randomly place the portal
		portal.x = (int)rand() % num_horzBox;
		portal.y = (int)rand() % num_vertBox;
	}

	if (portal.x == p2.x && portal.y == p2.y) {
		//teleport the player randomly
		p2.x = rand() % num_horzBox;
		p2.y = rand() % num_vertBox;
		//randomly place the portal
		portal.x = (int)rand() % num_horzBox;
		portal.y = (int)rand() % num_vertBox;
	}

	//I want them to only move when the key is held down so both direction are reset each clock cycle
	direction = -1;
	directionTwo = -1;

}


int main()
{

	//Setting pseudorandom time 
	srand(time(0));

	//Window that we can play the game in  
	sf::RenderWindow window(sf::VideoMode(w, h), "2D-Shooter");

	//Textures load an image into the GPU Memory 
	sf::Texture t1, t2, t3, t4, t5, t6, t7;
	t1.loadFromFile("image/background.png");
	t2.loadFromFile("image/cat1.png");
	t3.loadFromFile("image/cat2.png");
	t4.loadFromFile("image/fire.png");
	t5.loadFromFile("image/dead.png");
	t6.loadFromFile("image/white.png");
	t7.loadFromFile("image/fire2.png");

	//Sprite has physical dimmensions that can be set in  
	//coordinate system, setPosition(x,y), and drawn on screen 
	sf::Sprite sprite1(t1);
	sf::Sprite sprite2(t2);
	sf::Sprite sprite3(t3);
	sf::Sprite sprite4(t4);
	sf::Sprite sprite5(t5);
	sf::Sprite sprite6(t6);
	sf::Sprite sprite7(t7);


	//Make a clock
	sf::Clock clock;
	float timer = 0.0f, delay = 0.06f;

	//Used to limit player movement
	int counter = 0;    //counts how many "frames" have passed

	//initially place the portal object randomly on the screen
	portal.x = (int)rand() % num_horzBox;
	portal.y = (int)rand() % num_vertBox;

	//Place the players on oppisite sides of the screen
	p1.x = num_horzBox - 1;
	p2.x = 0;
	p1.y = num_vertBox / 2;
	p2.y = num_vertBox / 2;

	while (window.isOpen())
	{
		//timer  
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//Allow us to check when a user does something 
		sf::Event e;

		//Check when the window is closed 
		while (window.pollEvent(e))
		{
			//If user presses x in the top right, Windows, top left, Mac,  close the window 
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}


		if (timer > delay)
		{
			timer = 0;//reset timer 

			//every two times the player moves
			if (counter % 2 == 0) {
				move(); //move the players 
			}
			bulletMoves();
			counter++;
		}


		//Control for Player one 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) direction = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) direction = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) direction = 2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) direction = 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) direction = 4;
		//If the player dies they can no longer move
		if (p1.alive == false) {
			direction = -1;
		}

		//Checks if player one hit the reset key
		//Resets the game 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
			//Both players are set to allive
			p1.alive = true;
			p2.alive = true;
			//They are both set back to their starting position
			p1.x = num_horzBox - 1;
			p2.x = 0;
			p1.y = num_vertBox / 2;
			p2.y = num_vertBox / 2;
			//A reset game message is printed
			std::cout << "Game Reset!\n\n" << std::endl;
		}

		//Control for Player two
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) directionTwo = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) directionTwo = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) directionTwo = 2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) directionTwo = 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) directionTwo = 4;
		//If the player dies they can not move
		if (p2.alive == false) {
			directionTwo = -1;
		}

		/*****************/
		//Draw in window
		/*****************/
		window.clear();    //clear the window so new frame can be drawn in 

		//1st: Draw Background first 
		for (int i = 0; i < num_horzBox; i++)
		{
			for (int j = 0; j < num_vertBox; j++)
			{
				//Set position of sprite1 one at a time 
				sprite1.setPosition(i*size, j*size);

				//Draw sprite1 but, do not show it on screen.  
				window.draw(sprite1);

			}
		}

		//2nd: Draw in any bullets if they were fired
		for (int i = 0; i < 3; i++) {
			sprite4.setPosition(b1[i].x*size, b1[i].y*size);
			window.draw(sprite4);
		}

		for (int i = 0; i < 3; i++) {
			sprite7.setPosition(b2[i].x*size, b2[i].y*size);
			window.draw(sprite7);
		}


		//3rd: The draw the players onto the screen 

		if (p1.alive) {
			sprite2.setPosition(p1.x*size, p1.y*size);
			window.draw(sprite2);
		}
		else {
			//Draws the dead player sprite if the player is dead
			sprite5.setPosition(p1.x*size, p1.y*size);
			window.draw(sprite5);

		}

		if (p2.alive) {
			sprite3.setPosition(p2.x*size, p2.y*size);
			window.draw(sprite3);
		}
		else {
			//Draws the dead player sprite if the player is dead
			sprite5.setPosition(p2.x*size, p2.y*size);
			window.draw(sprite5);
		}

		//4th: Display the portal
		sprite6.setPosition(portal.x*size, portal.y*size);
		window.draw(sprite6);


		//Show everything we have drawn on the screen	
		window.display();

	}

	return 0;

}
