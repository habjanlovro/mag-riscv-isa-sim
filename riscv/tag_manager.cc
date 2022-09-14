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


tag_memory_t::tag_memory_t() : enabled(false), tag_fd(-1) {
	bus = new bus_t;
}

static std::string parse_name(std::stringstream& ss) {
	std::string r;
	char c;
	while (ss.get(c) && c != ' ') {
		r += c;
	}
	if (r.size() == 0) {
		throw std::runtime_error("Missing name for perimiter guard!");
	}
	return r;
}

static std::string parse_file(std::stringstream& ss) {
	std::string r;
	char c;
	do {
		ss.get(c);
	} while(!ss.eof() && c != '"');
	while (ss.get(c) && c != '"') {
		r += c;
	}
	if (r.size() == 0) {
		throw std::runtime_error("Missing file for perimiter guard!");
	}
	return r;
}

static uint8_t parse_tag(std::stringstream& ss) {
	std::string r;
	char c;
	while (ss.get(c)) {
		r += c;
	}
	return (uint8_t) std::stoul(r);
}

tag_memory_t::tag_memory_t(
		const std::string& policy_file,
		const std::string& tag_file) : enabled(true), tag_fd(-1) {
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

		std::getline(policy_file_stream, line);
		std::stringstream line_stream(line);

		std::string name = parse_name(line_stream);
		std::string file = parse_file(line_stream);
		uint8_t tag = parse_tag(line_stream);

		tag_pg_t pg(name, file, tag);
		perimiter_guards.push_back(pg);
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
}

tag_memory_t::~tag_memory_t() {
	if (tag_fd > 0) {
		if (close(tag_fd) < 0) {
			std::cerr << "Failed to close file descriptor! Error: "
				<< strerror(errno) << std::endl;
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
		std::exit(1);
	}
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

std::vector<uint8_t> tag_memory_t::copy_tag_mem(reg_t pbuf, reg_t len, reg_t off) {
	if (!enabled) {
		return std::vector<uint8_t>(len, 0);
	}
	std::vector<uint8_t> buf(len);
	ssize_t ret = pread(tag_fd, buf.data(), len, off);
	return buf;
}

std::vector<uint8_t> tag_memory_t::pg_in(int fd, reg_t pbuf, reg_t len) {
	if (!enabled) {
		return std::vector<uint8_t>(len, 0);
	}
	try {
		auto pg = active_perimiter_guards.at(fd);
		return std::vector<uint8_t>(len, pg.tag);
	} catch (...) {
		return std::vector<uint8_t>(len, 0);
	}
}

bool tag_memory_t::pg_out(int fd, reg_t addr, const std::vector<uint8_t>& data) {
	if (!enabled) {
		return true;
	}
	try {
		auto pg = active_perimiter_guards.at(fd);
		for (auto& tag : data) {
			auto check = lca(pg.tag, tag);
			if (!is_descendant(tag, pg.tag)) {
				return false;
			}
		}
	} catch (...) {}
	return true;
}

void tag_memory_t::register_fd(const std::string& file, int fd) {
	for (auto& pg : perimiter_guards) {
		if (pg.file == file) {
			active_perimiter_guards[fd] = pg;
			break;
		}
	}
}

void tag_memory_t::unregister_fd(int fd) {
	active_perimiter_guards.erase(fd);
}

uint64_t tag_memory_t::check(uint64_t lhs, uint64_t rhs) {
	if (!enabled) {
		return 0;
	}
	uint64_t tag = 0;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 56), (uint8_t)(rhs >> 56)) << 56;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 48), (uint8_t) (rhs >> 48)) << 48;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 40), (uint8_t) (rhs >> 40)) << 40;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 32), (uint8_t) (rhs >> 32)) << 32;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 24), (uint8_t) (rhs >> 24)) << 24;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 16), (uint8_t) (rhs >> 16)) << 16;
	tag |= (uint64_t) lca((uint8_t) (lhs >> 8), (uint8_t) (rhs >> 8)) << 8;
	tag |= (uint64_t) lca((uint8_t) lhs, (uint8_t) rhs);

	return tag;
}


tag_manager_t::tag_manager_t() : enabled(false) {}

tag_manager_t::tag_manager_t(tag_memory_t *m, processor_t *p)
		: memory(m), processor(p) {
	if (memory && memory->enabled && p) {
		enabled = true;
		mmu = p->get_mmu();
	} else {
		enabled = false;
		mmu = nullptr;
	}
}

tag_manager_t::~tag_manager_t() {

}

void tag_manager_t::propagate(const uint8_t pc_addr_tag, const reg_t rd,
		bool f) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(2)! lca(PC, @PC) invalid tag!");
		}
		if (f) {
			FPR_tags.write(rd, new_tag);
		} else {
			XPR_tags.write(rd, new_tag);
		}
	}
}

void tag_manager_t::propagate(const uint8_t pc_addr_tag, const reg_t rd,
		const uint8_t rs, bool f) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(3)! lca(PC, RS) invalid tag!");
		}
		if (f) {
			FPR_tags.write(rd, new_tag);
		} else {
			XPR_tags.write(rd, new_tag);
		}
	}
}

void tag_manager_t::propagate(const uint8_t pc_addr_tag, const reg_t rd,
		const uint8_t rs1, const uint8_t rs2, bool f) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs1);
		new_tag = memory->lca(new_tag, rs2);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(4)! lca(PC, @PC, RS1, RS2) invalid tag!");
		}
		if (f) {
			FPR_tags.write(rd, new_tag);
		} else {
			XPR_tags.write(rd, new_tag);
		}
	}
}

void tag_manager_t::propagate(const uint8_t pc_addr_tag, const reg_t rd,
		const uint8_t rs1, const uint8_t rs2, const uint8_t rs3, bool f) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs1);
		new_tag = memory->lca(new_tag, rs2);
		new_tag = memory->lca(new_tag, rs3);
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate(4)! lca(PC, @PC, RS1, RS2) invalid tag!");
		}
		if (f) {
			FPR_tags.write(rd, new_tag);
		} else {
			XPR_tags.write(rd, new_tag);
		}
	}
}

void tag_manager_t::propagate_branch(const uint8_t pc_addr_tag,
		const reg_t jmp_addr) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
#ifdef TAG_JUMP_CHECK
		uint8_t addr_tag = mmu->load_insn(jmp_addr).tag;
		new_tag = memory->lca(new_tag, addr_tag);
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate_branch()! lca(@PC, @JMP, PC, RS1, RS2) invalid tag!");
		}
		pc_tag = new_tag;
	}
};

void tag_manager_t::propagate_branch(const uint8_t pc_addr_tag,
		const reg_t jmp_addr, const uint8_t rs1) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(rs1, new_tag);
#ifdef TAG_JUMP_CHECK
		uint8_t addr_tag = mmu->load_insn(jmp_addr).tag;
		new_tag = memory->lca(new_tag, addr_tag);
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate_branch()! lca(@PC, @JMP, PC, RS1, RS2) invalid tag!");
		}
		pc_tag = new_tag;
	}
}

void tag_manager_t::propagate_branch(const uint8_t pc_addr_tag,
		const reg_t jmp_addr, const uint8_t rs1, const uint8_t rs2) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(rs1, new_tag);
		new_tag = memory->lca(rs2, new_tag);
#ifdef TAG_JUMP_CHECK
		uint8_t addr_tag = mmu->load_insn(jmp_addr).tag;
		new_tag = memory->lca(new_tag, addr_tag);
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error propagate_branch()! lca(@PC, @JMP, PC, RS1, RS2) invalid tag!");
		}
		pc_tag = new_tag;
	}
}

void tag_manager_t::jump(const uint8_t pc_addr_tag, const reg_t jmp_addr,
		const reg_t rd) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
#ifdef TAG_JUMP_CHECK
		uint8_t addr_tag = mmu->load_insn(jmp_addr).tag;
		new_tag = memory->lca(new_tag, addr_tag);
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error jump(2)! lca(PC, jump_addr) invalid tag!");
		}
		pc_tag = new_tag;
		XPR_tags.write(rd, new_tag);
	}
}

void tag_manager_t::jump(const uint8_t pc_addr_tag, const reg_t jmp_addr,
		const reg_t rd, const uint8_t rs) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs);
#ifdef TAG_JUMP_CHECK
		uint8_t addr_tag = mmu->load_insn(jmp_addr).tag;
		// TODO do we stop if we jump from higher to lower priority?
		new_tag = memory->lca(new_tag, addr_tag);
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error jump(3)! lca(PC, jump_addr, RS) invalid tag!");
		}
		pc_tag = new_tag;
		XPR_tags.write(rd, new_tag);
	}
}

template<typename T>
void tag_manager_t::load(const uint8_t pc_addr_tag, T tag_bytes, reg_t rd,
		uint8_t rs, bool f) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs);
		for (size_t i = 0; i < sizeof tag_bytes; i++) {
			uint8_t tag = tag_bytes >> (8 * i);
			new_tag = memory->lca(new_tag, tag);
		}
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error load()! lca(PC, RS, *mem) invalid tag!");
		}
		if (f) {
			FPR_tags.write(rd, new_tag);
		} else {
			XPR_tags.write(rd, new_tag);
		}
	}
}

template void tag_manager_t::load<uint8_t>(const uint8_t pc_addr_tag, uint8_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<int8_t>(const uint8_t pc_addr_tag, int8_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<uint16_t>(const uint8_t pc_addr_tag, uint16_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<int16_t>(const uint8_t pc_addr_tag, int16_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<uint32_t>(const uint8_t pc_addr_tag, uint32_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<int32_t>(const uint8_t pc_addr_tag, int32_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<uint64_t>(const uint8_t pc_addr_tag, uint64_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<int64_t>(const uint8_t pc_addr_tag, int64_t tag_bytes, reg_t rd, uint8_t rs, bool f);
template void tag_manager_t::load<uint128_t>(const uint8_t pc_addr_tag, uint128_t tag_bytes, reg_t rd, uint8_t rs, bool f);


template<typename T>
T tag_manager_t::store(const uint8_t pc_addr_tag, const reg_t store_addr,
		const uint8_t rs1, const uint8_t rs2) {
	if (enabled) {
		uint8_t new_tag = memory->lca(pc_tag, pc_addr_tag);
		new_tag = memory->lca(new_tag, rs1);
		new_tag = memory->lca(new_tag, rs2);
#ifdef TAG_MEM_CHECK
		T load_curr_tag;
		switch (sizeof load_curr_tag) {
			case 1:
				load_curr_tag = mmu->load_uint8(store_addr).second;
				break;
			case 2:
				load_curr_tag = mmu->load_uint16(store_addr).second;
				break;
			case 4:
				load_curr_tag = mmu->load_uint32(store_addr).second;
				break;
			case 8:
				load_curr_tag = mmu->load_uint64(store_addr).second;
				break;
			case 16:
				auto l_one = mmu->load_uint64(store_addr).second;
				auto l_two = mmu->load_uint64(store_addr + 8).second;
				load_curr_tag = ((uint128_t)l_one << 64) + l_two;
				break;
		}
		uint8_t tag_curr = 0;
		for (size_t i = 0; i < sizeof load_curr_tag; i++) {
			uint8_t tag = load_curr_tag >> (8 * i);
			memory->lca(tag_curr, tag);
		}
		// TODO do we throw or raise the tag status (similiar to jump)?
		if (!memory->is_descendant(tag_curr, new_tag)) {
			throw std::runtime_error("Error store()! lca(PC, RS1, RS2) invalid tag for MEM location!");
		}
#endif
		if (new_tag == TAG_INVALID) {
			throw std::runtime_error("Error store()! lca(PC, RS1, RS2) invalid tag!");
		}
		T result = 0;
		for (size_t i = 0; i < sizeof result; i++) {
			result |= (uint64_t)new_tag << (8 * i);
		}
		return result;
	}
	return 0;
}

template uint8_t tag_manager_t::store<uint8_t>(const uint8_t pc_addr_tag, const reg_t store_addr, const uint8_t rs1, const uint8_t rs2);
template uint16_t tag_manager_t::store<uint16_t>(const uint8_t pc_addr_tag, const reg_t store_addr, const uint8_t rs1, const uint8_t rs2);
template uint32_t tag_manager_t::store<uint32_t>(const uint8_t pc_addr_tag, const reg_t store_addr, const uint8_t rs1, const uint8_t rs2);
template uint64_t tag_manager_t::store<uint64_t>(const uint8_t pc_addr_tag, const reg_t store_addr, const uint8_t rs1, const uint8_t rs2);
template uint128_t tag_manager_t::store<uint128_t>(const uint8_t pc_addr_tag, const reg_t store_addr, const uint8_t rs1, const uint8_t rs2);

void tag_manager_t::print() {
	for (size_t i = 0; i < NXPR; i++) {
		std::cout << "Reg " << i << ": " << (int) XPR_tags[i] << " "
			<< (int) FPR_tags[i] << std::endl;
	}
	std::cout << "PC tag: " << (int) pc_tag << std::endl;
}

inline bool tag_memory_t::is_descendant(const uint8_t lhs, const uint8_t rhs) {
	return lca(lhs, rhs) != TAG_INVALID && lca(lhs, rhs) == rhs;
}
