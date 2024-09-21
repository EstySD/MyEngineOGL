#include "scene.h"
Scene::Scene(const char* path){
	this->path = path;
	pugi::xml_parse_result result = doc.load_file(path);
	if (!result) {
		std::cout << "Cant load scene: " << result;
		std::cout << " Description: " << result.description() << std::endl;
	}
	LoadObjects();
	LoadShaders();
	LoadMaterials();
	LoadLight();
	LoadCamera();
}
void Scene::LoadCamera()
{
	pugi::xml_node camPos = doc.child("scene").child("camera").child("position");
	viewPos = glm::vec3(camPos.child("x").text().as_float(),
		camPos.child("y").text().as_float(),
		camPos.child("z").text().as_float()
	);
}
void Scene::UpdateCamera(Camera& cam)
{
	this->projection = cam.GetProj();
	this->view = cam.GetView();
	this->viewPos = cam.GetPos();
	this->resolution = cam.resolution;
	pugi::xml_node camPos=doc.child("scene").child("camera").child("position");
	camPos.child("x").text().set(viewPos.x);
	camPos.child("y").text().set(viewPos.y);
	camPos.child("z").text().set(viewPos.z);
}
void Scene::Save()
{
	SaveObjects();
	SaveMaterials();
	SaveLight();
	doc.save_file(path);
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
		pugi::xml_node xml_transform = xml_object.child("transform");
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
			model.transform = transform;
		model.MaterialIndex = xml_object.child("material").text().as_uint();
		models.push_back(model);
	}
}

void Scene::SaveObjects()
{
	pugi::xml_node xml_objects = doc.child("scene").child("objects");
	int i = 0;//object count
	for (pugi::xml_node xml_object : xml_objects.children()) {
		pugi::xml_node xml_transform = xml_object.child("transform");
			Transform transform = models[i].transform;
			pugi::xml_node xml_pos = xml_transform.child("position");
			xml_pos.child("x").text().set(transform.getPos().x);
			xml_pos.child("y").text().set(transform.getPos().y);
			xml_pos.child("z").text().set(transform.getPos().z);
			pugi::xml_node xml_rot = xml_transform.child("rotation");
			xml_rot.child("x").text().set(transform.getRot().x);
			xml_rot.child("y").text().set(transform.getRot().y);
			xml_rot.child("z").text().set(transform.getRot().z);
			pugi::xml_node xml_scale = xml_transform.child("scale");
			xml_scale.child("x").text().set(transform.getScale().x);
			xml_scale.child("y").text().set(transform.getScale().y);
			xml_scale.child("z").text().set(transform.getScale().z);
		xml_object.child("material").text().set(models[i].MaterialIndex);
		i++;
	}
}

void Scene::AppendObject(Model model)
{
	pugi::xml_node xml_objects = doc.child("scene").child("objects");
	pugi::xml_node xml_object = xml_objects.append_child("object");
	xml_object.append_child("path").text().set(model.path);
	pugi::xml_node xml_transform = xml_object.append_child("transform");
	pugi::xml_node xml_pos = xml_transform.append_child("position");
	xml_pos.append_child("x").text().set(model.transform.getPos().x);
	xml_pos.append_child("y").text().set(model.transform.getPos().y);
	xml_pos.append_child("z").text().set(model.transform.getPos().z);
	pugi::xml_node xml_rot = xml_transform.append_child("rotation");
	xml_rot.append_child("x").text().set(model.transform.eulerRot.x);
	xml_rot.append_child("y").text().set(model.transform.eulerRot.y);
	xml_rot.append_child("z").text().set(model.transform.eulerRot.z);
	pugi::xml_node xml_scale = xml_transform.append_child("scale");
	xml_scale.append_child("x").text().set(model.transform.scale.x);
	xml_scale.append_child("y").text().set(model.transform.scale.y);
	xml_scale.append_child("z").text().set(model.transform.scale.z);

	xml_object.append_child("material").text().set(model.MaterialIndex);
	models.push_back(model);
}

void Scene::DeleteObject(unsigned int& num)
{
	pugi::xml_node xml_objects = doc.child("scene").child("objects");
	int i = 0;
	for (pugi::xml_node xml_object : xml_objects.children()) {
		if (i == num) {
			xml_objects.remove_child(xml_object);
			models.erase(models.begin() + i);
		}
		i++;
	}
}

void Scene::LoadShaders()
{
	pugi::xml_node xml_shaders = doc.child("scene").child("shaders");
	for (pugi::xml_node xml_shader : xml_shaders.children()) {
		shaders.push_back(BasicShader(xml_shader.text().as_string()));
	}
}

void Scene::SaveShaders()
{
	pugi::xml_node xml_shaders = doc.child("scene").child("shaders");
	int i = 0;
	for (pugi::xml_node xml_shader : xml_shaders.children()) {
		xml_shader.text().set(shaders[i].filepath);
		i++;
	}
}

void Scene::LoadMaterials()
{

	pugi::xml_node xml_materials = doc.child("scene").child("materials");
	for (pugi::xml_node xml_material : xml_materials.children()) {
		Mat mat;
		mat.shaderIndex = xml_material.attribute("shader").as_uint();
		mat.setMode((MatType)xml_material.attribute("type").as_uint());
		if (!xml_material.child("diffuse")) {
			materials.push_back(mat);
			continue;
		}
		mat = Mat(
			Texture(xml_material.child("diffuse").text().get(), TextureType::diffuse),
			Texture(xml_material.child("specular").text().as_string(), TextureType::specular),
			Texture(xml_material.child("normal").text().as_string(), TextureType::normal)
		);
		materials.push_back(mat);
	}
}

void Scene::SaveMaterials()
{

	pugi::xml_node xml_materials = doc.child("scene").child("materials");
	int i = 0;
	for (pugi::xml_node xml_material : xml_materials.children()) {
		Mat mat = materials[i];
		xml_material.attribute("shader").set_value(mat.shaderIndex);
		xml_material.attribute("type").set_value(mat.getMode());
		if (mat.empty) continue;
		xml_material.child("diffuse").text().set(mat.diffuse.path);
		xml_material.child("specular").text().set(mat.specular.path);
		xml_material.child("normal").text().set(mat.normal.path);
		i++;
	}
}

void Scene::AppendMaterial(Mat mat)
{
	pugi::xml_node xml_materials = doc.child("scene").child("materials");
	pugi::xml_node xml_mat = xml_materials.append_child("material");
	xml_mat.append_attribute("shader").set_value(mat.shaderIndex);
	xml_mat.append_attribute("type").set_value(mat.getMode());
	if (!mat.empty) {
		xml_mat.append_child("diffuse").text().set(mat.diffuse.path);
		xml_mat.append_child("specular").text().set(mat.specular.path);
		xml_mat.append_child("normal").text().set(mat.normal.path);
	}
	materials.push_back(mat);
}

void Scene::DeleteMaterial(unsigned int& num)
{
	pugi::xml_node xml_materials = doc.child("scene").child("materials");
	int i = 0;
	for (pugi::xml_node xml_material : xml_materials.children()) {
		if (i == num) {
			xml_materials.remove_child(xml_material);
		}
		i++;
	}
	materials.erase(materials.begin() + num);
}

void Scene::AppendPointLight(PointLight pointLight)
{
	pugi::xml_node xml_lights = doc.child("scene").child("lights");
	pugi::xml_node xml_pointLight= xml_lights.child("pointLights").append_child("pointLight");
	pugi::xml_node xml_pos = xml_pointLight.append_child("position");
	xml_pos.append_child("x").text().set(pointLight.transform.pos.x);
	xml_pos.append_child("y").text().set(pointLight.transform.pos.y);
	xml_pos.append_child("z").text().set(pointLight.transform.pos.z);
	xml_pointLight.append_child("scale").text().set(pointLight.transform.scale.x);
	pugi::xml_node xml_color = xml_pointLight.append_child("color");
	xml_color.append_child("x").text().set(pointLight.color.x);
	xml_color.append_child("y").text().set(pointLight.color.y);
	xml_color.append_child("z").text().set(pointLight.color.z);
	xml_pointLight.append_child("constant").text().set(pointLight.clq.x);
	xml_pointLight.append_child("linear").text().set(pointLight.clq.y);
	xml_pointLight.append_child("quadratic").text().set(pointLight.clq.z);
	pointLights.push_back(pointLight);
}

void Scene::DeletePointLight(unsigned int &num)
{
	pugi::xml_node xml_lights = doc.child("scene").child("lights").child("pointLights");
	int i = 0;
	for (pugi::xml_node xml_light : xml_lights.children()) {
		if (i == num) {
			xml_lights.remove_child(xml_light);
		}
		i++;
	}
	pointLights.erase(pointLights.begin() + num);
}

glm::vec3& Scene::GetCamPos()
{
	return viewPos;
}

std::vector<Model>& Scene::GetModels()
{
	return models;
}

std::vector<PointLight>& Scene::GetPointLights()
{
	return pointLights;
}

DirectLight& Scene::GetDirLight()
{
	return dirLight;
}

void Scene::LoadLight()
{

	lightShader = Shader("shaders/LightBillboard.glsl");
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
	//point light
	for (pugi::xml_node xml_pointLight : xml_lights.child("pointLights").children()) {
		glm::vec3 position, color;
		float constant, linear, quadratic;
		pugi::xml_node xml_pos = xml_pointLight.child("position");
		float x = xml_pos.child("x").text().as_float();
		float y = xml_pos.child("y").text().as_float();
		float z = xml_pos.child("z").text().as_float();
		position = glm::vec3(x, y, z);
		float size = xml_pointLight.child("scale").text().as_float();
		pugi::xml_node xml_color = xml_pointLight.child("color");
		x = xml_color.child("x").text().as_float();
		y = xml_color.child("y").text().as_float();
		z = xml_color.child("z").text().as_float();
		color = glm::vec3(x, y, z);
		constant = xml_pointLight.child("constant").text().as_float();
		linear = xml_pointLight.child("linear").text().as_float();
		quadratic = xml_pointLight.child("quadratic").text().as_float();
		pointLights.push_back(PointLight(position, color, glm::vec3(size),glm::vec3(constant, linear, quadratic)));
	}
	//spot light
	for (pugi::xml_node xml_spotLight : xml_lights.child("spotLights").children()) {
		glm::vec3 position, direction, color;
		float cutOff, outercutOff;
		pugi::xml_node xml_pos = xml_spotLight.child("position");
		float x = xml_pos.child("x").text().as_float();
		float y = xml_pos.child("y").text().as_float();
		float z = xml_pos.child("z").text().as_float();
		position = glm::vec3(x, y, z);
		pugi::xml_node xml_color = xml_spotLight.child("color");
		x = xml_color.child("x").text().as_float();
		y = xml_color.child("y").text().as_float();
		z = xml_color.child("z").text().as_float();
		color = glm::vec3(x, y, z);
		pugi::xml_node xml_direction = xml_spotLight.child("rotation");
		x = xml_direction.child("x").text().as_float();
		y = xml_direction.child("y").text().as_float();
		z = xml_direction.child("z").text().as_float();
		direction = glm::vec3(x, y, z);
		cutOff = xml_spotLight.child("cutOff").text().as_float();
		outercutOff = xml_spotLight.child("outercutOff").text().as_float();
		spotLights.push_back(SpotLight(position, direction, color, cutOff, outercutOff));
	}
}

void Scene::SaveLight()
{
	pugi::xml_node xml_lights = doc.child("scene").child("lights");
	//direct light
	pugi::xml_node xml_directLight = xml_lights.child("directLight");
	xml_directLight.child("rotation").child("x").text().set(dirLight.transform.eulerRot.x);
	xml_directLight.child("rotation").child("y").text().set(dirLight.transform.eulerRot.y);
	xml_directLight.child("rotation").child("z").text().set(dirLight.transform.eulerRot.z);
	xml_directLight.child("color").child("x").text().set(dirLight.color.x);
	xml_directLight.child("color").child("y").text().set(dirLight.color.y);
	xml_directLight.child("color").child("z").text().set(dirLight.color.z);
	//point light
	int i = 0;
	for (pugi::xml_node xml_pointLight : xml_lights.child("pointLights").children()) {
		pugi::xml_node xml_pos = xml_pointLight.child("position");
		xml_pos.child("x").text().set(pointLights[i].transform.pos.x);
		xml_pos.child("y").text().set(pointLights[i].transform.pos.y);
		xml_pos.child("z").text().set(pointLights[i].transform.pos.z);
		xml_pointLight.child("scale").text().set(pointLights[i].transform.scale.x);
		pugi::xml_node xml_color = xml_pointLight.child("color");
		xml_color.child("x").text().set(pointLights[i].color.x);
		xml_color.child("y").text().set(pointLights[i].color.y);
		xml_color.child("z").text().set(pointLights[i].color.z);
		xml_pointLight.child("constant").text().set(pointLights[i].clq.x);
		xml_pointLight.child("linear").text().set(pointLights[i].clq.y);
		xml_pointLight.child("quadratic").text().set(pointLights[i].clq.z);
		i++;
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
		shaders[shaderIndex].setVec("viewPos", viewPos);
		glm::mat4 mvp = glm::mat4(1.0);
		glm::mat4 model = models[i].transform.getModelMatrix();
		mvp = projection*view*model*mvp;
		shaders[shaderIndex].use();
		shaders[shaderIndex].setMat4("model", model);
		shaders[shaderIndex].setMat4("transform", mvp);
		Mat mat = materials[models[i].MaterialIndex];
		if (mat.empty == true)
			models[i].Draw(shaders[shaderIndex]);
		else models[i].Draw(shaders[shaderIndex], mat);
	}
}

void Scene::LightDraw()
{
	lightShader.use();
	for (int i = 0; i < pointLights.size(); i++) {
		glm::mat4 mvp = glm::mat4(1.0);
		mvp = projection *view
			* pointLights[i].transform.getModelMatrix() * mvp;
		lightShader.setMat4("transform", mvp);
		lightShader.setVec("color", pointLights[i].color);
		lightShader.setVec("resolution", resolution);
		billBoard.Draw(lightShader, pointLights[i].transform, view);
	}
}


