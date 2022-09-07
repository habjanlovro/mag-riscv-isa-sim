require_extension('F');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(f32_add(f32(FRS1), f32(FRS2)));
set_fp_exceptions;
