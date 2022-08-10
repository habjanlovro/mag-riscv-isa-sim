#include "tag_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "mmu.h"

tag_memory_t::tag_memory_t(const char *file_path) {
	std::ifstream tag_file(file_path);
	if (!tag_file.is_open()) {
		std::ostringstream oss;
		oss << "Failed to open tag file '" << file_path << ": " << strerror(errno);
		throw std::runtime_error(oss.str());
	}

	int line_num = 1;

	std::string line;
	std::getline(tag_file, line);
	int n = std::stod(line);
	if (n <= 0) {
		throw std::runtime_error("Wrong fromat of the D2CC policy!");
	}
	policy = std::vector<std::vector<uint8_t>>(n);
	tag_mem = std::make_shared<mem_t>(PGSIZE);
	names = std::map<int, std::string>();

	for (int i = 0; i < n; i++) {
		line_num++;
		policy[i] = std::vector<uint8_t>(n);
		std::getline(tag_file, line);
		std::stringstream line_stream(line);
		line_stream >> names[i];
		for (int j = 0; j < n; j++) {
			std::string item;
			line_stream >> item;
			policy[i][j] = std::stoul(item);
		}
	}

	while (std::getline(tag_file, line)) {
		line_num++;
		std::vector<std::string> tokens;
		std::stringstream line_stream(line);
		for (std::string token; std::getline(line_stream, token, ',');) {
			tokens.push_back(token);
		}

		if (tokens.size() != 3) {
			std::ostringstream oss;
			oss << "Line: " << line_num << ": Failed to read!";
			throw std::runtime_error(oss.str());
		}
		uint64_t addr = std::stoul(tokens[0], nullptr, 16);
		uint64_t size = std::stoul(tokens[1]);
		std::string tag = tokens[2];

		uint8_t *bytes = (uint8_t *) malloc(sizeof(uint8_t) * size);
		if (bytes == nullptr) {
			std::abort();
		}
		memset(bytes, 1, size);
		tag_mem->store(addr, size, bytes);
		free(bytes);
	}
}

tag_manager_t::tag_manager_t() : enabled(false) {}

tag_manager_t::tag_manager_t(const std::shared_ptr<tag_memory_t>& m)
		: memory(m) {
	enabled = memory.get() != 0;
}
