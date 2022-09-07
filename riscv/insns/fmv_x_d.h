require_extension('D');
require_rv64;
require_fp;
TAG.propagate(pc_tag, TAG_DEST, TAG_RS1_F);
WRITE_RD(FRS1.v[0]);
