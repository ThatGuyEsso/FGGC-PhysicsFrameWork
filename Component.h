#pragma once
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_impl_dx11.h"
class GameObject;
//Component Interface
class Component
{
public:
	enum class ComponentType
	{
		PhysicModel,
		Transform,
		Appearance,
		Collider,
		Renderer
	};

	virtual GameObject* GetOwner()=0;
	virtual void SetOwner(GameObject* newOwner)=0;
	virtual void UpdateComponent(float deltaTime) = 0;
	virtual ComponentType GetComponentType() { return _type; }
	virtual void DrawGUI() { ; }
	virtual Component* GetClassType()=0;
protected:
	GameObject* _owner;
	ComponentType _type;
	
};

