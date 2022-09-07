require_extension(EXT_ZFHMIN);
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(sext32((int16_t)(FRS1.v[0])));
