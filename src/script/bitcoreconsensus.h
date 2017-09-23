// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2017-2017 The BitCoin Core developers
//// Copyright (c) 2009-2017 The BitCore Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCORE_BITCORECONSENSUS_H
#define BITCORE_BITCORECONSENSUS_H

#include <stdint.h>

#if defined(BUILD_BITCORE_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/bitcore-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBBITCORECONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BITCORECONSENSUS_API_VER 1

typedef enum bitcoreconsensus_error_t
{
    bitcoreconsensus_ERR_OK = 0,
    bitcoreconsensus_ERR_TX_INDEX,
    bitcoreconsensus_ERR_TX_SIZE_MISMATCH,
    bitcoreconsensus_ERR_TX_DESERIALIZE,
    bitcoreconsensus_ERR_AMOUNT_REQUIRED,
    bitcoreconsensus_ERR_INVALID_FLAGS,
} bitcoreconsensus_error;

/** Script verification flags */
enum
{
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    bitcoreconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = bitcoreconsensus_SCRIPT_FLAGS_VERIFY_P2SH | bitcoreconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               bitcoreconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | bitcoreconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               bitcoreconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | bitcoreconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not NULL, err will contain an error/success code for the operation
EXPORT_SYMBOL int bitcoreconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, bitcoreconsensus_error* err);

EXPORT_SYMBOL int bitcoreconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, bitcoreconsensus_error* err);

EXPORT_SYMBOL unsigned int bitcoreconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // BITCORE_BITCORECONSENSUS_H
