#include "JsonLoad.h"

std::vector<NodeData> JsonNodeLoader::LoadNodes(const std::string& filepath)
{
	std::vector<NodeData> nodes;
	std::ifstream file(filepath);
	if (!file.is_open()) return nodes;

	nlohmann::json json;
	file >> json;

	for (const auto& node : json["nodes"]) {
		NodeData data;
		data.name = node["name"];
		data.pos = Math::Vector3(
			node["position"][0], node["position"][1], node["position"][2]);
		data.rotation = Math::Vector3(
			node["rotation"][0], node["rotation"][1], node["rotation"][2]);
		data.scale = Math::Vector3(
			node["scale"][0], node["scale"][1], node["scale"][2]);

		nodes.push_back(data);
	}

	return nodes;
}

std::vector<NodeData> JsonNodeLoader::LoadNodeS(const std::string& filepath)
{
	std::vector<NodeData> nodes;
	std::ifstream file(filepath);
	if (!file.is_open()) return nodes;

	nlohmann::json json;
	file >> json;

	for (const auto& node : json["nodes"]) 
	{
		NodeData data;
		data.name = node["name"];
		data.type = node.value("type", "");
		data.pos = Math::Vector3(
			node["position"][0], node["position"][1], node["position"][2]);
		data.rotation = Math::Vector3(
			node["rotation"][0], node["rotation"][1], node["rotation"][2]);
		data.scale = Math::Vector3(
			node["scale"][0], node["scale"][1], node["scale"][2]);
		data.params = node.value("params", nlohmann::json{});

		nodes.push_back(data);
	}

	return nodes;
}
