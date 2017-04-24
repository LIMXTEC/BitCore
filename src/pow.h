// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The BitCore Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCORE_POW_H
#define BITCORE_POW_H

#include "consensus/params.h"

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class uint256;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&); // BitCore: Select retargeting
unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&); // BitCore: Standard retargeting (V1)
unsigned int GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&); // BitCore: Digishield retargeting (V2)
//unsigned int GetNextWorkRequired_V3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&); // BitCore: Retargeting to support the PoW change phase (V3)
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&); // BitCore: Select retargeting
unsigned int CalculateNextWorkRequired_V1(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&); // BitCore: Standard retargeting (V1)
unsigned int CalculateNextWorkRequired_V2(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&); // BitCore: Digishield retargeting (V2)
//unsigned int CalculateNextWorkRequired_V3(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&); // BitCore: Retargeting to support the PoW change phase (V3)

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params&);

#endif // BITCORE_POW_H
