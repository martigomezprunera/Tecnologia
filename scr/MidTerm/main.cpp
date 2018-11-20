#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "..\..\dep\inc\xml\rapidxml.hpp"
#include "..\..\dep\inc\xml\rapidxml_utils.hpp"
#include "..\..\dep\inc\xml\rapidxml_iterators.hpp"
#include "..\..\dep\inc\xml\rapidxml_print.hpp"


void fXML()
{
	rapidxml::xml_document<> doc;
	std::ifstream file("fichero.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);	rapidxml::xml_node<> *pRoot = doc.first_node();	for (rapidxml::xml_node<> *pNode = pRoot->first_node("book"); pNode; pNode = pNode->next_sibling())
	{
		std::cout << pNode->name() << ": ";
		for (rapidxml::xml_attribute<> *pAttr = pNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			std::cout << pAttr->value() << std::endl;
		}
		for (rapidxml::xml_node<> *pNodeI = pNode->first_node(); pNodeI; pNodeI = pNodeI->next_sibling())
		{
			std::cout << pNodeI->name() << ": " << pNodeI->value() << std::endl;
			for (rapidxml::xml_attribute<> *pAttr = pNodeI->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				std::cout << pAttr->name() << ": " << pAttr->value() << std::endl;
			}
		}
		std::cout << std::endl;
	}
}

int main()
{
		fXML();	system("pause");

	return 0;

}