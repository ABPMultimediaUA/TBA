#include "WeaponDrop.h"
#include "../../Motor/PhysicsEngine.h"


WeaponDrop::WeaponDrop(std::shared_ptr<SceneNode> nodo, const std::string& name) : Entity(-1, nodo, name), timeRespawnWeapon(3)
{
}


WeaponDrop::~WeaponDrop()
{
}



void WeaponDrop::setPosition(const Vec3<float>& pos)
{
	m_renderState.setPosition(pos);

	btTransform transform = m_ghostObject->getWorldTransform();
	transform.setOrigin(btVector3(pos.getX(), pos.getY(), pos.getZ()));
	m_ghostObject->setWorldTransform(transform);

	m_nodo->setPosition(pos);
}

void WeaponDrop::asignaTiempo(Clock tiempo) {
	//ponemos el tiempo al tiempo real que el server te envia, cambiamos el estado a usado y quitamos la colision.
	
	//PhysicsEngine::i().removeGhostObject(m_ghostObject);
	estado = USADO;
	m_nodo->setVisible(false);
	clockRespawnWeapon = tiempo;
}

void WeaponDrop::ArmaCogida()
{
	if (estado == DISPONIBLE) {
		//PhysicsEngine::i().removeGhostObject(m_ghostObject);
		estado = USADO;
		clockRespawnWeapon.restart();
		m_nodo->setVisible(false);
	}
}