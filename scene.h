#include "pugixml/pugixml.hpp"
#include <iostream>

class Scene {
public:
	const char* path;
	Scene(const char* path);
	void LoadObjects();
};