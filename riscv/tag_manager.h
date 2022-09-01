#ifndef _RISCV_TAG_MANAGER_H_
#define _RISCV_TAG_MANAGER_H_

#include <memory>
#include <stdio.h>
#include <vector>
#include <map>

#include "devices.h"
#include "simif.h"


#define TAG_INVALID ((uint8_t) ((1 << 8) - 1))

class processor_t;
class mmu_t;

#include <iostream>


struct tag_region_data_t {
	reg_t addr;
	size_t size;
	uint8_t tag;
};

struct tag_pg_t {
	std::string name;
	size_t fd;
	uint8_t tag;

	tag_pg_t(const std::string& name, const size_t fd, const uint8_t tag)
		: name(name), fd(fd), tag(tag) {}
	tag_pg_t() : tag(255) {}
};

class tag_memory_t : public simif_t {
	public:
		tag_memory_t() : tag_fd(-1) {}
		tag_memory_t(const std::string& policy_file, const std::string& tag_file);
		virtual ~tag_memory_t();

		inline uint8_t lca(const uint8_t x, const uint8_t y) const {
			return policy[x][y];
		}

		void add_mem(const reg_t addr, abstract_device_t *device);

		uint8_t fetch_tag(const reg_t loc);
		void store_tag(reg_t loc, size_t size, uint8_t *buffer);
		void load_tag(reg_t loc, const size_t size, uint8_t *buffer);

		  // should return NULL for MMIO addresses
		char* addr_to_mem(reg_t addr);
		// used for MMIO addresses
		bool mmio_load(reg_t addr, size_t len, uint8_t* bytes);
		bool mmio_store(reg_t addr, size_t len, const uint8_t* bytes);
		// Callback for processors to let the simulation know they were reset.
		void proc_reset(unsigned id) {}

  		const char* get_symbol(uint64_t addr);

		const std::vector<tag_region_data_t>& get_tagged_data() const {
			return tagged_data;
		}

		void say_hello() {
			std::cerr << "Hello from tag memory!" << std::endl;
		}

		void copy_tag_mem(reg_t pbuf, reg_t len, reg_t off);

	private:
		std::vector<std::vector<uint8_t>> policy;
		std::map<int, std::string> names;
		std::vector<tag_region_data_t> tagged_data;
		std::map<size_t, tag_pg_t> perimiter_guards;
		bus_t *bus;
		int tag_fd;
};


class tag_manager_t {
	public:
		tag_manager_t();
		tag_manager_t(tag_memory_t *m, processor_t *p);
		~tag_manager_t();

		void propagate(const reg_t pc, const reg_t rd);
		void propagate(const reg_t pc, const reg_t rd, const uint8_t rs);
		void propagate(const reg_t pc, const reg_t rd, const uint8_t rs1,
			const uint8_t rs2);

		void propagate_branch(const reg_t pc, const reg_t jmp_addr,
			const uint8_t rs1, const uint8_t rs2);

		void load(const reg_t pc, reg_t rd, uint8_t rs, reg_t addr,
			size_t num_bytes);
		void store(const reg_t pc, const uint8_t rs1, const uint8_t rs2,
			reg_t addr, size_t num_bytes);

		void jump(const reg_t pc, const reg_t rd, const reg_t addr);
		void jump(const reg_t pc, const reg_t rd, const uint8_t rs, const reg_t addr);


		void print();


		regfile_t<uint8_t, NXPR, true> XPR_tags;
		regfile_t<uint8_t, NFPR, false> FPR_tags;
		uint8_t pc_tag;
	private:
		bool enabled;
		tag_memory_t *memory;
		processor_t *processor;
		mmu_t *mmu;
		uint8_t temp_mem[sizeof(uint64_t)];
};

#endif
