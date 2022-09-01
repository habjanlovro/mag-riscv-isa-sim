#include "tag_manager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "mmu.h"
#include "processor.h"

tag_memory_t::tag_memory_t(
		const std::string& policy_file,
		const std::string& tag_file) : tag_fd(-1) {
	std::ifstream policy_file_stream(policy_file);
	if (!policy_file_stream.is_open()) {
		std::ostringstream oss;
		oss << "Failed to open tag file '" << policy_file << ": " << strerror(errno);
		throw std::runtime_error(oss.str());
	}

	int line_num = 1;

	std::string line;
	std::string temp;
	std::getline(policy_file_stream, line);
	std::stringstream line_s(line);
	int n, num_pgs;
	line_s >> n >> num_pgs;
	if (n <= 0) {
		throw std::runtime_error("Wrong fromat of the D2CC policy!");
	}
	policy = std::vector<std::vector<uint8_t>>(
		256, std::vector<uint8_t>(256, TAG_INVALID));
	names = std::map<int, std::string>();

	for (int i = 0; i < n; i++) {
		line_num++;
		std::getline(policy_file_stream, line);
		std::stringstream line_stream(line);
		line_stream >> names[i];
		for (int j = 0; j < n; j++) {
			std::string item;
			line_stream >> item;
			policy[i][j] = std::stoul(item);
		}
	}

	// perimiter guards
	for (int i = 0; i < num_pgs; i++) {
		line_num++;

		std::string name, fd_s, tag_s;
		size_t fd;
		uint8_t tag;

		std::getline(policy_file_stream, line);
		std::stringstream line_stream(line);
		line_stream >> name >> fd_s >> tag_s;

		fd = std::stoul(fd_s);
		tag = std::stoul(tag_s);

		tag_pg_t pg(name, fd, tag);
		if (perimiter_guards.find(fd) == perimiter_guards.end()) {
			perimiter_guards[fd] = pg;
		} else {
			std::cerr << "Skipping duplicate definition of perimiter guard for "
				<< "file descriptor '" << (int) fd << "'" << std::endl;
		}
	}
	while (std::getline(policy_file_stream, line)) {
		line_num++;
		std::vector<std::string> tokens;
		std::stringstream line_stream(line);
		for (std::string token; std::getline(line_stream, token, ',');) {
			tokens.push_back(token);
		}

		if (tokens.size() != 3) {
			std::ostringstream oss;
			oss << "Line " << line_num << ": Failed to read!";
			throw std::runtime_error(oss.str());
		}
		reg_t addr = std::stoul(tokens[0], nullptr, 16);
		size_t size = std::stoul(tokens[1]);
		uint8_t tag = std::stoul(tokens[2]);

		tag_region_data_t t = {addr, size, tag };

		tagged_data.push_back(t);
	}

	tag_fd = open(tag_file.c_str(), O_RDONLY);
	if (tag_fd < 0) {
		throw std::runtime_error("Failed to open the tag file!");
	}
	bus = new bus_t();
	mem_t *m = new mem_t(PGSIZE);
	bus->add_device(0, m);
}

tag_memory_t::~tag_memory_t() {
	if (tag_fd < 0) {
		if (close(tag_fd) < 0) {
			std::cerr << "Failed to close file descriptor! Error: " << strerror(errno) << std::endl;
		}
	}
	delete bus;
}

void tag_memory_t::add_mem(const reg_t addr, abstract_device_t *device) {
	abstract_device_t *duplicated;
	if (auto d = dynamic_cast<rom_device_t*>(device)) {
		duplicated = new rom_device_t(*d);
	} else if (auto d = dynamic_cast<mem_t*>(device)) {
		duplicated = new mem_t(*d);
	} else if (auto d = dynamic_cast<clint_t*>(device)) {
		duplicated = new clint_t(*d);
	} else {
		std::cerr << "Failed add_mem" << std::endl;
		std::exit(5);
	}
	std::cerr << "adding " << std::hex << addr << std::endl;
	bus->add_device(addr, duplicated);
}


static bool paddr_ok(reg_t addr) {
	return (addr >> MAX_PADDR_BITS) == 0;
}

bool tag_memory_t::mmio_load(reg_t addr, size_t len, uint8_t* bytes) {
	if (addr + len < addr || !paddr_ok(addr + len - 1))
		return false;
	return bus->load(addr, len, bytes);
}

bool tag_memory_t::mmio_store(reg_t addr, size_t len, const uint8_t* bytes) {
	if (addr + len < addr || !paddr_ok(addr + len - 1))
		return false;
	return bus->store(addr, len, bytes);
}

char* tag_memory_t::addr_to_mem(reg_t addr) {
	if (!paddr_ok(addr)) {
		return NULL;
	}
	auto desc = bus->find_device(addr);
	if (auto mem = dynamic_cast<mem_t*>(desc.second)) {
		if (addr - desc.first < mem->size()) {
			return mem->contents(addr - desc.first);
		}
	}
	return NULL;
}

const char* tag_memory_t::get_symbol(uint64_t addr) {
	return "";
}

void tag_memory_t::copy_tag_mem(reg_t pbuf, reg_t len, reg_t off) {
	std::vector<uint8_t> buf(len);
	ssize_t ret = pread(tag_fd, buf.data(), len, off);
	for (auto& c : buf) {
		std::cerr << (int) c;
	}
	std::cerr << std::endl;
	std::cerr << "PREAD " << ret << std::endl;
	if (ret > 0) {
		bus->store(pbuf, len, buf.data());
	}
}


tag_manager_t::tag_manager_t() : enabled(false) {}

tag_manager_t::tag_manager_t(tag_memory_t *m, processor_t *p)
		: memory(m), processor(p) {
	if (memory && p) {
		enabled = true;
		mmu = new mmu_t(m, p);
	} else {
		enabled = false;
		mmu = nullptr;
	}
}

tag_manager_t::~tag_manager_t() {
	if (mmu) {
		delete mmu;
	}
}

static uint8_t load_tag(mmu_t *mmu, reg_t addr) {
	return 0;
}

void tag_manager_t::propagate(const reg_t pc, const reg_t rd) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(2)! lca(PC, @PC) invalid tag!");
		}
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::propagate(const reg_t pc, const reg_t rd, const uint8_t rs) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		new_tag = memory->lca(new_tag, rs);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(3)! lca(PC, RS) invalid tag!");
		}
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::propagate(const reg_t pc, const reg_t rd,
		const uint8_t rs1, const uint8_t rs2) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		new_tag = memory->lca(new_tag, rs1);
		new_tag = memory->lca(new_tag, rs2);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(4)! lca(PC, @PC, RS1, RS2) invalid tag!");
		}
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::propagate_branch(const reg_t pc, const reg_t jmp_addr,
		const uint8_t rs1, const uint8_t rs2) {
	if (enabled) {
		uint8_t new_tag = memory->lca(
			load_tag(mmu, pc), load_tag(mmu, jmp_addr));
		new_tag = memory->lca(pc_tag, new_tag);
		new_tag = memory->lca(rs1, new_tag);
		new_tag = memory->lca(rs2, new_tag);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate_branch()! lca(@PC, @JMP, PC, RS1, RS2) invalid tag!");
		}
		pc_tag = new_tag;
	}
}


void tag_manager_t::load(const reg_t pc, reg_t rd, uint8_t rs, reg_t addr,
		size_t num_bytes) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		new_tag = memory->lca(new_tag, rs);
		for (size_t offset = 0; offset < num_bytes; offset++) {
			memory->lca(new_tag, load_tag(mmu, addr + offset));
		}
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error load()! lca(PC, RS, *mem) invalid tag!");
		}
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::store(const reg_t pc, const uint8_t rs1, const uint8_t rs2,
		reg_t addr, size_t num_bytes) {
	if (enabled) {
		// if (addr > 0xffffffff) {
		// 	// std::cerr << "address > 32 bit" << std::endl;
		// 	return;
		// }
		// if (pc > 0xffffffff) {
		// 	return;
		// }
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		new_tag = memory->lca(new_tag, rs1);
		new_tag = memory->lca(new_tag, rs2);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error store()! lca(PC, RS1, RS2) invalid tag!");
		}
		for (size_t offset = 0; offset < num_bytes; offset++) {
		}
	}
}

void tag_manager_t::jump(const reg_t pc, const reg_t rd, const reg_t addr) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		uint8_t addr_tag = load_tag(mmu, addr);
		new_tag = memory->lca(new_tag, addr_tag);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Erorr jump(2)! lca(@addr, PC) invalid tag!");
		}
		pc_tag = new_tag;
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::jump(const reg_t pc, const reg_t rd, const uint8_t rs,
		const reg_t addr) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, load_tag(mmu, pc));
		uint8_t addr_tag = load_tag(mmu, addr);
		new_tag = memory->lca(new_tag, load_tag(mmu, addr));
		new_tag = memory->lca(new_tag, rs);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error jump(3)! lca(@addr, PC, RS) invalid tag!");
		}
		pc_tag = new_tag;
		XPR_tags.write(rd, new_tag);
	}
}


void tag_manager_t::print() {
	for (size_t i = 0; i < NXPR; i++) {
		std::cout << "Reg " << i << ": " << (int) XPR_tags[i] << std::endl;
	}
}
