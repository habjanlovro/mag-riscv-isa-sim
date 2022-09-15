require_extension('A');
require_rv64;
reg_t addr = RS1;
auto res = MMU.load_int64(addr, true);
TAG.load<int64_t>(pc_tag, addr, res.second, TAG_DEST, TAG_RS1);
MMU.acquire_load_reservation(addr);
WRITE_RD(res.first);
