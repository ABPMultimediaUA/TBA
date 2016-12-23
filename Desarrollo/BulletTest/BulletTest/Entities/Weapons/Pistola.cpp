#include "Pistola.h"
#include "../../Motor de Red/Cliente.h"
#include "../../Motor de Red/Estructuras.h"
#include "../EntityManager.h"

Pistola::Pistola() : Weapon()
{
	
}


Pistola::~Pistola()
{
}

void Pistola::inicializar()
{
	damage = 35;
	capacidadAmmo = 6;
	disparos = 0;
	recarga = milliseconds(1000);
	cadencia = milliseconds(350);
	numCargadores = numCargadoresPistola;
	SIZE_OF_WORLD = btVector3(1500, 1500, 1500);
	FUERZA = btVector3(10, 10, 10);
}

void Pistola::update(Time elapsedTime)
{
	if (equipada) {
		Vec3<float> player_pos = EntityManager::i().getEntity(PLAYER)->getRenderState()->getPosition();
		Vec3<float> player_rot = EntityManager::i().getEntity(PLAYER)->getRenderState()->getRotation();
		m_renderState.updatePositions(Vec3<float>(player_pos.getX(), player_pos.getY() + 6.5f, player_pos.getZ()));
		m_renderState.updateRotations(player_rot);

		if (estadoWeapon == DESCARGADA) {
			if (numCargadores > 0) {
				if (relojrecarga.getElapsedTime() < recarga) {
					printf("recargando\n");
				}
				else {
					estadoWeapon = CARGADA;
					disparos = 0;
					numCargadores--;
				}
			}
			else {
				relojrecarga.restart();
			}
		}
	}

}

void Pistola::handleInput()
{
}

void Pistola::cargarContenido()
{
	Vec3<float> player_pos = EntityManager::i().getEntity(PLAYER)->getRenderState()->getPosition();
	m_nodo = GraphicEngine::i().createAnimatedNode(Vec3<float>(player_pos.getX(), player_pos.getY()+6.5, player_pos.getZ()), Vec3<float>(0.2f, 0.2f, 0.2f), "", "../media/arma/pistola.obj");
	m_nodo->setVisible(false);
	m_nodo->setTexture("../media/ice0.jpg", 0);

}

void Pistola::borrarContenido()
{
}

void Pistola::handleMessage(const Message & message)
{

}

bool Pistola::handleTrigger(TriggerRecordStruct * Trigger)
{
	return false;
}

void Pistola::shoot() {

	if (disparos < capacidadAmmo) {


		if (relojCadencia.getElapsedTime().asMilliseconds() > cadencia.asMilliseconds()) {
			//aumentamos en uno el numero de disparos, para reducir la municion
			disparos++;

			// posicion de la camara
			btVector3 start = bt(GraphicEngine::i().getActiveCamera()->getPosition());

			//a�adimos un poco de desvio en el arma
			start += btVector3(Randf(-1.f, 1.f), Randf(-1.f, 1.f), Randf(-1.f, 1.f)) / 10.f;

			btVector3 target = bt(GraphicEngine::i().getActiveCamera()->getTarget());
			btVector3 direccion = target - bt(GraphicEngine::i().getActiveCamera()->getPosition());
			direccion.normalize();

			btVector3 end = start + (direccion*SIZE_OF_WORLD);

			btCollisionWorld::ClosestRayResultCallback ray(start, end);

			PhysicsEngine::i().m_world->rayTest(start, end, ray);

			btVector3 posicionImpacto;


			if (ray.hasHit())//si ray ha golpeado algo entro
			{
			
				Entity* ent = static_cast<Entity*>(ray.m_collisionObject->getUserPointer());

				if (ent != EntityManager::i().getEntity(PLAYER))
				{
					if (ent->getClassName() == "Enemy") {
						Message msg(ent, "COLISION_BALA", &damage);
						MessageHandler::i().sendMessage(msg);
					}
					//Para mover objetos del mapa
					posicionImpacto = ray.m_hitPointWorld;

					if (ent->getClassName() == "PhysicsEntity") {
						btRigidBody::upcast(ray.m_collisionObject)->activate(true);
						btRigidBody::upcast(ray.m_collisionObject)->applyImpulse(direccion*FUERZA, posicionImpacto);
					}
				}

				
			}

			GunBullet* bala = new GunBullet(cons(start), cons(direccion), cons(posicionImpacto), GraphicEngine::i().getActiveCamera()->getRotation());
			bala->cargarContenido();

			if (Cliente::i().isConected()) {
				//enviamos el disparo de la bala al servidor para que el resto de clientes puedan dibujarla
				Cliente::i().dispararBala(cons(start), cons(direccion), cons(posicionImpacto), GraphicEngine::i().getActiveCamera()->getRotation());
			}

			relojCadencia.restart();
		}

	}

	if (disparos == capacidadAmmo && estadoWeapon == CARGADA) {
		if (numCargadores > 0) {
			relojrecarga.restart();
		}
		estadoWeapon = DESCARGADA;
	}

}