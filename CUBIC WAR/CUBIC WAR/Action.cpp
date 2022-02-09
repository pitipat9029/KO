#include "Action.h"

Action::Action(sf::RenderWindow* pWindow, sf::Vector2f* pMousePosView)
{
	this->pWindow = pWindow;
	this->pMousePosView = pMousePosView;
}

Action::~Action()
{
	GameEnd();
}

void Action::StartGame(int numPlayer)
{
	this->pMap = new Map;

	for (int i = 0; i < numPlayer; i++) {
		this->vPlayers.push_back(new Player);
	}
	countTurn = 0;
	RandomStartPosition();
	this->NextTurn();
}

void Action::GameEnd()
{
	for (int i = 0; i < this->vPlayers.size(); i++) {
		delete this->vPlayers[i];
	}
	delete this->pMap;
}

void Action::Render()
{
	if (isWait == false) {
		this->pMap->Render(this->pWindow);
		ShowGridHighlight();
		this->RenderUnit();
	}
	this->Update();
}

void Action::ClickEvents()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		if (!this->isMousePress) {
			// Mouse Left click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (this->isWait) {
					this->isWait = false;
				}
				else {
					if (pGridPointed != 0) {
						if (this->isMoveMode) {
							this->Move();
						}
						else {
							if (pGridPointed->isUnit && pGridPointed->GetUnit() != 0) {
								this->pGridMoveStart = pGridPointed;
								this->CreateGridArea(2);
								this->isMoveMode = true;
							}
							else {

							}
						}
					}
				}
			}
			// Mouse Right click
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				std::cout << " => End Turn" << std::endl;
				NextTurn();
			}
			this->isMousePress = true;
		}
	}
	else {
		this->isMousePress = false;
	}
}

void Action::NextTurn()
{
	this->isWait = true;
	if (this->countTurn == 0) {
		idPlayerNow = 0;
		this->countTurn++;
	}
	else if (this->idPlayerNow < this->vPlayers.size() - 1) {
		this->idPlayerNow++;
	}
	else {
		this->idPlayerNow = 0;
		this->countTurn++;
	}
	std::cout << "Turn: " << countTurn;
	std::cout << " ----> Player " << idPlayerNow + 1 << " is playing";
}

void Action::Update()
{
	this->currentMousePos = sf::Mouse::getPosition(*this->pWindow);
	this->pGridPointed = this->CheckGridPointed();

	ClickEvents();
}

Grid* Action::CheckGridPointed()
{
	std::vector<Grid*> gridHovered;
	for (unsigned int i = 0; i < this->pMap->vGrids.size(); i++) {
		for (unsigned int j = 0; j < this->pMap->vGrids[i].size(); j++) {
			if (pMap->vGrids[i][j].isHovered(currentMousePos))
			{
				gridHovered.push_back(&pMap->vGrids[i][j]);
			}
		}
	}

	if (gridHovered.size() > 0) {
		for (unsigned int i = 0; i < gridHovered.size() - 1; i++) {
			bool isTemp = 0;
			for (unsigned int j = 0; j < gridHovered.size() - i - 1; j++) {
				float distanceOne = gridHovered[j]->distanceFromMouse(this->currentMousePos);
				float distanceTwo = gridHovered[j + 1]->distanceFromMouse(this->currentMousePos);
				if (distanceOne > distanceTwo){
					std::swap(gridHovered[j + 1], gridHovered[j]);
					isTemp = 1;
				}
			}
			if (!isTemp) break;
		}
		if(gridHovered[0]->isEnabled()) return gridHovered[0];
	}
	return 0;
}

void Action::CreateGridArea(int radius)
{
	this->pMap->SetGridAllEnable(false);
	for (int q = this->pGridMoveStart->GetRC().y - radius; q <= this->pGridMoveStart->GetRC().y + radius; q++) {
		for (int r = this->pGridMoveStart->GetRC().x - radius; r <= this->pGridMoveStart->GetRC().x + radius; r++) {
			if (abs(-(q - this->pGridMoveStart->GetRC().y) - (r - this->pGridMoveStart->GetRC().x)) <= radius) {
				int col = q + (r - (r & 1)) / 2;
				int row = r;
				if (row >= 0 && row < 11 && col >= 0 && col < 15) {
					Grid* pGrid = &this->pMap->vGrids[row + 1][col + 1];
					if (pGrid != this->pGridMoveStart && !pGrid->isBuilding && !pGrid->isUnit) {
						pGrid->SetEnabled(true);
					}
				}
			}
		}
	}
	this->pMap->SetGridEdgeDisable();
}

void Action::HighlightGridPointed(sf::Color cFill, sf::Color cOutline)
{
	sf::CircleShape rect(35, 6);
	rect.setPosition(pGridPointed->GetPosition().x - 5, pGridPointed->GetPosition().y);
	rect.setFillColor(cFill);
	rect.setOutlineColor(cOutline);
	rect.setOutlineThickness(-1.5f);
	this->pWindow->draw(rect);
}

void Action::HighlightGridArea(sf::Color cFill, sf::Color cOutline)
{
	sf::CircleShape rect(35, 6);
	rect.setFillColor(cFill);
	for (unsigned int i = 0; i < this->pMap->vGrids.size(); i++) {
		for (unsigned int j = 0; j < this->pMap->vGrids[i].size(); j++) {
			if (this->pMap->vGrids[i][j].isEnabled())
			{
				rect.setPosition(this->pMap->vGrids[i][j].GetPosition().x - 5, this->pMap->vGrids[i][j].GetPosition().y);
				this->pWindow->draw(rect);
			}
		}
	}

}

void Action::ShowGridHighlight()
{
	if (this->isMoveMode) {
		this->HighlightGridArea(sf::Color(0, 255, 0, 100), sf::Color(0, 0, 0, 0));
		if (this->pGridPointed != 0) this->HighlightGridPointed(sf::Color(0, 255, 0, 100), sf::Color(0, 0, 0, 0));
	}
	else {
		if (this->pGridPointed != 0) this->HighlightGridPointed(sf::Color(0, 0, 0, 0), sf::Color::White);
	}
}

void Action::RandomStartPosition()
{
	this->pMap->vGrids[5][3].CreateBuilding("B", 0);
	this->pMap->vGrids[6][3].AddUnit("Engineer", this->pMap->vUnits, 0);
	this->pMap->vGrids[8][12].CreateBuilding("B", 1);
	this->pMap->vGrids[7][13].AddUnit("Engineer", this->pMap->vUnits, 1);
}

void Action::RenderUnit()
{
	for (unsigned int i = 0; i < this->pMap->vUnits.size(); i++) {
		this->pMap->vUnits[i]->Render(this->pWindow);
	}
}

void Action::Move()
{
	this->pGridPointed->AddUnit(this->pGridMoveStart->GetUnit());
	this->pGridMoveStart->ClearUnit();
	this->pGridMoveStart = 0;
	this->isMoveMode = false;
}
