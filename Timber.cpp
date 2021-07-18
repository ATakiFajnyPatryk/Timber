#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace sf;
using namespace std;

mt19937_64 gen(chrono::system_clock::now().time_since_epoch().count());
int rd(int a, int b);

void setTextInTheMiddle(Text & text);

void updateBranches(int score);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum side {NONE, LEFT, RIGHT};
side branchPositions[NUM_BRANCHES];

void updateHighscore(int score, string & highscore);

int main()
{
	VideoMode vm(1920, 1080);

	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2[4];
	for (int i = 0; i < 4; i++)
		spriteTree2[i].setTexture(textureTree2);
	spriteTree2[0].setPosition(50, 0);
	spriteTree2[1].setPosition(350, 0);
	spriteTree2[2].setPosition(1350, 0);
	spriteTree2[3].setPosition(1550, 0);
	spriteTree2[0].setScale(0.5f, 1.2f);
	spriteTree2[1].setScale(0.4f, 0.9f);
	spriteTree2[2].setScale(0.45f, 0.9f);
	spriteTree2[3].setScale(0.4f, 0.85f);

	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(-200, -200);
	bool beeActive = false;
	float beeSpeed = 0;

	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	const int NUM_CLOUDS = 3;
	Sprite spriteCloud[NUM_CLOUDS];
	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		spriteCloud[i].setTexture(textureCloud);
		spriteCloud[i].setPosition(-1000, -1000);
	}
	vector<bool> cloudActive(NUM_CLOUDS);
	vector<float> cloudSpeed(NUM_CLOUDS);

	Clock clock;

	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	int score = 0;
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(100);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 20);

	Text messageText;
	messageText.setFont(font);
	messageText.setString("Press Enter to start!");
	messageText.setCharacterSize(75);
	messageText.setFillColor(Color::White);
	setTextInTheMiddle(messageText);

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);
	side playerSide = LEFT;

	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 2000);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(2000, 830);
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	ifstream highscore_file("highscore.txt");
	string highscore;
	if (highscore_file.good())
	{
		highscore_file >> highscore;
		highscore_file.close();
	}
	Text highscoreText;
	highscoreText.setFont(font);
	highscoreText.setCharacterSize(100);
	highscoreText.setFillColor(Color::White);
	highscoreText.setPosition(20, 160);
	highscoreText.setString("Highscore = " + highscore);

	while (window.isOpen())
	{	
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			score = 0;
			scoreText.setString("Score = 0");
			timeRemaining = 6;
			highscoreText.setPosition(20, 1600);

			for (int i = 1; i < NUM_BRANCHES; i++)
				branchPositions[i] = NONE;
			spriteRIP.setPosition(675, 2000);
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}
		if (acceptInput)
		{
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
			{
				playerSide = RIGHT;

				score++;
				stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());
				timeRemaining += (2 / score) + 0.15f;
				timeRemaining = min(timeRemaining, 6.0f);
				
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				updateBranches(score);
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				
				acceptInput = false;

				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
			{
				playerSide = LEFT;

				score++;
				stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());
				timeRemaining += (2 / score) + 0.15;
				timeRemaining = min(timeRemaining, 6.0f);

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				updateBranches(score);
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				
				acceptInput = false;

				chop.play();
			}
		}

		if (!paused)
		{
			Time dt = clock.restart();
			
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f)
			{
				paused = true;
				acceptInput = false;
				messageText.setString("Out of time!!");
				setTextInTheMiddle(messageText);
				outOfTime.play();
				updateHighscore(score, highscore);
				highscoreText.setString("Highscore = " + highscore);
				highscoreText.setPosition(20, 160);
			}

			if (!beeActive)
			{
				beeSpeed = rd(200, 400);
				float height = rd(500, 1000);
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100)
					beeActive = false;
			}

			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudActive[i])
				{
					cloudSpeed[i] = rd(5, 200);
					float height = rd(-150, 300);
					spriteCloud[i].setPosition(-200, height);
					cloudActive[i] = true;
				}
				else
				{
					spriteCloud[i].setPosition(spriteCloud[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()), spriteCloud[i].getPosition().y);
					if (spriteCloud[i].getPosition().x > 1920)
						cloudActive[i] = false;
				}
			}

			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == LEFT)
				{
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == RIGHT)
				{
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else
					branches[i].setPosition(3000, height);
			}

			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y +  (logSpeedY * dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				if (playerSide == LEFT)
					spriteRIP.setPosition(525, 760);
				else
					spriteRIP.setPosition(1225, 760);
				spritePlayer.setPosition(2000, 660);
				messageText.setString("SQUISHED!!");
				setTextInTheMiddle(messageText);
				death.play();
				updateHighscore(score, highscore);
				highscoreText.setString("Highscore = " + highscore);
				highscoreText.setPosition(20, 160);
			}
		}

		window.clear();

		window.draw(spriteBackground);
		for (int i = 0; i < NUM_CLOUDS; i++)
			window.draw(spriteCloud[i]);
		for (int i = 0; i < 4; i++)
			window.draw(spriteTree2[i]);
		for (int i = 0; i < NUM_BRANCHES; i++)
			window.draw(branches[i]);
		window.draw(spriteTree);
		
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);
		
		window.draw(scoreText);
		window.draw(highscoreText);
		window.draw(timeBar);
		if (paused)
			window.draw(messageText);

		window.display();

	}
	return 0;
}

int rd(int a, int b)
{
    return uniform_int_distribution<int>(a, b)(gen);
}

void setTextInTheMiddle(Text & text)
{
	FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(1920 / 2.0f, 1080 / 2.0f);
}

void updateBranches(int score)
{
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];

	int r = rd(0, max(250, 400 - score));
	if (r < 100)
		branchPositions[0] = LEFT;
	else if (r < 200)
		branchPositions[0] = RIGHT;
	else
		branchPositions[0] = NONE;

}

void updateHighscore(int score, string & highscore)
{
	stringstream ss;
	ss << score;
	if (ss.str().length() > highscore.length() || (ss.str().length() == highscore.length() && ss.str() > highscore))
		highscore = ss.str();
	ofstream highscore_file("highscore.txt", ios::out | ios::trunc);
	highscore_file << highscore;
	highscore_file.close();
}

// clang++ -std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wno-sign-conversion -Wfloat-equal -fsanitize=address,undefined -D_GLIBCXX_DEBUG -DDEBUG -ggdb Timber.cpp -o Timber -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio