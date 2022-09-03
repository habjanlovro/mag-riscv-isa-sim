require_extension('M');
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, TAG_RS2);
require_rv64;
reg_t lhs = zext32(RS1);
reg_t rhs = zext32(RS2);
if(rhs == 0)
  WRITE_RD(UINT64_MAX);
else
  WRITE_RD(sext32(lhs / rhs));
