#pragma once
#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "Commons.h"
#include "Collisions.h"
#include <vector>
#include "CharacterKoopa.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "Coin.h"

class Texture2D; //forward declaration
class Character;
class LevelMap;
class PowBlock;

class GameScreenLevel1 : public GameScreen{
	private:
		Texture2D* m_background_texture;
		CharacterMario* mario;
		CharacterLuigi* luigi;
		CharacterKoopa* koopa;
		Coin* coin;
		LevelMap* m_level_map;
		PowBlock* m_pow_block;
		bool m_screenshake;
		float m_shake_time;
		float m_wobble;
		float m_background_yPos;
		std::vector<CharacterKoopa*> m_enemies;
		std::vector<Coin*> m_coins;
		int spawnerFrameCount;

		bool SetUpLevel();
		void SetLevelMap();
		void UpdatePOWBlock();
		void DoScreenshake();
		void UpdateEnemies(float deltaTime, SDL_Event e);
		void UpdateCoins(float deltaTime, SDL_Event e);
		void CreateKoopa(Vector2D position, FACING direction, float speed);
		void CreateCoin(Vector2D position, FACING direction, float speed);
		void EnemySpawner();
		void CoinSpawner();

	public:
		GameScreenLevel1(SDL_Renderer* renderer);
		~GameScreenLevel1();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;	
};

#endif