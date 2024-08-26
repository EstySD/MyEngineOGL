#include "scene.h"
Scene::Scene(const char* path) : path{path} {
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(path);
	if (!result) {
		std::cout << "Cant load scene: " << result;
		std::cout << " Description: " << result.description() << std::endl;
	}
}
