#pragma once
#include <vector>
#include "GameObject.h"
#include "SceneManager.h"
#include "Components/Ressource.h"

class CreateTours
{
public:
	CreateTours();
	static void CreateJin(float _poisionX, float _positionY);
	static void CreateLulu (float _poisionX, float _positionY);
	static void CreateMalphite(float _poisionX, float _positionY);
	static void CreateXinZhao(float _poisionX, float _positionY);
	static void CreateBat2(float _poisionX, float _positionY);
	static void CreateBat3(float _poisionX, float _positionY);

private:
	static std::vector<GameObject*> tower;
	static sf::Texture* texture;
};
