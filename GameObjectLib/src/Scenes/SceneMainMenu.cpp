#include "Scenes/SceneMainMenu.h"
#include "WindowManager.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "AudioManager.h"
//#include "AudioManager.h"
#include "WindowManager.h"

SceneMainMenu::SceneMainMenu(sf::RenderWindow* _window) : Scene(_window) {
	texture = nullptr;
	textureBullet = nullptr;
}

void SceneMainMenu::Create() {
	Scene::Create();
	sf::Texture backgroundTexture1;
	if (!backgroundTexture1.loadFromFile("../assets/Sprite_LOL/background/background_lol.png"))
	{
		std::cout << "pas d'image" << std::endl;
	}

	GameObject* background1 = CreateBackgroundGameObject("Background1", WindowManager::GetWindowWidth() / 2, WindowManager::GetWindowHeight() / 2, backgroundTexture1);
	this->CreateSceneButtonsMenu();
	this->activeOption(false);
	this->activeMenu(true);

}
void SceneMainMenu::Delete() {
	delete texture;
	delete textureBullet;
	Scene::Delete();
}

void SceneMainMenu::Render(sf::RenderWindow* _window) {
	Scene::Render(_window);
}

void SceneMainMenu::CreateSceneButtonsMenu () {
	float widthScreen = WindowManager::GetWindow()->getSize().x;
	float heightScreen = WindowManager::GetWindow()->getSize().y;
	playButton = CreateButtonGameObject("Play", widthScreen / 2, heightScreen / 3, 50);
	optionsButton = CreateButtonGameObject("Options", widthScreen / 2, heightScreen / 2, 20);
	quitButton = CreateButtonGameObject("Quit", widthScreen / 2, heightScreen / 1.5, 50);
	successButton = CreateButtonGameObject("Success", widthScreen / 1.2, heightScreen / 10, 25);
	rankButton = CreateButtonGameObject("Rank", widthScreen / 1.3, heightScreen / 10, 25);
	creditsButton = CreateButtonGameObject("Cr�dits", widthScreen / 1.1, heightScreen / 10, 25);
	backButton = CreateButtonGameObject("Back", widthScreen / 10, heightScreen / 10, 20);
	sliderFPS = CreateSliderGameObject("SliderFPS", widthScreen / 2, heightScreen / 2, 1200, 40, 50, 50, 20, WindowManager::GetFps(), WindowManager::GetMinFps(), WindowManager::GetMaxFps());
	//sliderVolume = CreateSliderGameObject("SliderVolume", widthScreen / 2, heightScreen / 1.5, 1200, 40, 50, 50, 20, AudioManager::GetVolume(), AudioManager::GetMaxVolume());
}

void SceneMainMenu::Update(sf::Time _delta) {
	Scene::Update(_delta);
	if (playButton->GetComponent<Button>()->IsClicked()) {
		SceneManager::RunScene("SceneGameLVSR");
	}
	else if (optionsButton->GetComponent<Button>()->IsClicked() && optionsButton->GetActive()) {
		this->activeMenu(false);
		this->activeOption(true);
	}
	else if (quitButton->GetComponent<Button>()->IsClicked() && quitButton->GetActive()) {
		WindowManager::GetWindow()->close();
	}
	else if (backButton->GetComponent<Button>()->IsClicked() && backButton->GetActive()) {
		this->activeOption(false);
		this->activeMenu(true);
	}
	else if (successButton->GetComponent<Button>()->IsClicked() && successButton->GetActive()) {
		SceneManager::RunScene("SceneSuccessMenu");
	}
	else if (rankButton->GetComponent<Button>()->IsClicked() && rankButton->GetActive()) {
		SceneManager::RunScene("SceneRankMenu");
	}
	else if (creditsButton->GetComponent<Button>()->IsClicked() && creditsButton->GetActive()) {
		SceneManager::RunScene("SceneCreditsMenu");
	}
	else if (sliderFPS) 
	{
		WindowManager::SetFps(sliderFPS->GetComponent<Slider>()->GetDataInt());
	}
	else if (sliderVolume)
	{
		//AudioManager::SetVolume(sliderVolume->GetComponent<Slider>()->GetDataInt());
	}
}

void SceneMainMenu::activeMenu(bool _state) {
	this->playButton->SetActive(_state);
	this->optionsButton->SetActive(_state);
	this->quitButton->SetActive(_state);
}

void SceneMainMenu::activeOption(bool _state) {
	this->backButton->SetActive(_state);
	this->sliderFPS->SetActive(_state);
	//this->sliderVolume->SetActive(_state);
}

SceneMainMenu::~SceneMainMenu() {
	this->Delete();
}