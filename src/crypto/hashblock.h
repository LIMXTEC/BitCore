#ifndef BITCORE_CRYPTO_HASHBLOCK_H
#define BITCORE_CRYPTO_HASHBLOCK_H

#include <arith_uint256.h>
#include <crypto/sph_blake.h>
#include <crypto/sph_bmw.h>
#include <crypto/sph_groestl.h>
#include <crypto/sph_jh.h>
#include <crypto/sph_keccak.h>
#include <crypto/sph_skein.h>
#include <crypto/sph_luffa.h>
#include <crypto/sph_cubehash.h>
#include <crypto/sph_shavite.h>
#include <crypto/sph_simd.h>
#include <crypto/sph_echo.h>
/*
//X16R Next Timetravel
#include <crypto/sph_hamsi.h>
#include <crypto/sph_fugue.h>
#include <crypto/sph_shabal.h>
#include <crypto/sph_whirlpool.h>
*/

//#include "util.h"
#include <algorithm>    // std::next_permutation

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_blake512_context     z_blake;
GLOBAL sph_bmw512_context       z_bmw;
GLOBAL sph_groestl512_context   z_groestl;
GLOBAL sph_jh512_context        z_jh;
GLOBAL sph_keccak512_context    z_keccak;
GLOBAL sph_skein512_context     z_skein;
GLOBAL sph_luffa512_context     z_luffa;
GLOBAL sph_cubehash512_context  z_cubehash;
GLOBAL sph_shavite512_context   z_shavite;
GLOBAL sph_simd512_context      z_simd;
GLOBAL sph_echo512_context      z_echo;

#define fillz() do { \
    sph_blake512_init(&z_blake); \
    sph_bmw512_init(&z_bmw); \
    sph_groestl512_init(&z_groestl); \
    sph_jh512_init(&z_jh); \
    sph_keccak512_init(&z_keccak); \
    sph_skein512_init(&z_skein); \
    sph_luffa512_init(&z_luffa); \
    sph_cubehash512_init(&z_cubehash); \
    sph_shavite512_init(&z_shavite); \
    sph_simd512_init(&z_simd); \
    sph_echo512_init(&z_echo); \
} while (0) 

#define ZBLAKE (memcpy(&ctx_blake, &z_blake, sizeof(z_blake)))
#define ZBMW (memcpy(&ctx_bmw, &z_bmw, sizeof(z_bmw)))
#define ZGROESTL (memcpy(&ctx_groestl, &z_groestl, sizeof(z_groestl)))
#define ZJH (memcpy(&ctx_jh, &z_jh, sizeof(z_jh)))
#define ZKECCAK (memcpy(&ctx_keccak, &z_keccak, sizeof(z_keccak)))
#define ZSKEIN (memcpy(&ctx_skein, &z_skein, sizeof(z_skein)))

#define HASH_FUNC_BASE_TIMESTAMP 1492973331 // BitCore: Genesis Timestamp
#define HASH_FUNC_COUNT 10                   // BitCore: HASH_FUNC_COUNT of 11
#define HASH_FUNC_COUNT_PERMUTATIONS 40320  // BitCore: HASH_FUNC_COUNT!

template<typename T1>
inline uint256 HashTimeTravel(const T1 pbegin, const T1 pend, uint32_t timestamp)
{
    sph_blake512_context     ctx_blake;
    sph_bmw512_context       ctx_bmw;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;
    sph_luffa512_context     ctx_luffa;
    sph_cubehash512_context  ctx_cubehash;
    sph_shavite512_context   ctx_shavite;
    sph_simd512_context      ctx_simd;
    sph_echo512_context      ctx_echo;
    static unsigned char pblank[1];

#ifndef QT_NO_DEBUG
    //std::string strhash;
    //strhash = "";
#endif

    arith_uint512 hash[HASH_FUNC_COUNT];

    // We want to permute algorithms. To get started we
    // initialize an array with a sorted sequence of unique
    // integers where every integer represents its own algorithm.
    uint32_t permutation[HASH_FUNC_COUNT];
    for (uint32_t i=0; i < HASH_FUNC_COUNT; i++) {
        permutation[i]=i;
    }

    // Compute the next permuation
    uint32_t steps = (timestamp - HASH_FUNC_BASE_TIMESTAMP)%HASH_FUNC_COUNT_PERMUTATIONS;
    for (uint32_t i=0; i < steps; i++) {
        std::next_permutation(permutation, permutation + HASH_FUNC_COUNT);
    }

    for (uint32_t i=0; i < HASH_FUNC_COUNT; i++) {
	    switch(permutation[i]) {
            case 0:
                sph_blake512_init(&ctx_blake);
                if (i == 0)
                    sph_blake512 (&ctx_blake, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_blake512 (&ctx_blake, static_cast<const void*>(&hash[i-1]), 64);                
                sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[i]));
            break;
            case 1:
                sph_bmw512_init(&ctx_bmw);
                if (i == 0)
                    sph_bmw512 (&ctx_bmw, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[i-1]), 64);
                sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[i]));
            break;
            case 2:
                sph_groestl512_init(&ctx_groestl);
                if (i == 0)
                    sph_groestl512 (&ctx_groestl, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_groestl512 (&ctx_groestl, static_cast<const void*>(&hash[i-1]), 64);
                sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[i]));
            break;
            case 3:
                sph_skein512_init(&ctx_skein);
                if (i == 0)
                    sph_skein512 (&ctx_skein, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_skein512 (&ctx_skein, static_cast<const void*>(&hash[i-1]), 64);
                sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[i]));
            break;
            case 4:
                sph_jh512_init(&ctx_jh);
                if (i == 0)
                    sph_jh512 (&ctx_jh, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_jh512 (&ctx_jh, static_cast<const void*>(&hash[i-1]), 64);
                sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[i]));
            break;
            case 5:
                sph_keccak512_init(&ctx_keccak);
                if (i == 0)
                    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[i-1]), 64);
                sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[i]));
            break;
            case 6:
                sph_luffa512_init(&ctx_luffa);
                if (i == 0)
                    sph_luffa512 (&ctx_luffa, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_luffa512 (&ctx_luffa, static_cast<void*>(&hash[i-1]), 64);
                sph_luffa512_close(&ctx_luffa, static_cast<void*>(&hash[i]));
            break;
            case 7:
                sph_cubehash512_init(&ctx_cubehash);
                if (i == 0)
                    sph_cubehash512 (&ctx_cubehash, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_cubehash512 (&ctx_cubehash, static_cast<const void*>(&hash[i-1]), 64);
                sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[i]));
            break;
            case 8:
                sph_shavite512_init(&ctx_shavite);
                if (i == 0)
                    sph_shavite512 (&ctx_shavite, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_shavite512(&ctx_shavite, static_cast<const void*>(&hash[i-1]), 64);
                sph_shavite512_close(&ctx_shavite, static_cast<void*>(&hash[i]));
            break;
            case 9:
                sph_simd512_init(&ctx_simd);
                if (i == 0)
                    sph_simd512 (&ctx_simd, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_simd512 (&ctx_simd, static_cast<const void*>(&hash[i-1]), 64);
                sph_simd512_close(&ctx_simd, static_cast<void*>(&hash[i]));
            break;
            case 10:
                sph_echo512_init(&ctx_echo);
                if (i == 0)
                    sph_echo512 (&ctx_echo, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
                else
                    sph_echo512 (&ctx_echo, static_cast<const void*>(&hash[i-1]), 64);
                sph_echo512_close(&ctx_echo, static_cast<void*>(&hash[i]));
            break;
	    }
    }

    return ArithToUint256(hash[HASH_FUNC_COUNT-1].trim256());

}

#endif // BITCORE_CRYPTO_HASHBLOCK_H
