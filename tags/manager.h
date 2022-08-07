#ifndef _TAGS_MANAGER_H_
#define _TAGS_MANAGER_H_

#include <memory>
#include <stdio.h>
#include <vector>
#include <map>

#include "devices.h"

int test_tag();


class tag_manager_t {
	public:
		tag_manager_t(const char *tag_file_path);
		~tag_manager_t();

	private:
		mem_t *tag_mem;
		std::vector<std::vector<uint8_t>> policy_matrix;
		std::map<int, std::string> tag_names;
		uint8_t reg_tags[NXPR];
		uint8_t pc_tag;
};

#endif
