#pragma 
#include "ParticleModel.h"
#include "Appearance.h"
#include "Transform.h"
#include "Graphics.h"
class Particle {
private:
	Transform* _transform;
	Appearance* _appearance;
	ParticleModel* _particleModel;
	Graphics* _graphics;
	float _liveTime;

public:
	Particle(Transform* transform, Appearance* appearance);
	~Particle();

	void Update(float deltaTime);
	void Draw(ID3D11DeviceContext* pImmediateContext);

	void SetLiveTime(float liveTime) { _liveTime = liveTime; }

	//Getters
	Transform* GetTransform() { return _transform; }
	Appearance* GetAppearance() { return _appearance; }
	ParticleModel* GetParticleModel() { return _particleModel; }
	float GetLiveTIme() { return _liveTime; }
	bool GetIsAlive() { return _liveTime > 0.0f; }

};