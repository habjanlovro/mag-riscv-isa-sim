require_extension('A');
auto res = MMU.load_int32(RS1, true);
TAG.load<int32_t>(pc_tag, res.second, TAG_DEST, TAG_RS1);
MMU.acquire_load_reservation(RS1);
WRITE_RD(res.first);
