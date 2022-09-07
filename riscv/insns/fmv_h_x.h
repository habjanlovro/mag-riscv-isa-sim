require_extension(EXT_ZFHMIN);
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1, true);
WRITE_FRD(f16(RS1));
