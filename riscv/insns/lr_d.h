require_extension('A');
require_rv64;
auto res = MMU.load_int64(RS1, true);
TAG.load<int64_t>(pc_tag, res.second, TAG_DEST, TAG_RS1);
MMU.acquire_load_reservation(RS1);
WRITE_RD(res.first);
