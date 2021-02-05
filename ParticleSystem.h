#pragma once
#include "Particle.h"
#include <vector>
class ParticleSystem {

private:
	int _nParticles;
	bool _isStopped=true;
	std::vector<Particle*> _particles;

	Vector3D _origin;

	std::vector<Vector3D> _particleForces;
public:
	ParticleSystem(Vector3D origin);
	~ParticleSystem();
	
	void CreateParticleSystem(int nParticles, Transform* initTransform, float liveTime,
		Vector3D initVelocity, Vector3D initAccel, Appearance* appearance);
	void RunParticleSystem(float deltaTime, ID3D11DeviceContext* pImmediateContext);
	void Update(float deltaTime, Particle* particle);
	void Draw(ID3D11DeviceContext* pImmediateContext, Particle* particle);

	void SetSystemOriging(Vector3D origin) { _origin = origin; }

	void ApplyParticleForce(Vector3D newForce) { _particleForces.push_back(newForce); }//adds a new force to all particles in system

};