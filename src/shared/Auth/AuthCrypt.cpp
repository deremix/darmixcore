/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "AuthCrypt.h"
#include "Hmac.h"

AuthCrypt::AuthCrypt()
{
    _initialized = false;
}

void AuthCrypt::Init()
{
    _send_i = _send_j = _recv_i = _recv_j = 0;
    _initialized = true;
}

void AuthCrypt::DecryptRecv(uint8 *data, size_t len)
{
    if (!_initialized) return;
    if (len < CRYPTED_RECV_LEN) return;

    for (size_t t = 0; t < CRYPTED_RECV_LEN; t++)
    {
        _recv_i %= _key.size();
        uint8 x = (data[t] - _recv_j) ^ _key[_recv_i];
        ++_recv_i;
        _recv_j = data[t];
        data[t] = x;
    }
}

void AuthCrypt::EncryptSend(uint8 *data, size_t len)
{
    if (!_initialized) return;
    if (len < CRYPTED_SEND_LEN) return;

    for (size_t t = 0; t < CRYPTED_SEND_LEN; t++)
    {
        _send_i %= _key.size();
        uint8 x = (data[t] ^ _key[_send_i]) + _send_j;
        ++_send_i;
        data[t] = _send_j = x;
    }
}

void AuthCrypt::SetKey(BigNumber *bn)
{
    uint8 *key = new uint8[SHA_DIGEST_LENGTH];
    GenerateKey(key, bn);
    _key.resize(SHA_DIGEST_LENGTH);
    std::copy(key, key + SHA_DIGEST_LENGTH, _key.begin());
    delete key;
}

AuthCrypt::~AuthCrypt()
{
}

void AuthCrypt::GenerateKey(uint8 *key, BigNumber *bn)
{
    HmacHash hash;
    hash.UpdateBigNumber(bn);
    hash.Finalize();
    memcpy(key, hash.GetDigest(), SHA_DIGEST_LENGTH);
}

