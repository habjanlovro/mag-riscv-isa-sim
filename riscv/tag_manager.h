#ifndef _RISCV_TAG_MANAGER_H_
#define _RISCV_TAG_MANAGER_H_

#include <memory>
#include <stdio.h>
#include <vector>
#include <map>

#include "devices.h"


class tag_memory_t {
	public:
		tag_memory_t() {}
		tag_memory_t(const char *file_path);
	private:
		std::shared_ptr<mem_t> tag_mem;
		std::vector<std::vector<uint8_t>> policy;
		std::map<int, std::string> names;
};


class tag_manager_t {
	public:
		tag_manager_t();
		tag_manager_t(const std::shared_ptr<tag_memory_t>& m);
		regfile_t<uint8_t, NXPR, true> XPR_tags;
		regfile_t<uint8_t, NFPR, false> FPR_tags;
		uint8_t pc_tag;
	private:
		std::shared_ptr<tag_memory_t> memory;
		bool enabled;
};

#endif
