#include "Component.h"


Component::~Component()
{
    _owner = nullptr;
}

GameObject* Component::GetOwner()
{
    return _owner;
}

void Component::SetOwner(GameObject* newOwner)
{
    _owner = newOwner;
}
