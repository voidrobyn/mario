#include "GameScreenLevel1.h"
#include <iostream>
#include "Texture2D.h"
#include "Character.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include <string>
#include <fstream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer) {
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() {
	m_background_texture = nullptr;
	delete my_character;
	my_character = nullptr;
	m_level_map = nullptr;
	m_pow_block = nullptr;

	m_enemies.clear();
}

void GameScreenLevel1::Render() {
	//draw background
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
	my_character->Render();
	//luigi->Render();
	m_pow_block->Render();
	for (int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->Render();
	}
}

bool GameScreenLevel1::SetUpLevel() {
	SetLevelMap("Maps/Level1.txt");

	my_character = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(40, 330),m_level_map);
	//luigi = new Character(m_renderer, "Images/Mario.png", Vector2D(150, 330));
	m_pow_block = new PowBlock(m_renderer, m_level_map);
	m_screenshake = false;
	m_background_yPos = 0.0f;
	CreateKoopa(Vector2D(150, 32), FACING_RIGHT, 0.02f);
	CreateKoopa(Vector2D(325, 32), FACING_LEFT, 0.02f);
	//Vector2D position, FACING direction, float speed

	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png")) {
		std::cout << "Failed to load background texture! " << std::endl;
		return false;
	}
	return true;
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
	//screen shake
	if (m_screenshake) {
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;
		if (m_shake_time <= 0.0f) {
			m_shake_time = false;
			m_background_yPos = 0.0f;
		}
	}

	//update character
	my_character->Update(deltaTime, e);
	UpdateEnemies(deltaTime, e);
	UpdatePOWBlock();
}

void GameScreenLevel1::SetLevelMap(std::string path) {
	std::ifstream inFile;
	inFile.open(path);
	int map[MAP_HEIGHT][MAP_WIDTH];

	for (int i = 0; i < MAP_HEIGHT; i++){
		for (int j = 0; j < MAP_WIDTH; j++) {
			inFile >> map[i][j];
			std::cout << map[i][j];
		}
		
	}
	
	//clear any old maps
	if (m_level_map != nullptr) {
		delete m_level_map;
	}
	//set new one
	m_level_map = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock() {
	if (Collisions::Instance()->Box(my_character->GetCollisionBox(), m_pow_block->GetCollisionsBox())) {
		if (m_pow_block->IsAvailable()) {
			if (my_character->Is_Jumping()) {
				DoScreenshake();
				m_pow_block->TakeHit();
				my_character->CancelJump();
			}
		}
	}
}

void GameScreenLevel1::DoScreenshake() {
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->TakeDamage();
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e){
	//need t  add mario hit code myself 

	if (!m_enemies.empty()){
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++){
			//check if the enemy is on bottom row of tiles
			if (m_enemies[i]->GetPosition().y > 300.0f){
				//check if enemy is left/right
				if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) || m_enemies[
					i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i]->GetCollisionBox().width * 0.55f))
					m_enemies[i]->SetAlive(false);
			}

			m_enemies[i]->Update(deltaTime, e);

			//check if enemy collides with player
			if ((m_enemies[i]->GetPosition().y > 300.0f || m_enemies[i]->GetPosition().y <= 64.0f) && (m_enemies[i]->
				GetPosition().x < 64.0f || m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f)){
				//ignore collisions if behind pipe
			}
			else{
				if (Collisions::Instance()->Circle(m_enemies[i], my_character)){
					if (m_enemies[i]->GetInjured()){
						m_enemies[i]->SetAlive(false);
					}
					else{
						my_character->SetAlive(false);
					}
				}
			}

			if (!m_enemies[i]->GetAlive()){
				enemyIndexToDelete = i; //schedule dead enemies deletion
			}

			//if (Collisions::Instance()->Box(my_character->GetCollisionBox(), m_enemies[i]->GetCollisionBox())) {
			//	//if (my_character->Is_Jumping()) {
			//		m_enemies[i]->TakeDamage();
			//	//}
			//}
		}

		if (enemyIndexToDelete != -1){
			m_enemies.erase(m_enemies.begin() + enemyIndexToDelete); // remove dead enemies
		}
		 
	}

}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed){
	koopa = new CharacterKoopa(m_renderer, "Images/Koopa.png", position, m_level_map, direction, speed);
	m_enemies.push_back(koopa);
}
