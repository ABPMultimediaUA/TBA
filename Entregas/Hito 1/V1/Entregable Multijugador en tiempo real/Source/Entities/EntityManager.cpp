
#include <iostream>

#include "EntityManager.h"
#include "Enemy.h"
#include <list>



void EntityManager::inicializar()
{
	

	std::unordered_map<int, Entity*>::iterator iter = m_entities.begin();
	for (; iter != m_entities.end(); ++iter) {
		iter->second->inicializar();
	}

}

void EntityManager::update(Time elapsedTime)
{

	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		i->second->update(elapsedTime);
	}

}


void EntityManager::updateRender(float interpolation)
{
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		i->second->updateRender(interpolation);
	}
}

void EntityManager::handleInput()
{
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		i->second->handleInput();
	}
}



void EntityManager::cargarContenido()
{
	std::unordered_map<int, Entity*>::iterator i = m_entities.begin();
	for (; i != m_entities.end(); ++i) {
		i->second->cargarContenido();
	}
}

void EntityManager::borrarContenido()
{
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		i->second->borrarContenido();
	}
}

void EntityManager::apagar()
{
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		//borramos la entities
		i->second->borrarContenido();
		delete i->second;
		i->second = 0;
	}
	//vaciamos el contenido del mapa
	m_entities.clear();

	m_jugadores.clear();

}

void EntityManager::registerEntity(Entity * entity)
{

	// comprobamos si la Entity tiene un ID
	if (entity->getID() == -1) {
		//Si no tiene, le damos una que este disponible
		//Lo hago asi porque no puedo asegurar que el id siguiente no este asignado ya
		while (m_entities.find(m_nextID) != m_entities.end()) {
			++m_nextID;
		}
		entity->setID(m_nextID++);
	}
	//en caso de que tenga ya id comprobamos que no este en uso
	else if (m_entities.find(entity->getID()) != m_entities.end()) {
		//mostramos error
		std::cout << "Error registrando entity de ID: " + entity->getID() << std::endl;
		return;
	}
	//si todo ha ido bien le asignamos el entity al map
	m_entities[entity->getID()] = entity;

	//Si la entity tiene GUID la a�adimos a la lista de jugadores
	if (entity->getGuid() != RakNet::UNASSIGNED_RAKNET_GUID) {
		m_jugadores[RakNet::RakNetGUID::ToUint32(entity->getGuid())] = entity;
	}

}

void EntityManager::removeEntity(Entity * entity)
{
	auto found = m_entities.find(entity->getID());

	//Si es diferente de m_entities.end() es que lo ha encontrado
	if (found != m_entities.end()) {
		m_entities.erase(found);
	}

	if (entity->getGuid() != RakNet::UNASSIGNED_RAKNET_GUID) {
		auto found_raknet = m_jugadores.find(RakNet::RakNetGUID::ToUint32(entity->getGuid()));
		//Si es diferente de m_entities.end() es que lo ha encontrado
		if (found_raknet != m_jugadores.end()) {
			m_jugadores.erase(found_raknet);
		}
	}
	
	//A�adimos la entity a una cola de borrado para asegurarnos de que no se
	//borra el mismo puntero dos veces.
	delete_set.insert(entity);

}

void EntityManager::cleanDeleteQueue()
{
	for (auto it = delete_set.begin(); it != delete_set.end(); ++it)
	{
		(*it)->borrarContenido();
		delete (*it);
	}
	delete_set.clear();
}

Entity * EntityManager::getEntity(int id)
{
	auto found = m_entities.find(id);
	if (found != m_entities.end())
		return found->second;
	//no existe devolvemos 0
	

	return NULL;
}

std::list<Entity*> EntityManager::getCharacters()
{
	std::list<Entity*>characters;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "Player" || i->second->getClassName() == "Enemy")
			characters.push_back(i->second);
	}
	return characters;
}

std::list<Entity*> EntityManager::getEnemies()
{
	std::list<Entity*>characters;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "Enemy" || i->second->getClassName() == "Enemy_Bot")
			characters.push_back(i->second);
	}
	return characters;
}

std::list<Entity*> EntityManager::getLifeObjects()
{
	std::list<Entity*>lifeObjects;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "LifeObject")
			lifeObjects.push_back(i->second);
	}
	return lifeObjects;
}

std::list<Entity*> EntityManager::getWeapons()
{
	std::list<Entity*>weapons;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "RocketLauncherDrop" || i->second->getClassName() == "PistolaDrop" || i->second->getClassName() == "AsaltoDrop")
			weapons.push_back(i->second);
	}
	return weapons;
}
std::list<Entity*> EntityManager::getRockets() {
	std::list<Entity*>weapons;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "RocketLauncherDrop")
			weapons.push_back(i->second);
	}
	return weapons;
}

std::list<Entity*> EntityManager::getPistolas() {
	std::list<Entity*>weapons;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "PistolaDrop")
			weapons.push_back(i->second);
	}
	return weapons;
}


std::list<Entity*> EntityManager::getAsalto() {
	std::list<Entity*>weapons;
	for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
		if (i->second->getClassName() == "AsaltoDrop")
			weapons.push_back(i->second);
	}
	return weapons;
}


Entity * EntityManager::getRaknetEntity(RakNet::RakNetGUID guid)
{
	auto found = m_jugadores.find(RakNet::RakNetGUID::ToUint32(guid));
	if (found != m_jugadores.end())
		return found->second;
	//no existe devolvemos 0
	return NULL;
}

void EntityManager::mostrarClientes() {
	std::cout << "////////////////////INICIO///////////////////" << std::endl;
	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		i->second->getGuid();
		
		std::cout << "Nombre del player: " << i->second->getName() << std::endl;
		std::cout << "Posicion: " << i->second->getRenderState()->getPosition().getX() << ", " << i->second->getRenderState()->getPosition().getZ() << std::endl;
		std::cout << "GUID de la Entity: " << RakNet::RakNetGUID::ToUint32(i->second->getGuid()) << std::endl;
		std::cout << "GUID de la Clave: " << i->first << std::endl;
		std::cout << "ID: " << i->second->getID() << std::endl;
		

	}
	std::cout << "//////////////////FIN/////////////////////" << std::endl;

}


void EntityManager::muestraPosClientes() {

	for (auto i = m_jugadores.begin(); i != m_jugadores.end(); ++i) {

		i->second->getGuid();

		//std::cout << "**Nombre del player: " << i->second->getName() << std::endl;
		//std::cout << "**Posicion: " << i->second->getRenderState()->getPosition().getX() << ", " << i->second->getRenderState()->getPosition().getZ() << std::endl;


	}

}