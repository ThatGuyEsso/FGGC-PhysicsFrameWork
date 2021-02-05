#include "Particle.h"
#include "Commons.h"
Particle::Particle(Transform* transform, Appearance* appearance)
	: _transform(transform), _appearance(appearance)
{
	_particleModel = new ParticleModel(_transform, Vector3D(), Vector3D(), true);
	_graphics = new Graphics(_appearance);
}

Particle::~Particle()
{
	_transform = nullptr;
	_appearance = nullptr;
}

void Particle::Update(float deltaTime)
{
	_particleModel->Update(deltaTime);

	_liveTime -= deltaTime-FPS;

}

void Particle::Draw(ID3D11DeviceContext* pImmediateContext)
{
	_graphics->Draw(pImmediateContext);
}
