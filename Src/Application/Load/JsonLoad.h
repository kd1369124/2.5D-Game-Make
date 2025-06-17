#pragma once
#include <string>
#include <vector>
#include "json.hpp"

struct NodeData {
	std::string name;
	Math::Vector3 pos;
	Math::Vector3 rotation;
	Math::Vector3 scale;
};

class JsonNodeLoader {
public:
	static std::vector<NodeData> LoadNodes(const std::string& filepath);
};