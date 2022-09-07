require_extension(EXT_ZFH);
require_fp;
softfloat_roundingMode = RM;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(sext32(f16_to_ui32(f16(FRS1), RM, true)));
set_fp_exceptions;
