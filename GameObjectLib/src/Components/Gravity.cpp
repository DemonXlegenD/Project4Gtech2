#include "Components/Gravity.h"

Gravity::Gravity() {
	this->Start();
}
void Gravity::Update(sf::Time _delta) {
	if (isPlaying) {
		GetOwner()->SetPosition((GetOwner()->GetPosition() + Maths::Vector2f::Up));
		GetOwner()->SetPosition((GetOwner()->GetPosition() + Maths::Vector2f::Up));
		GetOwner()->SetPosition((GetOwner()->GetPosition() + Maths::Vector2f::Up));
	}
}

void Gravity::Stop() {
	this->isPlaying = false;
}

void Gravity::Start() {
	this->isPlaying = true;
}