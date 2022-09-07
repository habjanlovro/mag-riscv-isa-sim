require_extension('F');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(fsgnj32(FRS1, FRS2, false, false));
