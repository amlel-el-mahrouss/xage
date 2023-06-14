/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Ar.cpp
 *			Purpose: Xplicit Archive Format
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Ar.h"

static bool
ar_internal_write(struct ar_context* ctx, const unsigned char* bytes, size_t sz) 
{
    XPLICIT_ASSERT(ctx);

    if (!ctx)
        return false;

    if (ctx && bytes) 
    {
        size_t index = 0;
        
        while (index < sz) 
        {
            fputc(bytes[index], ctx->fp);
            ++index;
        }

        return true;
    }

    return false;
}

static char
ar_internal_read(struct ar_context* ctx, fpos_t pos) 
{
    XPLICIT_ASSERT(ctx);

    if (!ctx)
        return AR_END_OF_RECORD;

    if (fsetpos(ctx->fp, &pos) != -1)
        return fgetc(ctx->fp);

    return 0;
}

size_t 
ar_fast_strlen(const unsigned char* bytes, size_t sz)
{
    if (!bytes) return 0;
    if (*bytes == AR_END_OF_RECORD) return 0;

    size_t index = 0;
    while (bytes[index] != AR_END_OF_RECORD) ++index;
    return index;
}

void
ar_update_epoch(struct ar_context* ctx) {
    time_t epoch = time(0); // from 1970

    if (ctx->header.create_epoch == 0)
        ctx->header.create_epoch = epoch;
    else
        ctx->header.edit_epoch = epoch;

    fpos_t pos = 0;
    fsetpos(ctx->fp, &pos);

    for (int index = 0; index < AR_HDR_SZ; ++index) {
        char* byte = (char*)&ctx->header;
        fputc(byte[index], ctx->fp);
    }

    fflush(ctx->fp);
}

struct ar_context*
ar_new(const char* path, const char* res) {
    ar_file_t* fp = NULL;

    xplicit_log("Assert: fopen_s...");
    xplicit_log("Begin of record...");

    assert(fopen_s(&fp, path, res) == 0);

    if (fp && (strcmp((const char*)res, AR_FILE_RESTRICT_W) == 0)) {
        struct ar_context* ctx = (struct ar_context*)malloc(sizeof(struct ar_context));

        if (ctx) {
            ctx->fp = fp;
            XPLICIT_ASSERT(ctx->fp);

            time_t epoch = time(0);

            ctx->header.create_epoch = epoch;
            ctx->header.edit_epoch = ctx->header.create_epoch;
            ctx->header.magic = AR_MAGIC;
            ctx->header.size = AR_HDR_SZ;

            for (int index = 0; index < AR_HDR_SZ; ++index) {
                char* byte = (char*)&ctx->header;
                fputc(byte[index], fp);
            }

            fflush(fp);

            return ctx;
        }

        fclose(fp);
    } else if (fp && (strcmp((const char*)res, AR_FILE_RESTRICT_R) == 0)) {
        struct ar_context* ctx = (struct ar_context*)malloc(sizeof(struct ar_context));
        if (!ctx) {
            fclose(fp);
            xplicit_log("Warning! could not allocate context, probably out of memory blocks...\n");

            return NULL;
        }

        fpos_t pos = 0;
        fsetpos(fp, &pos);

        for (int index = 0; index < AR_HDR_SZ; ++index) {
            ((char*)&ctx->header)[index] = fgetc(fp);
            assert(((char*)&ctx->header)[index] != -1);
        }

        if (ctx->header.magic != AR_MAGIC) {
            free(ctx);
            fclose(fp);

            xplicit_log("Warning! Bad magic on archive, closing file pointer.");

            return NULL;
        } else {

            ctx->fp = fp;
            return ctx;
        }
    }

    return NULL;
}

size_t 
ar_len(const unsigned char* bytes, size_t len) {
    if (!bytes) return 0;

    size_t index = 0;
    while (len > index) {
        ++index;
    }

    return index;
}

void
ar_update_header_info(struct ar_context* ctx, const unsigned char* bytes, size_t len) {
    ++ctx->header.extension_table.count;
    if (bytes) ctx->header.extension_table.size += ar_len(bytes, len);

    fpos_t pos = 0;
    fsetpos(ctx->fp, &pos);
    for (int index = 0; index < AR_HDR_SZ; ++index) {
        char* byte = (char*)&ctx->header;
        fputc(byte[index], ctx->fp);
    }

    fflush(ctx->fp);
}

bool
ar_write(struct ar_context* ctx, const unsigned char* bytes, fpos_t off, size_t sz) {
    if (bytes && ctx) {
        ar_update_header_info(ctx, bytes, sz);
        fsetpos(ctx->fp, &off);

        return ar_internal_write(ctx, bytes, sz);
    }

    return false;
}


bool
ar_read(struct ar_context* ctx, unsigned char* bytes, size_t off, size_t sz) {
    if (ctx && bytes) {
        size_t idx = 0;
        while (idx < sz) {
            bytes[idx] = ar_internal_read(ctx, off + idx);
            ++idx;
        }

        return true;
    }

    return false;
}


bool
ar_flush(struct ar_context* ctx) { return ctx && (fflush(ctx->fp) == 0); }

bool
ar_close(struct ar_context* ctx) {
    if (ctx && ctx->fp) 
    {
        fwrite(&ctx->header, AR_HDR_SZ, SEEK_SET, ctx->fp);

        if (ar_flush(ctx)) 
        {
            fclose(ctx->fp);
            free(ctx);

#ifndef _NDEBUG
            xplicit_log("[AR] Closed archive context!");
#endif

            return true;
        }
    }

    return false;
}

namespace Xplicit
{
    ArchiveManager::ArchiveManager(const char* path, const char* rest)
        : m_ar(ar_new(path, rest)), m_good(true), m_off(0)
    {
#ifdef XPLICIT_DEBUG
        std::string message;
        message += "Class ArchiveManager, Epoch: ";
        message += std::to_string(xplicit_get_epoch());

        XPLICIT_INFO(message);
#endif
    }

    ArchiveManager::~ArchiveManager()
    {
        if (m_ar)
            ar_close(m_ar);

#ifdef XPLICIT_DEBUG
        std::string message;
        message += "~ArchiveManager, Epoch: ";
        message += std::to_string(xplicit_get_epoch());

        XPLICIT_INFO(message);
#endif
    }

    long ArchiveManager::size() noexcept
    {
        if (!m_ar)
            return -1;

        auto fp = m_ar->fp;
        XPLICIT_ASSERT(fp);

        fseek(fp, 0, SEEK_END);
        return ftell(fp);
    }

    void ArchiveManager::seek(const size_t seek) noexcept
    {
        m_off = seek;
    }

    size_t ArchiveManager::tell() noexcept
    {
        if (!m_ar)
            return ArchiveManager::npos;

        return ftell(m_ar->fp);
    }

    bool ArchiveManager::good() const noexcept { return m_good; }

    ArchiveManager& ArchiveManager::operator<<(const unsigned char* bytes)
    {
        if (!bytes)
            return *this;

        auto len = ar_len(bytes, 1024);

        ar_write(m_ar, bytes, m_off, len);

        ++m_off;

        m_good = (ferror(m_ar->fp) == 0);

        return *this;
    }

    ArchiveManager& ArchiveManager::operator>>(unsigned char* bytes)
    {
        if (!bytes)
            return *this;

        auto len = ar_len(bytes, 1024);

        ar_read(m_ar, bytes, m_off, len);
        ++m_off;

        m_good = (ferror(m_ar->fp) == 0);

        return *this;
    }
}