#pragma once

#include <SparseGraph.h>
#include "Celda.h"
#include <vec3.hpp>
#include <MapLoader.h>


class Map
{
public:
	static Map& i() {
		static Map singleton;
		return singleton;
	}


	void inicializar();

	
	void borrarContenido();

	SparseGraph& getGrafo() {
		return *grafo;
	}

	//este metodo se encarga de comprobar si hay algun obstaculo entre las dos posiciones (un raycast por ejemplo), si no hay ningun obstaculo se sigue un camino recto, sino hay que calcular el camino.
	bool isPathObstructed(Vec2f posIni, Vec2f posFinal, float radio);
	void ConvertirNodosAPosiciones(std::list<int>& CaminoDeNodos, std::list<Vec2f>& camino);
	void addNodosToCells();


	//Busca en la lista de puntos de spawn alguno que no intersecte con ning�n enemigo de radio x,
	//luego con la lista que queda se coge un punto aleatorio
	Vec3<float> searchSpawnPoint();

	std::vector<Vec3<float>> getSpawnPoints() { return map.getSpawnPoints(); };

	void CalcularNodosCercanos(Vec2f& pos, std::list<NavGraphNode*>& nodosCercanos, Vec2f& posBot);


	

private:
	//True, grafo dirigido
	SparseGraph* grafo;
	CellSpacePartition* cellSpace;


	//Cargador del mapa
	MapLoader map;
	


	Map();
	
};

