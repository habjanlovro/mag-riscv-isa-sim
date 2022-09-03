require_extension('M');
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, TAG_RS2);
require_rv64;
sreg_t lhs = sext32(RS1);
sreg_t rhs = sext32(RS2);
if(rhs == 0)
  WRITE_RD(lhs);
else
  WRITE_RD(sext32(lhs % rhs));
