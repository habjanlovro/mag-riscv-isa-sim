require_extension('D');
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F, TAG_RS2_F, true);
WRITE_FRD(fsgnj64(FRS1, FRS2, false, true));
