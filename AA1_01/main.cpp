#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "..\dep\inc\xml\rapidxml.hpp"
#include "..\dep\inc\xml\rapidxml_utils.hpp"
#include "..\dep\inc\xml\rapidxml_iterators.hpp"
#include "..\dep\inc\xml\rapidxml_print.hpp"

//TIPO DE ARMA
class Weapon
{
	public:
		int idWeapon;
		int alcance;
};


//ESTRUCTURA DE DATOS ENEMIGO
class Enemigo
{
	public:
		int idEnemigo;
		int vitalidad;
		int ataque;
		int defensa;
		int velocidad;
		int experiencia;
		Weapon w[2];
};

void main()
{	
	rapidxml::xml_document<> doc;
	std::ifstream file("config.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	//ENEMIGOS
	std::vector<Enemigo> enemies;
	int i = 0;
	int j = 0;

	//LEEMOS XML
	rapidxml::xml_node<> *pRoot = doc.first_node();
	for (rapidxml::xml_node<> *pNode = pRoot->first_node("enemigo"); pNode; pNode = pNode->next_sibling())
	{
		//DATOS PARA VECTOR
		Enemigo aux;
		aux.idEnemigo = std::stoi(pNode->first_node("id")->value());
		aux.vitalidad = std::stoi(pNode->first_node("vitalidad")->value());
		aux.ataque = std::stoi(pNode->first_node("ataque")->value());
		aux.defensa = std::stoi(pNode->first_node("defensa")->value());
		aux.velocidad = std::stoi(pNode->first_node("velocidad")->value());
		aux.experiencia = std::stoi(pNode->first_node("experiencia")->value());
		
		//ATTRIBUTOS
		for (rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			aux.w[0].idWeapon = std::stoi(pAttr->value());
		}

		enemies.push_back(aux);
	}


	system("pause");
}