require_extension(EXT_ZFHMIN);
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, true);
WRITE_FRD(f32_to_f16(f32(FRS1)));
set_fp_exceptions;
