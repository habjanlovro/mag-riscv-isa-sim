// See LICENSE for license details.

#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include "memif.h"

void memif_t::read(addr_t addr, size_t len, void* bytes, void* tag_bytes)
{
  size_t align = cmemif->chunk_align();
  if (len && (addr & (align-1)))
  {
    size_t this_len = std::min(len, align - size_t(addr & (align-1)));
    uint8_t chunk[align];
    uint8_t tag_chunk[align];

    cmemif->read_chunk(addr & ~(align-1), align, chunk, tag_chunk);
    memcpy(bytes, chunk + (addr & (align-1)), this_len);
    memcpy(tag_bytes, tag_chunk + (addr & (align-1)), this_len);

    bytes = (char*)bytes + this_len;
    tag_bytes = (char*)tag_bytes + this_len;
    addr += this_len;
    len -= this_len;
  }

  if (len & (align-1))
  {
    size_t this_len = len & (align-1);
    size_t start = len - this_len;
    uint8_t chunk[align];
    uint8_t tag_chunk[align];

    cmemif->read_chunk(addr + start, align, chunk, tag_chunk);
    memcpy((char*)bytes + start, chunk, this_len);
    memcpy((char*)tag_bytes + start, tag_chunk, this_len);

    len -= this_len;
  }

  // now we're aligned
  for (size_t pos = 0; pos < len; pos += cmemif->chunk_max_size())
    cmemif->read_chunk(addr + pos, std::min(cmemif->chunk_max_size(), len - pos),
      (char*)bytes + pos, (char*)tag_bytes + pos);
}

void memif_t::write(addr_t addr, size_t len, const void* bytes, const void* tag_bytes)
{
  size_t align = cmemif->chunk_align();
  if (len && (addr & (align-1)))
  {
    size_t this_len = std::min(len, align - size_t(addr & (align-1)));
    uint8_t chunk[align];
    uint8_t tag_chunk[align];

    cmemif->read_chunk(addr & ~(align-1), align, chunk, tag_chunk);
    memcpy(chunk + (addr & (align-1)), bytes, this_len);
    memcpy(tag_chunk + (addr & (align-1)), tag_bytes, this_len);
    cmemif->write_chunk(addr & ~(align-1), align, chunk, tag_chunk);

    bytes = (char*)bytes + this_len;
    tag_bytes = (char*)tag_bytes + this_len;
    addr += this_len;
    len -= this_len;
  }

  if (len & (align-1))
  {
    size_t this_len = len & (align-1);
    size_t start = len - this_len;
    uint8_t chunk[align];
    uint8_t tag_chunk[align];

    cmemif->read_chunk(addr + start, align, chunk, tag_chunk);
    memcpy(chunk, (char*)bytes + start, this_len);
    memcpy(tag_chunk, (char*)tag_bytes + start, this_len);
    cmemif->write_chunk(addr + start, align, chunk, tag_chunk);

    len -= this_len;
  }

  size_t max_chunk = cmemif->chunk_max_size();
  for (size_t pos = 0; pos < len; pos += max_chunk) {
    cmemif->write_chunk(addr + pos, std::min(max_chunk, len - pos),
      (char*)bytes + pos, (char*)tag_bytes + pos);
  }
}

#define MEMIF_READ_FUNC \
  if(addr & (sizeof(val)-1)) \
    throw std::runtime_error("misaligned address"); \
  this->read(addr, sizeof(val), &val, &tag_val); \
  return std::make_pair(val, tag_val)

#define MEMIF_WRITE_FUNC \
  if(addr & (sizeof(val.first)-1)) \
    throw std::runtime_error("misaligned address"); \
  this->write(addr, sizeof(val.first), &(val.first), &(val.second))

std::pair<target_endian<uint8_t>, target_endian<uint8_t>> memif_t::read_uint8(addr_t addr)
{
  target_endian<uint8_t> val;
  target_endian<uint8_t> tag_val;
  MEMIF_READ_FUNC;
}

std::pair<target_endian<int8_t>, target_endian<int8_t>> memif_t::read_int8(addr_t addr)
{
  target_endian<int8_t> val;
  target_endian<int8_t> tag_val;
  MEMIF_READ_FUNC;
}

void memif_t::write_uint8(addr_t addr, std::pair<target_endian<uint8_t>, target_endian<uint8_t>> val)
{
  MEMIF_WRITE_FUNC;
}

void memif_t::write_int8(addr_t addr, std::pair<target_endian<int8_t>, target_endian<int8_t>> val)
{
  MEMIF_WRITE_FUNC;
}

std::pair<target_endian<uint16_t>, target_endian<uint16_t>> memif_t::read_uint16(addr_t addr)
{
  target_endian<uint16_t> val;
  target_endian<uint16_t> tag_val;
  MEMIF_READ_FUNC;
}

std::pair<target_endian<int16_t>, target_endian<int16_t>> memif_t::read_int16(addr_t addr)
{
  target_endian<int16_t> val;
  target_endian<int16_t> tag_val;
  MEMIF_READ_FUNC;
}

void memif_t::write_uint16(addr_t addr, std::pair<target_endian<uint16_t>, target_endian<uint16_t>> val)
{
  MEMIF_WRITE_FUNC;
}

void memif_t::write_int16(addr_t addr, std::pair<target_endian<int16_t>, target_endian<int16_t>> val)
{
  MEMIF_WRITE_FUNC;
}

std::pair<target_endian<uint32_t>, target_endian<uint32_t>> memif_t::read_uint32(addr_t addr)
{
  target_endian<uint32_t> val;
  target_endian<uint32_t> tag_val;
  MEMIF_READ_FUNC;
}

std::pair<target_endian<int32_t>, target_endian<int32_t>> memif_t::read_int32(addr_t addr)
{
  target_endian<int32_t> val;
  target_endian<int32_t> tag_val;
  MEMIF_READ_FUNC;
}

void memif_t::write_uint32(addr_t addr, std::pair<target_endian<uint32_t>, target_endian<uint32_t>> val)
{
  MEMIF_WRITE_FUNC;
}

void memif_t::write_int32(addr_t addr, std::pair<target_endian<int32_t>, target_endian<int32_t>> val)
{
  MEMIF_WRITE_FUNC;
}

std::pair<target_endian<uint64_t>, target_endian<uint64_t>> memif_t::read_uint64(addr_t addr)
{
  target_endian<uint64_t> val;
  target_endian<uint64_t> tag_val;
  MEMIF_READ_FUNC;
}

std::pair<target_endian<int64_t>, target_endian<int64_t>> memif_t::read_int64(addr_t addr)
{
  target_endian<int64_t> val;
  target_endian<int64_t> tag_val;
  MEMIF_READ_FUNC;
}

void memif_t::write_uint64(addr_t addr, std::pair<target_endian<uint64_t>, target_endian<uint64_t>> val)
{
  MEMIF_WRITE_FUNC;
}

void memif_t::write_int64(addr_t addr, std::pair<target_endian<int64_t>, target_endian<int64_t>> val)
{
  MEMIF_WRITE_FUNC;
}
