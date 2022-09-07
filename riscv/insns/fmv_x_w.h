require_extension('F');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(sext32(FRS1.v[0]));
