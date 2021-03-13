#pragma once

class GameObject;
//Component Interface
class Component
{
public:

	virtual GameObject* GetOwner()=0;
	virtual void SetOwner(GameObject* newOwner)=0;
protected:
	GameObject* _owner;
};

