/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef _AUTH_HMAC_H
#define _AUTH_HMAC_H

#include "Common.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>

class BigNumber;

#define SEED_KEY_SIZE 16

class HmacHash
{
    public:
        HmacHash();
        HmacHash(uint32 len, uint8 *seed);
        ~HmacHash();
        void UpdateBigNumber(BigNumber *bn);
        void UpdateData(const uint8 *data, int length);
        void UpdateData(const std::string &str);
        void Initialize();
        void Finalize();
        uint8 *GetDigest() { return m_digest; };
        int GetLength() { return SHA_DIGEST_LENGTH; };
    private:
        HMAC_CTX m_ctx;
        uint8 m_key[SEED_KEY_SIZE];
        uint8 m_digest[SHA_DIGEST_LENGTH];
};
#endif

