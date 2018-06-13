/**
 * @file compressor.h
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2017-2018 TileDB, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * This defines the tiledb CompressorType enum that maps to tiledb_compressor_t
 * C-api enum.
 */

#ifndef TILEDB_COMPRESSOR_H
#define TILEDB_COMPRESSOR_H

#include "tiledb/sm/misc/constants.h"

#include <cassert>

namespace tiledb {
namespace sm {

/** Defines the compressor type. */
enum class Compressor : char {
#define TILEDB_COMPRESSOR_ENUM(id) id
#include "tiledb/sm/c_api/tiledb_enum.h"
#undef TILEDB_COMPRESSOR_ENUM
};

/** Returns the string representation of the input compressor. */
inline const std::string& compressor_str(Compressor type) {
  switch (type) {
    case Compressor::NO_COMPRESSION:
      return constants::no_compression_str;
    case Compressor::GZIP:
      return constants::gzip_str;
    case Compressor::ZSTD:
      return constants::zstd_str;
    case Compressor::LZ4:
      return constants::lz4_str;
    case Compressor::BLOSC_LZ:
      return constants::blosc_lz_str;
    case Compressor::BLOSC_LZ4:
      return constants::blosc_lz4_str;
    case Compressor::BLOSC_LZ4HC:
      return constants::blosc_lz4hc_str;
    case Compressor::BLOSC_SNAPPY:
      return constants::blosc_snappy_str;
    case Compressor::BLOSC_ZLIB:
      return constants::blosc_zlib_str;
    case Compressor::BLOSC_ZSTD:
      return constants::blosc_zstd_str;
    case Compressor::RLE:
      return constants::rle_str;
    case Compressor::BZIP2:
      return constants::bzip2_str;
    case Compressor::DOUBLE_DELTA:
      return constants::double_delta_str;
    default:
      assert(0);
      return constants::empty_str;
  }
}

/** Returns the compressor based on the string representation. */
inline Compressor compressor_enum(const std::string& compressor_type_str) {
  if (compressor_type_str == constants::no_compression_str)
    return Compressor::NO_COMPRESSION;
  else if (compressor_type_str == constants::gzip_str)
    return Compressor::GZIP;
  else if (compressor_type_str == constants::zstd_str)
    return Compressor::ZSTD;
  else if (compressor_type_str == constants::lz4_str)
    return Compressor::LZ4;
  else if (compressor_type_str == constants::blosc_lz_str)
    return Compressor::BLOSC_LZ;
  else if (compressor_type_str == constants::blosc_lz4_str)
    return Compressor::BLOSC_LZ4;
  else if (compressor_type_str == constants::blosc_lz4hc_str)
    return Compressor::BLOSC_LZ4HC;
  else if (compressor_type_str == constants::blosc_snappy_str)
    return Compressor::BLOSC_SNAPPY;
  else if (compressor_type_str == constants::blosc_zlib_str)
    return Compressor::BLOSC_ZLIB;
  else if (compressor_type_str == constants::blosc_zstd_str)
    return Compressor::BLOSC_ZSTD;
  else if (compressor_type_str == constants::rle_str)
    return Compressor::RLE;
  else if (compressor_type_str == constants::bzip2_str)
    return Compressor::BZIP2;
  else if (compressor_type_str == constants::double_delta_str)
    return Compressor::DOUBLE_DELTA;
  else {
    assert(0);
    return Compressor::NO_COMPRESSION;
  }
}
}  // namespace sm
}  // namespace tiledb

#endif  // TILEDB_COMPRESSOR_H
