#pragma once
#include <unordered_map>
#include <string>
#include "TEntity.h"
#include "TModel.h"
class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	//TODO M�todos para a�adr resources, para buscar resources, meterlos al �rbol de la escena... (preguntar a Julio...)
	TModel* getMesh(std::string path);
	void getTexture(std::string path);
private:
	std::unordered_map<std::string, TModel*> models;
	//std::unordered_map<std::string, Texture> textures
};

