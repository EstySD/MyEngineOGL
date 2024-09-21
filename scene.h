#pragma once
#include "ext/pugixml/pugixml.hpp"
#include <iostream>
#include <vector>
#include "model.h"
#include "camera.h"
#ifndef SCENE_H_
#define SCENE_H_

class Scene {
private:
	DirectLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	std::vector<Model> models;
	std::vector<BasicShader> shaders;
	std::vector<Mat> materials;
	pugi::xml_document doc;
	glm::mat4 projection, view;
	glm::vec2 resolution;
	void LoadCamera();
	void LoadObjects();
	void SaveObjects();
	void LoadShaders();
	void SaveShaders();
	void LoadMaterials();
	void SaveMaterials();
	Shader lightShader;
	BillBoard billBoard;
	void LoadLight();
	void SaveLight();
	void ObjectsDraw();
	void LightDraw();
public:
	glm::vec3 viewPos;

	const char* path;
	Scene(const char* path);
	void UpdateCamera(Camera& cam);
	void Save();
	void Draw();
	void AppendObject(Model model);
	void DeleteObject(unsigned int& num);
	void AppendMaterial(Mat mat);
	void DeleteMaterial(unsigned int& num);
	void AppendPointLight(PointLight pointLight);
	void DeletePointLight(unsigned int& num);

	glm::vec3& GetCamPos();
	std::vector<Model>& GetModels();
	std::vector<PointLight>& GetPointLights();
	DirectLight& GetDirLight();
};

#endif