#include "Scenes/SceneGameAbstract.h"

#include "SceneManager.h"
#include "AssetManager.h"

#include "Components/Button.h"
#include "Components/SquareCollider.h"
#include "Components/SpriteRenderer.h"
#include "Components/Gravity.h"
#include "Components/Platforme.h"
#include "Components/Enemy/Grunt.h"
#include "Components/Enemy/Turret.h"
#include "Components/FireBullet.h"
#include "Components/Armes.h"

SceneGameAbstract::SceneGameAbstract(sf::RenderWindow* _window) : Scene(_window) {
	this->Awake();
	Scene::Create();
	srand(time(nullptr));
}

void SceneGameAbstract::Create() {
	Scene::Create();
	CreateBackground();
	this->CreatePauseMenuButtons();
	gamePause = false;
	escapeIsPress = false;
}

void SceneGameAbstract::CreatePauseMenuButtons() {
	float widthScreen = SceneManager::GetWindow()->getSize().x;
	float heightScreen = SceneManager::GetWindow()->getSize().y;
	pausePlayButton = CreateButtonGameObject("Continue", widthScreen / 2, heightScreen / 4.0, 50);
	pauseMenuPrincipalButton = CreateButtonGameObject("Menu Principal", widthScreen / 2, heightScreen / 2.5, 50);
	pauseOptionsButton = CreateButtonGameObject("Options", widthScreen / 2, heightScreen / 1.8, 50);
	pauseQuitButton = CreateButtonGameObject("Quit", widthScreen / 2, heightScreen / 1.4, 50);
	this->ManageMenuPause(false);
}

void SceneGameAbstract::Awake() {
	Scene::Awake();
}

void SceneGameAbstract::CreatePlayer() {
	player = this->CreateCharacterGameObject("Player", 400.f, 400.f, AssetManager::GetAsset("Player0"), 2.5f, 2.5f);
}

void SceneGameAbstract::CreateGrunt()
{
	if (!platforms.empty()) {
		int random = rand() % platforms.size();
		if (random != 0) {
			GameObject* rand_platform = platforms[random];
			int rand_grunt = rand() % 700 + 100;
			enemies.push_back(CreateGruntGameObject("Grunt1", rand_grunt, 100.f, 2.5f, 2.5f, AssetManager::GetAsset("Grunt0")));
		}

	}


}

void SceneGameAbstract::Collision(GameObject* _entity)
{
	bool colliding = false;
	for (GameObject* platform : this->platforms) {
		if (SquareCollider::IsColliding(*(_entity->GetComponent<SquareCollider>()), *(platform->GetComponent<SquareCollider>()))) {
			colliding = true;
			break;
		}
	}

	if (colliding)
	{
		_entity->GetComponent<Gravity>()->Stop();
	}
	else
	{
		_entity->GetComponent<Gravity>()->Start();
	}
}

void SceneGameAbstract::ManageMenuPause(bool _state) {
	escapeIsPress = _state;
	gamePause = _state;
	this->pauseMenuPrincipalButton->SetActive(_state);
	this->pausePlayButton->SetActive(_state);
	this->pauseOptionsButton->SetActive(_state);
	this->pauseQuitButton->SetActive(_state);
}

void SceneGameAbstract::ManageSceneGameButtonsPause()
{
	Command* pauseInput = inputHandlerPlayer->PauseInput();
	if (pauseInput && !escapeIsPress) {
		pauseInput->Execute();
		this->player->SetActive(false);
		this->ManageMenuPause(true);
		for (GameObject* enemy : this->enemies) {
			enemy->SetActive(false);
		}
		for (GameObject* platform : this->platforms) {
			platform->SetActive(false);
		}
	}
	else if (pauseInput && escapeIsPress) {
		pauseInput->Execute();
		this->player->SetActive(true);
		this->ManageMenuPause(false);
		for (GameObject* enemy : this->enemies) {
			enemy->SetActive(true);
		}
		for (GameObject* platform : this->platforms) {
			platform->SetActive(true);
		}
	}
}

void SceneGameAbstract::Update(sf::Time _delta) {

	SceneGameAbstract::ManageSceneGameButtonsPause();

	if (!gamePause)
	{
		Scene::Update(_delta);
		this->Collision(this->player);
		this->player->GetComponent<Sprite>()->PlayerPlayAnimation();
		for (GameObject* enemy : this->enemies) {
			this->Collision(enemy);
			enemy->GetComponent<Sprite>()->GruntPlayAnimation();
		}

		if (!this->player->GetComponent<Player>()->directionPlayer)
		{
			this->player->GetComponent<Sprite>()->SetScale(2.5f, 2.5f);
		}
		else
		{
			this->player->GetComponent<Sprite>()->SetScale(-2.5f, 2.5f);
		}
	}
	else
	{
		if (pausePlayButton->GetComponent<Button>()->IsClicked()) {
			this->player->SetActive(true);
			this->ManageMenuPause(false);
			for (GameObject* enemy : this->enemies) {
				enemy->SetActive(true);
			}
			for (GameObject* platform : platforms) {
				platform->SetActive(true);
			}
		}
		if (pauseMenuPrincipalButton->GetComponent<Button>()->IsClicked()) {
			SceneManager::RunScene("SceneMainMenu");
		}
		if (pauseOptionsButton->GetComponent<Button>()->IsClicked()) {
			std::cout << "Options" << std::endl;
		}
		if (pauseQuitButton->GetComponent<Button>()->IsClicked()) {
			SceneManager::GetWindow()->close();
		}
	}
}

void SceneGameAbstract::CreatePlatform(sf::Texture textureplatforme) {
}

void SceneGameAbstract::CreateBackground()
{
	sf::Texture backgroundTexture1;

	if (!backgroundTexture1.loadFromFile("../assets/Sprite/background/background_color.png"))
	{
		std::cout << "pas d'image" << std::endl;
	}
	GameObject* background1 = CreateBackgroundGameObject("Background1", SceneManager::GetWindowWidth() / 2, SceneManager::GetWindowHeight() / 2, backgroundTexture1);
};

void SceneGameAbstract::Render(sf::RenderWindow* _window) {
	Scene::Render(_window);
}


GameObject* SceneGameAbstract::CreateCharacterGameObject(const std::string& name, float positionx, float positiony, const sf::Texture texture, float scalex, float scaley)
{
	GameObject* gameObject = CreateGameObject(name);
	gameObject->SetPosition(Maths::Vector2f(positionx, positiony));


	Player* playerController = gameObject->CreateComponent<Player>();

	Armes* arme = gameObject->CreateComponent<Armes>();

	Sprite* sprite = gameObject->CreateComponent<Sprite>();
	sprite->SetTexture(texture);
	sprite->SetScale(scalex, scaley);
	sprite->SetSprite();

	SquareCollider* squareCollider = gameObject->CreateComponent<SquareCollider>();
	squareCollider->SetSize(sprite->GetBounds().x, sprite->GetBounds().y);
	squareCollider->SetScale(scalex, scaley);

	Gravity* gravity = gameObject->CreateComponent<Gravity>();

	InputPlayer* inputPlayer = gameObject->CreateComponent<InputPlayer>();
	inputHandlerPlayer = inputPlayer;

	return gameObject;
}

//ENEMY
GameObject* SceneGameAbstract::CreateGruntGameObject(const std::string& name, float positionx, float positiony, float scalex, float scaley, sf::Texture _texture)
{
	GameObject* gameObject = CreateGameObject(name);
	gameObject->SetPosition(Maths::Vector2f(positionx, positiony));

	Sprite* sprite = gameObject->CreateComponent<Sprite>();
	sprite->SetTexture(_texture);
	sprite->SetScale(scalex, scaley);
	sprite->SetSprite();

	SquareCollider* squareCollider = gameObject->CreateComponent<SquareCollider>();
	squareCollider->SetSize(sprite->GetBounds().x, sprite->GetBounds().y);
	squareCollider->SetScale(scalex, scaley);

	Gravity* gravity = gameObject->CreateComponent<Gravity>();


	return gameObject;

}

GameObject* SceneGameAbstract::CreateTurretGameObject(const std::string& name, float positionx, float positiony, float scalex, float scaley, sf::Texture _texture)
{
	GameObject* gameObject = CreateGameObject(name);
	gameObject->SetPosition(Maths::Vector2f(positionx, positiony));

	Sprite* sprite = gameObject->CreateComponent<Sprite>();
	sprite->SetTexture(_texture);
	sprite->SetScale(scalex, scaley);
	sprite->SetSprite();

	SquareCollider* squareCollider = gameObject->CreateComponent<SquareCollider>();
	squareCollider->SetSize(sprite->GetBounds().x, sprite->GetBounds().y);
	squareCollider->SetScale(scalex, scaley);

	Gravity* gravity = gameObject->CreateComponent<Gravity>();

	return gameObject;

}

GameObject* SceneGameAbstract::CreateBulletGameObject(const std::string& name, const sf::Texture textureBullet, float scalex, float scaley, GameObject* _player)
{
	GameObject* gameObject = CreateGameObject(name);
	gameObject->SetPosition(Maths::Vector2f(_player->GetPosition().GetX(), _player->GetPosition().GetY()));

	Sprite* sprite = gameObject->CreateComponent<Sprite>();
	sprite->SetTexture(textureBullet);
	sprite->SetScale(scalex, scaley);
	sprite->SetSprite();

	SquareCollider* squareCollider = gameObject->CreateComponent<SquareCollider>();
	squareCollider->SetSize(sprite->GetBounds().x, sprite->GetBounds().y);
	squareCollider->SetScale(scalex, scaley);

	FireBullet* fireBullet = gameObject->CreateComponent<FireBullet>();
	fireBullet->setDirection(_player);

	std::cout << gameObject << std::endl;
	return gameObject;

}