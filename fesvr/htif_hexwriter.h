// See LICENSE for license details.

#ifndef __HTIF_HEXWRITER_H
#define __HTIF_HEXWRITER_H

#include <map>
#include <vector>
#include <stdlib.h>
#include "memif.h"

class htif_hexwriter_t : public chunked_memif_t
{
public:
  htif_hexwriter_t(size_t b, size_t w, size_t d);

protected:
  size_t base;
  size_t width;
  size_t depth;
  std::map<addr_t,std::vector<char> > mem;

  void read_chunk(addr_t taddr, size_t len, void* dst, void* tag_dst);
  void write_chunk(addr_t taddr, size_t len, const void* src, const void* tag_src);
  void clear_chunk(addr_t taddr, size_t len, const void* tags) {}

  size_t chunk_max_size() { return width; }
  size_t chunk_align() { return width; }

  friend std::ostream& operator<< (std::ostream&, const htif_hexwriter_t&);
};

#endif // __HTIF_HEXWRITER_H
