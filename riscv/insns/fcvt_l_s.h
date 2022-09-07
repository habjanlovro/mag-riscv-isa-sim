require_extension('F');
require_rv64;
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(f32_to_i64(f32(FRS1), RM, true));
set_fp_exceptions;
