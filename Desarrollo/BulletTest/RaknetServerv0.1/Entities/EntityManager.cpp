#include "EntityManager.h"
#include "../Estructuras.h"
#include <string>


void EntityManager::sendPlayer(TPlayer & p, RakNet::RakPeerInterface *peer)
{
	TPlayer nuevocli;
	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
		//enviamos el nuevo player a todos los clientes que habian en el servidor
		bsOut.Write((RakNet::MessageID)NUEVO_PLAYER);
		bsOut.Write(p);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();

		//enviamos todos los clientes que habian en el servidor al nuevo player
	
		nuevocli.guid = i->second->getGuid();
		nuevocli.name = i->second->getName();
		nuevocli.position = i->second->getRenderState()->getPosition();
		bsOut.Write((RakNet::MessageID)NUEVO_CLIENTE);
		bsOut.Write(nuevocli);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p.guid, false);
		bsOut.Reset();
	}
}

void EntityManager::enviaNuevaPos(TMovimiento &p, RakNet::RakPeerInterface *peer)
{
	
	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != p.guid) {
			//std::cout << "///////ENVIAMOS POS INICIO////////" << std::endl;
			//std::cout << "El player: " << p.name << "se mueve desde: "<< getRaknetEntity(p.guid)->getRenderState()->getPosition().getX()<<","<< getRaknetEntity(p.guid)->getRenderState()->getPosition().getZ() <<" a la nueva pos: " << p.position.getX() << "," << p.position.getZ() << std::endl;
			//std::cout << "Enviamos esta nueva pos a: " << i->second->getName() <<"con guid: "<<RakNet::RakNetGUID::ToUint32(i->second->getGuid())<< std::endl;
			//enviamos la posicion actualizada del player a todos los clientes
			bsOut.Write((RakNet::MessageID)MOVIMIENTO);
			bsOut.Write(p);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
			//std::cout << "///////ENVIAMOS POS FINAL////////" << std::endl;
		}

	}
}

void EntityManager::lanzarGranda(TGranada & g, RakNet::RakPeerInterface * peer)
{

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != g.guid) {
			bsOut.Write((RakNet::MessageID)LANZAR_GRANADA);
			bsOut.Write(g);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
			//std::cout << "///////ENVIAMOS POS FINAL////////" << std::endl;
		}

	}

}



void EntityManager::enviaDesconexion(RakNet::RakNetGUID & guid, RakNet::RakPeerInterface *peer)
{

	
	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != guid) {

			//enviamos el guid del cliente que tiene que borrar
			bsOut.Write((RakNet::MessageID)DESCONECTADO);
			bsOut.Write(guid);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
		}

	}
}

void EntityManager::enviaDisparado(RakNet::RakNetGUID & guid, RakNet::RakNetGUID &dispara, RakNet::RakPeerInterface *peer)
{


	RakNet::BitStream bsOut;
		//se envia unicamente al cliente que ha sido disparado

			bsOut.Write((RakNet::MessageID)IMPACTO_BALA);
			
			bsOut.Write(dispara);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guid, false);
			bsOut.Reset();
		

	
}

void EntityManager::enviaDisparadoRocket(TImpactoRocket &impact, RakNet::RakPeerInterface * peer)
{

	RakNet::BitStream bsOut;
	//se envia unicamente al cliente que ha sido disparado

	bsOut.Write((RakNet::MessageID)IMPACTO_ROCKET);
	bsOut.Write(impact);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, impact.guidImpactado, false);
	bsOut.Reset();



}


void EntityManager::notificarMuerte(TPlayer & p, RakNet::RakPeerInterface *peer)
{


	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS
		
			//enviamos la estructura del cliente muerto para que todos cambien la posicion de ese cliente a la nueva pos asignada por el servidor
			bsOut.Write((RakNet::MessageID)MUERTE);
			bsOut.Write(p);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
		

	}
}

void EntityManager::enviaTiempoActualVida(Life * l, RakNet::RakNetGUID &guid, RakNet::RakPeerInterface * peer)
{
	RakNet::BitStream bsOut;
	TVidaServer vida;
	vida.id = l->getID();
	vida.tiempo = l->clockRecargaLife;


		//se envia a SOLO al nuevo cliente que se ha conectado

		//enviamos la estructura del cliente muerto para que todos cambien la posicion de ese cliente a la nueva pos asignada por el servidor
		bsOut.Write((RakNet::MessageID)NUEVA_VIDA);
		bsOut.Write(vida);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guid, false);
		bsOut.Reset();


	

}

void EntityManager::enviaTiempoActualArma(DropObject * o, RakNet::RakNetGUID &guid, RakNet::RakPeerInterface * peer)
{
	RakNet::BitStream bsOut;
	TVidaServer weapondrop;
	weapondrop.id = o->getID();
	weapondrop.tiempo = o->clockRecargaLife;


	//se envia a SOLO al nuevo cliente que se ha conectado

	//enviamos la estructura del cliente muerto para que todos cambien la posicion de ese cliente a la nueva pos asignada por el servidor
	bsOut.Write((RakNet::MessageID)NUEVA_ARMA);
	bsOut.Write(weapondrop);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guid, false);
	bsOut.Reset();




}

void EntityManager::enviarDisparoCliente(TBala & b, RakNet::RakPeerInterface *peer)
{


	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != b.guid) {

			//enviamos el guid del cliente que tiene que borrar
			bsOut.Write((RakNet::MessageID)DISPARAR_BALA);
			bsOut.Write(b);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
		}

	}
}


void EntityManager::enviarDisparoClienteRocket(TBala & b, RakNet::RakPeerInterface *peer)
{


	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != b.guid) {

			//enviamos el guid del cliente que tiene que borrar
			bsOut.Write((RakNet::MessageID)DISPARAR_ROCKET);
			bsOut.Write(b);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
		}

	}
}

void EntityManager::VidaCogida(int idVida, RakNet::RakPeerInterface * peer)
{

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS para que todos sepan que la vida con idVida ha sido cogida
		bsOut.Write((RakNet::MessageID)VIDA_COGIDA);
		bsOut.Write(idVida);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();


	}
}

void EntityManager::ArmaCogida(int idArma, RakNet::RakPeerInterface * peer)
{

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS para que todos sepan que el arma con idArma ha sido cogida y a todos le reaparezca a la vez
		bsOut.Write((RakNet::MessageID)ARMA_COGIDA);
		bsOut.Write(idArma);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();


	}
}

void EntityManager::inicializar()
{
	std::unordered_map<unsigned long, Entity*>::iterator iter = m_jugadores.begin();
	for (; iter != m_jugadores.end(); ++iter) {
		iter->second->inicializar();
	}

}

void EntityManager::update(Time elapsedTime)
{
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
		i->second->update(elapsedTime);
	}
}

void EntityManager::updateRender(float interpolation)
{
	/*for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		i->second->updateRender(interpolation);
	}*/
}

void EntityManager::handleInput()
{
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
		i->second->handleInput();
	}
}



void EntityManager::cargarContenido()
{
	std::unordered_map<unsigned long, Entity*>::iterator i = m_jugadores.begin();
	for (; i != m_jugadores.end(); ++i) {
		i->second->cargarContenido();
	}
}

void EntityManager::borrarContenido()
{
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
		i->second->borrarContenido();
	}
}

void EntityManager::apagar()
{
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
		//borramos la entities
		delete i->second;
		i->second = 0;
	}
	//vaciamos el contenido del mapa
	m_jugadores.clear();
}

void EntityManager::registerEntity(Entity * entity)
{

	//Si la entity tiene GUID la a�adimos a la lista de jugadores
	if (entity->getGuid() != RakNet::UNASSIGNED_RAKNET_GUID) {
		m_jugadores[RakNet::RakNetGUID::ToUint32(entity->getGuid())] = entity;
	}
	else {
		if (m_entities.find(entity->getID()) != m_entities.end()) {
			//mostramos error
			std::cout << "ESA VIDA o ARMA YA HA SIDO CREADA " + entity->getID() << std::endl;

		}
		else {
			//si todo ha ido bien le asignamos el entity al map
			std::cout << "CREO UNA VIDA o ARMA CON ID : " + entity->getID() << std::endl;
			m_entities[entity->getID()] = entity;
		}


	}




	
}

void EntityManager::removeEntity(Entity * entity)
{
	auto found = m_jugadores.find(RakNet::RakNetGUID::ToUint32(entity->getGuid()));
	//Si es diferente de m_entities.end() es que lo ha encontrado
	if (found != m_jugadores.end()) {
		m_jugadores.erase(found);
	}
}



Entity * EntityManager::getRaknetEntity(RakNet::RakNetGUID guid)
{
	auto found = m_jugadores.find(RakNet::RakNetGUID::ToUint32(guid));
	if (found != m_jugadores.end())
		return found->second;
	//no existe devolvemos 0
	return NULL;
}

Entity * EntityManager::getEntityID(int id)
{
	auto found = m_entities.find(id);
	if (found != m_entities.end())
		return found->second;
	//no existe devolvemos 0


	return NULL;
}

void EntityManager::mostrarClientes() {

	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {
	
		i->second->getGuid();
		std::cout << "Nombre del player: " << i->second->getName() << std::endl;
		std::cout << "Posicion: " << i->second->getRenderState()->getPosition().getX() << ", " << i->second->getRenderState()->getPosition().getZ() << std::endl;
		std::cout << "GUID de la Entity: " << RakNet::RakNetGUID::ToUint32(i->second->getGuid()) << std::endl;
		std::cout << "GUID de la Clave: " << i->first << std::endl;
		std::cout << "ID: " << i->second->getID() << std::endl;

	}


}

void EntityManager::enviaImpulso(TImpulso &impulso, RakNet::RakPeerInterface *peer)
{

	RakNet::BitStream bsOut;


	bsOut.Write((RakNet::MessageID)APLICAR_IMPULSO);
	bsOut.Write(impulso.fuerza);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, impulso.guid, false);
	bsOut.Reset();


}

void EntityManager::enviaCambioArma(TCambioArma & cambio, RakNet::RakPeerInterface * peer)
{

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a todos menos a nosotros mismos
		if (i->second->getGuid() != cambio.guid) {

			//enviamos el guid del cliente que tiene que borrar
			bsOut.Write((RakNet::MessageID)CAMBIO_ARMA);
			bsOut.Write(cambio);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
			bsOut.Reset();
		}

	}

}


void EntityManager::aumentaKill(RakNet::RakNetGUID & guid, RakNet::RakPeerInterface * peer)
{
	m_tabla.find(RakNet::RakNetGUID::ToUint32(guid))->second.kills += 1;

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS para que todos actualicen la tabla de puntuacion
		bsOut.Write((RakNet::MessageID)AUMENTA_KILL);
		bsOut.Write(guid);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();


	}
}

void EntityManager::aumentaMuerte(RakNet::RakNetGUID & guid, RakNet::RakPeerInterface * peer)
{
	m_tabla.find(RakNet::RakNetGUID::ToUint32(guid))->second.deaths += 1;

	RakNet::BitStream bsOut;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS para que todos actualicen la tabla de puntuacion
		bsOut.Write((RakNet::MessageID)AUMENTA_MUERTE);
		bsOut.Write(guid);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();


	}

}

void EntityManager::enviaFila(RakNet::RakPeerInterface * peer, TFilaTabla fila)
{

	RakNet::BitStream bsOut;

	m_tabla[RakNet::RakNetGUID::ToUint32(fila.guid)] = fila;

	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		//se envia a TODOS para que todos actualicen la tabla de puntuacion
		bsOut.Write((RakNet::MessageID)ACTUALIZA_TABLA);
		bsOut.Write(m_tabla);
		peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, i->second->getGuid(), false);
		bsOut.Reset();


	}

}
