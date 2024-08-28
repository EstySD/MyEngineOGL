#include "pugixml/pugixml.hpp"
#include <iostream>
#include <vector>
#include "model.h"

class Scene {
private:
	std::vector<Model> models;
	DirectLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<BasicShader> shaders;
	std::vector<Mat> materials;
	pugi::xml_document doc;
	glm::vec3 viewPos;
	glm::mat4 projection, view;
	void LoadObjects();
	void LoadShaders();
	void LoadMaterials();
	Shader lightShader;
	LightMesh lightMesh;
	void LoadLight();
	void ObjectsDraw();
	void LightDraw();
public:
	Scene(const char* path);
	void updateCamera(glm::vec3 viewPos, glm::mat4 projection, glm::mat4 view);
	void Draw();
};