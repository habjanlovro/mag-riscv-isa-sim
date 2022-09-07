require_extension('F');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F);
WRITE_RD(f32_eq(f32(FRS1), f32(FRS2)));
set_fp_exceptions;
