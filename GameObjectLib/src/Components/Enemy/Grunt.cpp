#include "Components/Enemy/Grunt.h"
#include "AssetManager.h"

std::map<std::string, sf::Texture> AssetManager::assets;

Grunt::Grunt() : EnemyAbstract(200, 10, 20) {};

void Grunt::setDirection(Direction newDirection) {
    direction = newDirection;

}
void Grunt::MouvementGrunt() {
    srand(time(nullptr));
    //int rand_attack = rand() % 3 + 1;
    int rand_mouve = rand() % 2 + 1;

    if (rand_mouve == 1) {
        GetOwner()->GetComponent<Sprite>()->GruntPlayAnimation();

        /*grunt.SetPosition(grunt.SetPosition()) */
            /*if (rand_attack == 1) {
                enemy.loadFromFile("Sprite/enemy/grunt_shoot.png");
            }*/
    }
    else if (rand_mouve == 2) {
        GetOwner()->SetPosition(GetOwner()->GetPosition() + Maths::Vector2f::Right + Maths::Vector2f(3, 0));
        GetOwner()->GetComponent<Grunt>()->setDirection(Grunt::Direction::Right);
        GetOwner()->GetComponent<Sprite>()->GruntPlayAnimationRun();
    }
    else if (hp == 0) {
        //enemy.loadFromFile("Sprite/enemy/grunt_death.png");
    }
}