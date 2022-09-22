#ifndef _RISCV_TAG_MANAGER_H_
#define _RISCV_TAG_MANAGER_H_

#include <memory>
#include <stdio.h>
#include <vector>
#include <map>
#include <exception>

#include "devices.h"
#include "simif.h"


#define TAG_INVALID ((uint8_t) ((1 << 8) - 1))

class processor_t;
class mmu_t;


struct tag_region_data_t {
	reg_t addr;
	size_t size;
	uint8_t tag;
};

struct tag_pg_t {
	std::string name;
	std::string file;
	uint8_t tag;

	tag_pg_t(const std::string& name, const std::string& file, const uint8_t tag)
		: name(name), file(file), tag(tag) {}
	tag_pg_t() : tag(255) {}
};

class tag_memory_t : public simif_t {
	public:
		tag_memory_t();
		tag_memory_t(const std::string& policy_file, const std::string& tag_file);
		virtual ~tag_memory_t();

		inline uint8_t lca(const uint8_t x, const uint8_t y) const {
			return policy[x][y];
		}

		void add_mem(const reg_t addr, abstract_device_t *device);

		  // should return NULL for MMIO addresses
		char* addr_to_mem(reg_t addr);
		// used for MMIO addresses
		bool mmio_load(reg_t addr, size_t len, uint8_t* bytes);
		bool mmio_store(reg_t addr, size_t len, const uint8_t* bytes);
		// Callback for processors to let the simulation know they were reset.
		void proc_reset(unsigned id) {}

  		const char* get_symbol(uint64_t addr);

		std::vector<uint8_t> copy_tag_mem(reg_t pbuf, reg_t len, reg_t off);

		std::vector<uint8_t> pg_in(int fd, reg_t pbuf, reg_t len);
		bool pg_out(int fd, reg_t addr, const std::vector<uint8_t>& data);

		void register_fd(const std::string& name, int fd);
		void unregister_fd(int fd);

		inline bool is_descendant(const uint8_t lhs, const uint8_t rhs);

		uint64_t store_chunk_check(uint64_t lhs, uint64_t rhs);

		bool enabled;
	private:
		std::vector<std::vector<uint8_t>> policy;
		std::map<int, std::string> names;
		std::vector<tag_region_data_t> tagged_data;
		std::map<int, tag_pg_t> active_perimeter_guards;
		std::vector<tag_pg_t> perimeter_guards;
		bus_t *bus;
		int tag_fd;
};


class tag_manager_t {
	public:
		tag_manager_t();
		tag_manager_t(tag_memory_t *m, processor_t *p);
		~tag_manager_t();

		void propagate(const uint8_t pc_addr_tag, const reg_t rd,
			bool f = false);
		void propagate(const uint8_t pc_addr_tag, const reg_t rd,
			const uint8_t rs, bool f = false);
		void propagate(const uint8_t pc_addr_tag, const reg_t rd,
			const uint8_t rs1, const uint8_t rs2, bool f = false);
		void propagate(const uint8_t pc_addr_tag, const reg_t rd,
			const uint8_t rs1, const uint8_t rs2, const uint8_t rs3,
			bool f = false);

		void propagate_branch(const uint8_t pc_addr_tag, const reg_t jmp_addr);
		void propagate_branch(const uint8_t pc_addr_tag, const reg_t jmp_addr,
			const uint8_t rs1);
		void propagate_branch(const uint8_t pc_addr_tag, const reg_t jmp_addr,
			const uint8_t rs1, const uint8_t rs2);


		template<typename T>
		void load(const uint8_t pc_addr_tag, const reg_t addr, T tag_bytes,
			reg_t rd, uint8_t rs, bool f = false);

		template<typename T>
		T store(const uint8_t pc_addr_tag, const reg_t store_addr,
			const uint8_t rs1, const uint8_t rs2);

		void jump(const uint8_t pc_addr_tag, const reg_t jmp_addr,
			const reg_t rd);
		void jump(const uint8_t pc_addr_tag, const reg_t jmp_addr,
			const reg_t rd, const uint8_t rs);

		template<typename T>
		T amo(const uint8_t pc_addr_tag, const reg_t addr, const T load_tag,
			const reg_t rd, const uint8_t rs1, const uint8_t rs2);


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
