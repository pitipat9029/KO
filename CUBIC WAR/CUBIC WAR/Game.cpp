#include "Game.h"

Game::Game()
{
	this->initWindow();
	this->initVariable();
	this->initFont();
	this->initText();
	this->initButton();

	this->initMap();
}

Game::~Game()
{
	delete window;
	delete map;
	delete building;
}

const bool Game::IsRuning() const
{
	return this->window->isOpen();
}

// Seting Game start

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Game", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Game::initVariable()
{
	this->map = new Map;
	this->gameAction = new Action(this->map, this->window, this->map->gridInfos, this->gamePlayers, &this->mousePosView);
}

void Game::initPlayer(int numbers)
{
	Player player;
	for (int i = 0; i < numbers; i++) {
		gamePlayers.push_back(player);
	}
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->event)) {

		switch (this->event.type)  
		{
		case sf::Event::Closed:
			this->window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape) this->window->close();
			break;
		}
		if (button.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
		{
			button.setFillColor(sf::Color::Blue);
		}
		else
		{
			button.setFillColor(sf::Color::Red);
		}

		//button press
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (button.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
			{
				std::cout << "Start Your turn\n";
				roll1 = (rand() % 6) + 1;
				roll2 = (rand() % 6) + 1;

				points = roll1 + roll2;
				std::cout << "dice1 = " << roll1 << "\n";
				std::cout << "dice2 = " << roll2 << "\n";
				std::cout << "Your total points is " << points << "\n";

				if (roll1 == roll2)
				{
					std::cout << "U can roll again! \n";
				}
			}
		}

	}
}

void Game::initVariable()
{
	this->points = 0;

}

void Game::initFont()
{
	this->font.loadFromFile("Fonts/Minecraft.ttf");
}

void Game::initText()
{
	this->guiText.setFont(this->font);
	this->guiText.setFillColor(sf::Color::Blue);
	this->guiText.setCharacterSize(32);
}


void Game::UpdateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::Update()
{
	this->pollEvents();
	this->UpdateMousePosition();
}

void Game::updateGui()
{
	std::stringstream ss;

	ss << "Points: " << this->points;
	this->guiText.setString(ss.str());
}
 
void Game::Render()
{
	this->window->clear();

	//--------------------------

	//Code draw things here
	this->player.render(this->window);
	this->map->Render(*this->window);
	this->gameAction->Update();

	//--------------------------

	//Render gui
	//this->RenderGui(this->window);
	this->window->display();
}

Game::Game()
{
	this->initWindow();
	this->initPlayer(2);
	this->initVariable();
}
void Game::renderButton(sf::RenderTarget* target)
{
	target->draw(this->button);
}

Game::~Game()
{
	delete window;
	delete map;
	delete gameAction;
}
