
#include "Enemy.h"
#include <PhysicsEngine.h>
#include <GraphicEngine.h>
#include <Estructuras.h>
#include <Cliente.h>
#include <EntityManager.h>
#include <iostream>

//Clase que representa a un enemigo, esta clase recibe mensajes de sincronizacion de movimiento. 
//Tambien se encarga de enviar los mensajes apropiados al servidor cuando halla recibido un impacto
//de bala o de rocket.

Enemy::Enemy(const std::string& name, RakNet::RakNetGUID guid) : Entity(-1, NULL, name, guid)
{
	/*m_pStateMachine = new MachineState(this);
	m_pStateMachine->SetCurrentState(&Patrullar::i());
	m_pStateMachine->SetGlobalState(&Patrullar::i());*/
}


Enemy::~Enemy()
{

}

void Enemy::inicializar()
{
	animation = new Animation();
	granada = new Granada();
	m_isDying = false;
}

void Enemy::update(Time elapsedTime)
{
	desencolaMovimiento();

	updateState();
	updateAnimation();

	isMoving = true;
	if (m_renderState.getPreviousPosition().getX() == m_renderState.getPosition().getX() &&
		m_renderState.getPreviousPosition().getY() == m_renderState.getPosition().getY() &&
		m_renderState.getPreviousPosition().getZ() == m_renderState.getPosition().getZ())
		isMoving = false;


	//m_pStateMachine->Update();

	/*if (m_isDying && relojMuerte.getElapsedTime().asSeconds() > 3) {
		m_isDying = false;
	}*/

}

void Enemy::handleInput()
{
}

void Enemy::cargarContenido()
{
	//Creas el nodo(grafico)
	m_nodo = GraphicEngine::i().createAnimatedNode(Vec3<float>(0, 100, 0), Vec3<float>(0.05f, 0.05f, 0.05f), "", "../media/ArmyPilot.b3d");
	m_nodo.get()->setTexture("../media/body01.png", 1);
	m_nodo.get()->setTexture("../media/head01.png", 0);
	m_nodo.get()->setTexture("../media/m4tex.png", 2);

	//m_renderState.setPosition(Vec3<float>(0, 100, 0));

	animation->addAnimation("Default", 0, 0);
	animation->addAnimation("Run_Forwards", 1, 69);
	animation->addAnimation("Run_backwards", 70, 138);
	animation->addAnimation("Walk", 139, 183);
	animation->addAnimation("Jump", 184, 219);
	animation->addAnimation("Jump2", 184, 219);
	animation->addAnimation("Idle", 220, 472);
	animation->addAnimation("AimRunning", 473, 524);


	granada->inicializar();
	granada->cargarContenido();

	m_animState = quieto;

	radius = 1.2f;
	height = 7.3f;
	mass = 1000.f;


	m_rigidBody = PhysicsEngine::i().createCapsuleRigidBody(this, height, radius, mass, DISABLE_SIMULATION);

	btBroadphaseProxy* proxy = m_rigidBody->getBroadphaseProxy();
	proxy->m_collisionFilterGroup = col::Collisions::Enemy;
	proxy->m_collisionFilterMask = col::enemyCollidesWith;

}

void Enemy::borrarContenido()
{
	delete animation;

	PhysicsEngine::i().removeRigidBody(m_rigidBody);
	
	GraphicEngine::i().removeNode(m_nodo);
}
//Teletransporta un enemigo a la posicion que le pasas
void Enemy::setPosition(const Vec3<float>& pos)
{
	m_renderState.setPosition(pos);
	btTransform transform = m_rigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(pos.getX(), pos.getY(), pos.getZ()));
	m_rigidBody->setCenterOfMassTransform(transform);
	m_nodo.get()->setPosition(pos);
}


void Enemy::updateEnemigo(Vec3<float> pos) {
	m_renderState.updatePositions(pos);
	btTransform transform = m_rigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(pos.getX(), pos.getY() + (height / 2) + radius, pos.getZ()));
	m_rigidBody->setCenterOfMassTransform(transform);

}

void Enemy::handleMessage(const Message & message)
{
	if (message.mensaje == "COLISION_BALA") {
		if (!m_isDying) {
			//Este float * es una referencia a una variable de clase asi que no hay problema
			TImpactoBala impacto;
			impacto.damage = *static_cast<float*>(message.data);
			impacto.guid = m_guid;

			Cliente::i().dispatchMessage(impacto, IMPACTO_BALA);

			static_cast<Player*>(EntityManager::i().getEntity(PLAYER))->relojHit.restart();
		}
		
	}
	else if (message.mensaje == "ARMAUP") {
		//TODO poner el codigo de cambiar el modelo del arma hacia arriba

	}
	else if (message.mensaje == "ARMADOWN") {
		//TODO poner el codigo de cambiar el modelo del arma hacia abajo

	}
	else if (message.mensaje == "COLISION_ROCKET") {
		Cliente::i().dispatchMessage(*(TImpactoRocket*)message.data, IMPACTO_ROCKET);
		static_cast<Player*>(EntityManager::i().getEntity(PLAYER))->relojHit.restart();
		delete message.data;
	}
}

bool Enemy::handleTrigger(TriggerRecordStruct * Trigger)
{
	return false;
}

//pila posiciones
void Enemy::encolaMovimiento(TMovimiento& mov)
{
	// A�adir a la cola
	m_positions.push(mov);
}

void Enemy::desencolaMovimiento()
{	
	
	if (m_positions.size() > 3) {
		TMovimiento mov;
		while (!m_positions.empty()) {
			mov = m_positions.front();
			//lo borramos de la cola
			m_positions.pop();
			//llamamos al update con la nueva posicion
		}
		updateEnemigo(mov.position);
		m_renderState.updateRotations(mov.rotation);
		m_isDying = mov.isDying;
	}

	else if (m_positions.size() > 0) {
		TMovimiento mov;
		mov = m_positions.front();
		//lo borramos de la cola
		m_positions.pop();
		//llamamos al update con la nueva posicion
		updateEnemigo(mov.position);
		m_renderState.updateRotations(mov.rotation);
		m_isDying = mov.isDying;
	}else {
		updateEnemigo(m_renderState.getPosition() + m_renderState.getVelocity() * (1.f / 15.f));
		
	}

}

void Enemy::lanzarGranada(TGranada g)
{
	granada->serverShoot(g);
}


void Enemy::updateAnimation()
{
	switch (m_animState)
	{
	case quieto:
		if (animation->getActualAnimation() != "Idle") {
			m_nodo.get()->setAnimation(animation->getAnimationStart("Idle"), animation->getAnimationEnd("Idle"));
		}
		break;

	case andando:
		if (animation->getActualAnimation() != "Walk") {
			m_nodo.get()->setAnimation(animation->getAnimationStart("Walk"), animation->getAnimationEnd("Walk"));
		}
		break;

	case saltando:
		if (animation->getActualAnimation() != "Jump") {
			m_nodo.get()->setAnimation(animation->getAnimationStart("Jump"), animation->getAnimationEnd("Jump"));
		}
		break;
	case saltando2:
		if (animation->getActualAnimation() != "Jump2") {
			m_nodo.get()->setAnimation(animation->getAnimationStart("Jump2"), animation->getAnimationEnd("Jump2"));
		}
		break;

	}
}

void Enemy::updateState()
{
	if (isMoving) {
		m_animState = andando;
	}
	else {
		m_animState = quieto;
	}
}
