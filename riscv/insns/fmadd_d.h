require_extension('D');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, TAG_RS3_F, true);
WRITE_FRD(f64_mulAdd(f64(FRS1), f64(FRS2), f64(FRS3)));
set_fp_exceptions;
