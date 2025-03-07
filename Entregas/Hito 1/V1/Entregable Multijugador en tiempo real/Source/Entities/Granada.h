#pragma once
#include "irrlicht.h"
#include "Entity.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Estructuras.h>
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

#define GRANADACARGADA 0
#define GRANADADISPARADA 1


class Granada :
	public Entity
{
public:
	Granada();
	~Granada();

	void setRigidBody(btRigidBody* rigidBody) { m_rigidBody = rigidBody; }

	int getEstado() const{ return estado;}
	void setEstado(int newEstado) { estado = newEstado; }

	// Heredado v�a Entity
	virtual void inicializar() override;
	virtual void update(Time elapsedTime) override;
	virtual void handleInput() override;
	virtual void cargarContenido() override;
	virtual void borrarContenido() override;
	virtual void handleMessage(const Message& message) override;
	virtual std::string getClassName() { return "Granada"; }
	virtual bool handleTrigger(TriggerRecordStruct* Trigger) override;
	virtual void setPosition(const Vec3<float> &pos) override;

	void shoot(const btVector3& posicionPlayer);
	void serverShoot(TGranada& g);

	float explosion(Vec3<float> posExplosion, Vec3<float> posCharacter, float radio);

	btRigidBody* m_rigidBody;

	Clock clockRecargaGranada;

	float timeRecargaGranada = 2;

private:
	int estado= GRANADACARGADA;
	RakNet::RakNetGUID guidLanzador = RakNet::UNASSIGNED_RAKNET_GUID;
	Vec3<float> fuerza;
	float radioExplosion;
	btGhostObject* m_explosion;
};

