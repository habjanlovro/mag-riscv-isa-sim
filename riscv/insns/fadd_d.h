require_extension('D');
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(f64_add(f64(FRS1), f64(FRS2)));
set_fp_exceptions;
