/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Ar.h
 *			Purpose: XPX Archive Format
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

constexpr auto AR_MAGIC = 0xBADF00D;

#define AR_EXTENSION ".dir"
#define AR_MAKE_EXTENSION(NAME) NAME AR_EXTENSION

struct ar_extension_table;
struct ar_header;

typedef enum 
{
    AR_FLAG_NONE = 0x0, //! no flags
    AR_FLAG_PASSWORD = 0x7c, //! this archive has a password (AR Extension)
} AR_FLAGS;

XPX_PACKED_STRUCT(struct ar_extension_table {
    uint64_t start;
    uint64_t count;
    uint64_t size;
    uint32_t flags;
})

/*!
  !  Stripped down tar header
  !*/

XPX_PACKED_STRUCT(struct ar_header {
    uint64_t magic;
    time_t edit_epoch;
    time_t create_epoch;
    size_t size;
    struct ar_extension_table extension_table;
})

#define AR_HDR_SZ (sizeof(struct ar_header))

typedef enum {
    AR_COMPRESSION_TYPE_NONE = 0x00, //! Uncompressed scene.
    AR_COMPRESSION_TYPE_ZLIB = 0x10, //! Zlib compression.
    AR_COMPRESSION_TYPE_COUNT = 2,
} AR_COMPRESSION_TYPE;

typedef FILE ar_file_t;

XPX_PACKED_STRUCT(struct ar_context{
    ar_file_t * fp;
    struct ar_header header;
})

/* We provide our own way to tell in a file what is the end of a record. */

#define AR_FILE_RESTRICT_W ("wb")
#define AR_FILE_RESTRICT_R ("rb")

#define AR_END_OF_RECORD ('\0')

XPLICIT_API size_t
ar_fast_strlen(const unsigned char* deflated, size_t sz);

XPLICIT_API void
ar_update_epoch(struct ar_context* ctx);

XPLICIT_API struct ar_context*
ar_new(const char* path, const char* res);

XPLICIT_API size_t
ar_len(const unsigned char* bytes, size_t sz);

XPLICIT_API void
ar_update_header_info(struct ar_context* ctx, const unsigned char* bytes, size_t sz);

XPLICIT_API bool
ar_write(struct ar_context* ctx, const unsigned char* bytes, fpos_t off, size_t sz);

XPLICIT_API bool
ar_read(struct ar_context* ctx, unsigned char* bytes, size_t off, size_t sz);

XPLICIT_API bool
ar_flush(struct ar_context* ctx);

XPLICIT_API bool
ar_close(struct ar_context* ctx);

namespace XPX 
{
    class XPLICIT_API ArchiveManager final 
    {
    public:
        ArchiveManager(const char* path, const char* rest);
        ~ArchiveManager();

    public:
        XPLICIT_COPY_DELETE(ArchiveManager);

    public:
        ArchiveManager& operator<<(const unsigned char* bytes);
        ArchiveManager& operator>>(unsigned char* bytes);

	public:
		void seek(const size_t seek = 0) noexcept;
		void set(const std::size_t& sz) noexcept;
		bool good() const noexcept;
		size_t tell() noexcept;
        long size() noexcept;

        static constexpr const std::size_t npos = ~0UL;

    private:
        std::size_t mSz;
        ar_context* mAr;
        size_t mOff;
        bool mGood;

    };

}
