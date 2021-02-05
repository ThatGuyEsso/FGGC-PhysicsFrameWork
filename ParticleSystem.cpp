#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3D origin)
	:_origin(origin)
{
}

ParticleSystem::~ParticleSystem()
{
	if (!_particles.empty()) {
		for (Particle* particle : _particles) {
			particle = nullptr;
		}
	}
	_particles.clear();
}

void ParticleSystem::CreateParticleSystem(int nParticles, Transform* initTransform,
	float liveTime, Vector3D initVelocity, Vector3D initAccel, Appearance* appearance)
{
	_nParticles = nParticles;

	for (int i = 0; i < _nParticles; i++) {
		Particle* particle = new Particle(initTransform, appearance);
		particle->GetTransform()->SetPosition(_origin);
		particle->SetLiveTime(liveTime);
		particle->GetParticleModel()->SetCurrentVelocity(initVelocity);
		particle->GetParticleModel()->SetCurrentAcceleration(initAccel);

	}
	_isStopped = false;
}

void ParticleSystem::RunParticleSystem(float deltaTime, ID3D11DeviceContext* pImmediateContext)
{
	if (!_particles.empty() && !_isStopped) {

		for (Particle* particle : _particles) {
			if (particle->GetLiveTIme() > 0) {

				for (Vector3D force : _particleForces) {
					particle->GetParticleModel()->ApplyForce(force);
				}
				
				Update(deltaTime, particle);
				Draw(pImmediateContext, particle);
			}
			else {
				
			}
		}
	}
}

void ParticleSystem::Update(float deltaTime, Particle* particle)
{
	
		particle->Update(deltaTime);
	
}

void ParticleSystem::Draw(ID3D11DeviceContext* pImmediateContext, Particle* particle)
{
	particle->Draw(pImmediateContext);

}
