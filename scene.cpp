#include "scene.h"
Scene::Scene(const char* path){

	pugi::xml_parse_result result = doc.load_file(path);
	if (!result) {
		std::cout << "Cant load scene: " << result;
		std::cout << " Description: " << result.description() << std::endl;
	}
	LoadObjects();
	LoadShaders();
	LoadMaterials();
	LoadLight();
}
void Scene::updateCamera(glm::vec3 viewPos, glm::mat4 projection, glm::mat4 view)
{
	this->viewPos = viewPos;
	this->view = view;
	this->projection = projection;
}
void Scene::Draw()
{
	LightDraw();
	ObjectsDraw();
}
void Scene::LoadObjects() {
	pugi::xml_node xml_objects = doc.child("scene").child("objects");
	for (pugi::xml_node xml_object : xml_objects.children()) {
		Model model(xml_object.child("path").text().as_string());
		pugi::xml_node xml_transforms = xml_object.child("transforms");
		for (pugi::xml_node xml_transform : xml_transforms.children()) {
			Transform transform;
			pugi::xml_node xml_pos = xml_transform.child("position");
			float x = xml_pos.child("x").text().as_float();
			float y = xml_pos.child("y").text().as_float();
			float z = xml_pos.child("z").text().as_float();
			transform.setPos(glm::vec3(x, y, z));
			pugi::xml_node xml_rot = xml_transform.child("rotation");
			x = xml_rot.child("x").text().as_float();
			y = xml_rot.child("y").text().as_float();
			z = xml_rot.child("z").text().as_float();
			transform.setRot(glm::vec3(x, y, z));
			pugi::xml_node xml_scale = xml_transform.child("scale");
			x = xml_scale.child("x").text().as_float();
			y = xml_scale.child("y").text().as_float();
			z = xml_scale.child("z").text().as_float();
			transform.setScale(glm::vec3(x, y, z));
			model.transforms.push_back(transform);
		}
		model.MaterialIndex = xml_object.child("material").text().as_uint();
		models.push_back(model);

	}

}

void Scene::LoadShaders()
{
	pugi::xml_node xml_shaders = doc.child("scene").child("shaders");
	for (pugi::xml_node xml_shader : xml_shaders.children()) {
		shaders.push_back(BasicShader(xml_shader.text().as_string()));
	}

}

void Scene::LoadMaterials()
{
	pugi::xml_node xml_materials = doc.child("scene").child("materials");
	for (pugi::xml_node xml_material : xml_materials.children()) {
		Mat mat;
		mat.shaderIndex = xml_material.attribute("shader").as_uint();
		if (!xml_material.child("diffuse")) {
			materials.push_back(mat);
			continue;
		}
		mat = Mat(
			Texture(xml_material.child("diffuse").text().as_string(), TextureType::diffuse),
			Texture(xml_material.child("specular").text().as_string(), TextureType::specular),
			Texture(xml_material.child("normal").text().as_string(), TextureType::normal)
		);
		materials.push_back(
			mat
		);
	}
}

void Scene::LoadLight()
{

	lightShader = Shader("shaders/lightShader.glsl");
	pugi::xml_node xml_lights = doc.child("scene").child("lights");
	 //direct light
	pugi::xml_node xml_directLight = xml_lights.child("directLight");
	float x = xml_directLight.child("rotation").child("x").text().as_float();
	float y = xml_directLight.child("rotation").child("y").text().as_float();
	float z = xml_directLight.child("rotation").child("z").text().as_float();
	glm::vec3 pos = glm::vec3(x, y, z);
	x = xml_directLight.child("color").child("x").text().as_float();
	y = xml_directLight.child("color").child("y").text().as_float();
	z = xml_directLight.child("color").child("z").text().as_float();
	glm::vec3 color = glm::vec3(x, y, z);
	dirLight = DirectLight(pos, color);
	for (pugi::xml_node xml_pointLight : xml_lights.child("pointLights").children()) {
		glm::vec3 position, color;
		float constant, linear, quadratic;
		pugi::xml_node xml_pos = xml_pointLight.child("position");
		float x = xml_pos.child("x").text().as_float();
		float y = xml_pos.child("y").text().as_float();
		float z = xml_pos.child("z").text().as_float();
		position = glm::vec3(x, y, z);
		pugi::xml_node xml_color = xml_pointLight.child("color");
		x = xml_color.child("x").text().as_float();
		y = xml_color.child("y").text().as_float();
		z = xml_color.child("z").text().as_float();
		color = glm::vec3(x, y, z);
		constant = xml_pointLight.child("constant").text().as_float();
		linear = xml_pointLight.child("linear").text().as_float();
		quadratic = xml_pointLight.child("quadratic").text().as_float();
		pointLights.push_back(PointLight(position, color, constant, linear, quadratic));
	}

}

void Scene::ObjectsDraw()
{
	for (int i = 0; i < shaders.size(); i++) {
		shaders[i].use();
		shaders[i].setDirLight(dirLight);
		for (int j = 0; j < pointLights.size(); j++) {
			shaders[i].updatePointLight(pointLights[j], j);
		}
	}
	for (int i = 0; i < models.size(); i++) {
		unsigned int matIndex = models[i].MaterialIndex;
		unsigned int shaderIndex = materials[matIndex].shaderIndex;
		shaders[shaderIndex].use();
		shaders[shaderIndex].setVec3("viewPos", viewPos);
		for (int j = 0; j < models[i].transforms.size(); j++) {
			glm::mat4 mvp = glm::mat4(1.0);
			glm::mat4 model = models[i].transforms[j].getModelMatrix();
			mvp = projection*view*model*mvp;
			shaders[shaderIndex].use();
			shaders[shaderIndex].setMat4("model", model);
			shaders[shaderIndex].setMat4("transform", mvp);
			models[i].Draw(shaders[shaderIndex]);
		}
	}
}

void Scene::LightDraw()
{
	lightShader.use();
	for (int i = 0; i < pointLights.size(); i++) {
		glm::mat4 mvp = glm::mat4(1.0);
		mvp = projection * view * pointLights[i].transform.getModelMatrix()*mvp;
		lightShader.setMat4("transform", mvp);
		lightShader.setVec3("color", pointLights[i].color);
		lightMesh.Draw(lightShader);
	}
}


