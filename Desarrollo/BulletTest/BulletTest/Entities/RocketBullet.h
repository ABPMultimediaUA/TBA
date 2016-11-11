#pragma once

#include "../Otros/vec3.hpp"
#include "Entity.h"
#include "EntityManager.h"
#include "../Otros/Time.hpp"
#include "../Otros/Clock.hpp"

class RocketBullet : public Entity
{
public:
	RocketBullet(Vec3<float> position, Vec3<float> direction, Vec3<float> finalposition, Vec3<float> rotation);
	~RocketBullet();

private:
	Vec3<float> m_direction;
	Vec3<float> m_position;
	Vec3<float> m_rotation;
	float m_velocity;
	Time m_lifetime;
	Clock timelifeclock;

	float radioExplosion;





	// Heredado v�a Entity
	virtual void inicializar() override;

	virtual void update(Time elapsedTime) override;

	virtual void handleInput() override;

	virtual void cargarContenido() override;

	virtual void borrarContenido() override;

	virtual void handleMessage(const Message & message) override;

	virtual std::string getClassName() override;

};