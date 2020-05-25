#include "Component.h"

Component::~Component() {}
void Component::Start() {}
void Component::Update() {}
void Component::Destroy() {}

void Component::SetGameObject(std::weak_ptr<GameObject> gameObject)
{
    this->gameObject = gameObject;
}
std::weak_ptr<GameObject> Component::GetGameObject()
{
    return gameObject;
}