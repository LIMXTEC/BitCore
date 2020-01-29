// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2019 Limxtec developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>
// FXTC BEGIN
#include <spork.h>
// FXTC END

// Megacoin
#include <bignum.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>
#include <math.h>

unsigned int static DUAL_KGW3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params ) 
{
    // current difficulty formula, ERC3 - DUAL_KGW3, written by Bitcoin Talk Limx Dev
    // BitSend and Europecoin Developer
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    bool kgwdebug=false;
    uint64_t PastBlocksMass = 0;
    int64_t PastRateActualSeconds = 0;
    int64_t PastRateTargetSeconds = 0;
    double PastRateAdjustmentRatio = double(1);
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;
    double EventHorizonDeviation;
    double EventHorizonDeviationFast;
    double EventHorizonDeviationSlow;
    //DUAL_KGW3 SETUP
    static const uint64_t Blocktime = 9.6 * 60; // 9.6 = 10 min (Value = Value*0.96) Limx DEV 23.04.2017
    static const unsigned int timeDaySeconds = 60 * 60 * 24;
    uint64_t pastSecondsMin = timeDaySeconds * 0.025;
    uint64_t pastSecondsMax = timeDaySeconds * 7;
    uint64_t PastBlocksMin = pastSecondsMin / Blocktime;
    uint64_t PastBlocksMax = pastSecondsMax / Blocktime;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) {  return bnPowLimit.GetCompact(); }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;
        PastDifficultyAverage.SetCompact(BlockReading->nBits);
        if (i > 1) {
            if(PastDifficultyAverage >= PastDifficultyAveragePrev)
                PastDifficultyAverage = ((PastDifficultyAverage - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev;
            else
                PastDifficultyAverage = PastDifficultyAveragePrev - ((PastDifficultyAveragePrev - PastDifficultyAverage) / i);
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;
        PastRateActualSeconds = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds = Blocktime * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(72)), -1.228));  //28.2 and 144 possible
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
                if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast))
                { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    //KGW Original
    arith_uint256 kgw_dual1(PastDifficultyAverage);
    arith_uint256 kgw_dual2;
    kgw_dual2.SetCompact(pindexLast->nBits);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
         kgw_dual1 *= PastRateActualSeconds;
         kgw_dual1 /= PastRateTargetSeconds;
    }

    int64_t nActualTime1 = pindexLast->GetBlockTime() - pindexLast->pprev->GetBlockTime();
    int64_t nActualTimespanshort = nActualTime1;

    // Retarget BTC Original ...not exactly
    // Small Fix

    if(nActualTime1 < 0) nActualTime1 = Blocktime;

    if (nActualTime1 < Blocktime / 3)
        nActualTime1 = Blocktime / 3;
    if (nActualTime1 > Blocktime * 3)
        nActualTime1 = Blocktime * 3;

    kgw_dual2 *= nActualTime1;
    kgw_dual2 /= Blocktime;

    //Fusion from Retarget and Classic KGW3 (BitSend=)

    arith_uint256 bnNew;
    bnNew = ((kgw_dual2 + kgw_dual1)/2);
    // DUAL KGW3 increased rapidly the Diff if Blocktime to last block under Blocktime/6 sec.

    if(kgwdebug)LogPrintf("nActualTimespanshort = %d \n", nActualTimespanshort );
    if( nActualTimespanshort < Blocktime/6 )
        {
        if(kgwdebug)LogPrintf("Vordiff:%08x %s bnNew first  \n", bnNew.GetCompact(), bnNew.ToString().c_str());
        const int nLongShortNew1   = 85; const int nLongShortNew2   = 100;
        bnNew = bnNew * nLongShortNew1;    bnNew = bnNew / nLongShortNew2;
        if(kgwdebug)LogPrintf("Erhöhte Diff:\n %08x %s bnNew second \n", bnNew.GetCompact(), bnNew.ToString().c_str() );
        }


    //BitBreak BitSend
    // Reduce difficulty if current block generation time has already exceeded maximum time limit.
    // Diffbreak 12 Hours
    const int nLongTimeLimit   = 12 * 60 * 60;
    if(kgwdebug)
    {
    LogPrintf("Prediff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
    LogPrintf("Vordiff %d \n", nLongTimeLimit);
    LogPrintf(" %d Block \n", BlockReading->nHeight );
    }

    if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit)  //block.nTime
    {
        bnNew = bnPowLimit;
           if(kgwdebug)LogPrintf("<BSD> Maximum block time hit - cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
    }

    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
    }
    return bnNew.GetCompact();
}

/*
unsigned int static DarkGravityWave(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);

    const int64_t nPastAlgoFastBlocks = 5; // fast average for algo
    const int64_t nPastAlgoBlocks = nPastAlgoFastBlocks * ALGO_ACTIVE_COUNT; // average for algo

    const int64_t nPastFastBlocks = nPastAlgoFastBlocks * 2; //fast average for chain
    int64_t nPastBlocks = nPastFastBlocks * ALGO_ACTIVE_COUNT; // average for chain

    // stabilizing block spacing
    if ((pindexLast->nHeight + 1) >= 0)
        nPastBlocks *= 100;

    // make sure we have at least ALGO_ACTIVE_COUNT blocks, otherwise just return powLimit
    if (!pindexLast || pindexLast->nHeight < nPastBlocks) {
        if (pindexLast->nHeight < nPastAlgoBlocks)
            return bnPowLimit.GetCompact();
        else
            nPastBlocks = pindexLast->nHeight;
    }

    const CBlockIndex *pindex = pindexLast;
    const CBlockIndex *pindexFast = pindexLast;
    arith_uint256 bnPastTargetAvg(0);
    arith_uint256 bnPastTargetAvgFast(0);

    const CBlockIndex *pindexAlgo = nullptr;
    const CBlockIndex *pindexAlgoFast = nullptr;
    const CBlockIndex *pindexAlgoLast = nullptr;
    arith_uint256 bnPastAlgoTargetAvg(0);
    arith_uint256 bnPastAlgoTargetAvgFast(0);

    // count blocks mined by actual algo for secondary average
    int32_t nVersion = pblock->nVersion & ALGO_VERSION_MASK;

    unsigned int nCountBlocks = 0;
    unsigned int nCountFastBlocks = 0;
    unsigned int nCountAlgoBlocks = 0;
    unsigned int nCountAlgoFastBlocks = 0;

    while (nCountBlocks < nPastBlocks && nCountAlgoBlocks < nPastAlgoBlocks) {
        arith_uint256 bnTarget = arith_uint256().SetCompact(pindex->nBits) / pindex->GetBlockHeader().GetAlgoEfficiency(pindex->nHeight); // convert to normalized target by algo efficiency

        // calculate algo average
        if (nVersion == (pindex->nVersion & ALGO_VERSION_MASK))
        {
            nCountAlgoBlocks++;

            pindexAlgo = pindex;
            if (!pindexAlgoLast)
                pindexAlgoLast = pindex;

            // algo average
            bnPastAlgoTargetAvg = (bnPastAlgoTargetAvg * (nCountAlgoBlocks - 1) + bnTarget) / nCountAlgoBlocks;
            // fast algo average
            if (nCountAlgoBlocks <= nPastAlgoFastBlocks)
            {
                nCountAlgoFastBlocks++;
                pindexAlgoFast = pindex;
                bnPastAlgoTargetAvgFast = bnPastAlgoTargetAvg;
            }
        }

        nCountBlocks++;

        // average
        bnPastTargetAvg = (bnPastTargetAvg * (nCountBlocks - 1) + bnTarget) / nCountBlocks;
        // fast average
        if (nCountBlocks <= nPastFastBlocks)
        {
            nCountFastBlocks++;
            pindexFast = pindex;
            bnPastTargetAvgFast = bnPastTargetAvg;
        }

        // next block
        if(nCountBlocks != nPastBlocks) {
            assert(pindex->pprev); // should never fail
            pindex = pindex->pprev;
        }
    }

    // FXTC instamine protection for blockchain
    if (pindexLast->GetBlockTime() - pindexFast->GetBlockTime() < params.nPowTargetSpacing / 2)
    {
        nCountBlocks = nCountFastBlocks;
        pindex = pindexFast;
        bnPastTargetAvg = bnPastTargetAvgFast;
    }

    arith_uint256 bnNew(bnPastTargetAvg);

    if (pindexAlgo && pindexAlgoLast && nCountAlgoBlocks > 1)
    {
        // FXTC instamine protection for algo
        if (pindexLast->GetBlockTime() - pindexAlgoFast->GetBlockTime() < params.nPowTargetSpacing * ALGO_ACTIVE_COUNT / 2)
        {
            nCountAlgoBlocks = nCountAlgoFastBlocks;
            pindexAlgo = pindexAlgoFast;
            bnPastAlgoTargetAvg = bnPastAlgoTargetAvgFast;
        }

        bnNew = bnPastAlgoTargetAvg;

        // pindexLast instead of pindexAlgoLst on purpose
        int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexAlgo->GetBlockTime();
        int64_t nTargetTimespan = nCountAlgoBlocks * params.nPowTargetSpacing * ALGO_ACTIVE_COUNT;

        // higher algo diff faster
        if (nActualTimespan < 1)
            nActualTimespan = 1;
        // lower algo diff slower
        if (nActualTimespan > nTargetTimespan*2)
            nActualTimespan = nTargetTimespan*2;

        // Retarget algo
        bnNew *= nActualTimespan;
        bnNew /= nTargetTimespan;
    } else {
        bnNew = bnPowLimit;
    }

    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindex->GetBlockTime();
    int64_t nTargetTimespan = nCountBlocks * params.nPowTargetSpacing;

    // higher diff faster
    if (nActualTimespan < 1)
        nActualTimespan = 1;
    // lower diff slower
    if (nActualTimespan > nTargetTimespan*2)
        nActualTimespan = nTargetTimespan*2;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    // at least PoW limit
    if ((bnPowLimit / pblock->GetAlgoEfficiency(pindexLast->nHeight+1)) > bnNew)
        bnNew *= pblock->GetAlgoEfficiency(pindexLast->nHeight+1); // convert normalized target to actual algo target
    else
        bnNew = bnPowLimit;

    // mining handbrake via spork
    if ((bnPowLimit * GetHandbrakeForce(pblock->nVersion, pindexLast->nHeight+1)) < bnNew)
        bnNew = bnPowLimit;
    else
        bnNew /= GetHandbrakeForce(pblock->nVersion, pindexLast->nHeight+1);

    return bnNew.GetCompact();
}
*/
/*
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}
*/

// Megacoin
unsigned int static KimotoGravityWell(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params) {
    /* current difficulty formula, megacoin - kimoto gravity well */
    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    
    uint64_t PastBlocksMass = 0;
    int64_t PastRateActualSeconds = 0;
    int64_t PastRateTargetSeconds = 0;
    double PastRateAdjustmentRatio = double(1);
    CBigNum PastDifficultyAverage;
    CBigNum PastDifficultyAveragePrev;
    double EventHorizonDeviation;
    double EventHorizonDeviationFast;
    double EventHorizonDeviationSlow;
    //DUAL_KGW3 SETUP
    static const int64_t Blocktime = 2.5 * 60; // 9.6 = 10 min (Value = Value*0.96) Limx DEV 23.04.2017
    static const unsigned int timeDaySeconds = 60 * 60 * 24;
    int64_t pastSecondsMin = timeDaySeconds * 0.25;
    int64_t pastSecondsMax = timeDaySeconds * 7;
    uint64_t PastBlocksMin = pastSecondsMin / Blocktime;
    uint64_t PastBlocksMax = pastSecondsMax / Blocktime;
    //const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    static const CBigNum bnPowLimit(uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) { return bnPowLimit.GetCompact(); }

    //int64_t LatestBlockTime = BlockLastSolved->GetBlockTime(); //not used

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;
            if (i == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
            else        { PastDifficultyAverage = ((CBigNum().SetCompact(BlockReading->nBits) - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev; }
        PastDifficultyAveragePrev = PastDifficultyAverage;

        PastRateActualSeconds                        = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds                        = Blocktime * PastBlocksMass;
        PastRateAdjustmentRatio                      = double(1);
        if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio                  = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(144)), -1.228));
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow  = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
            if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast)) { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    CBigNum bnNew(PastDifficultyAverage);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            bnNew *= PastRateActualSeconds;
            bnNew /= PastRateTargetSeconds;
        }
        if (bnNew > bnPowLimit) {
            bnNew = bnPowLimit;
        }
    return bnNew.GetCompact();
    
    /*
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) {  return bnPowLimit.GetCompact(); }

    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;
        PastDifficultyAverage.SetCompact(BlockReading->nBits);
        if (i > 1) {
            if(PastDifficultyAverage >= PastDifficultyAveragePrev)
                PastDifficultyAverage = ((PastDifficultyAverage - PastDifficultyAveragePrev) / i) + PastDifficultyAveragePrev;
            else
                PastDifficultyAverage = PastDifficultyAveragePrev - ((PastDifficultyAveragePrev - PastDifficultyAverage) / i);
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;
        PastRateActualSeconds = BlockLastSolved->GetBlockTime() - BlockReading->GetBlockTime();
        PastRateTargetSeconds = Blocktime * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation = 1 + (0.7084 * pow((double(PastBlocksMass)/double(72)), -1.228));  //28.2 and 144 possible
        EventHorizonDeviationFast = EventHorizonDeviation;
        EventHorizonDeviationSlow = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
                if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) || (PastRateAdjustmentRatio >= EventHorizonDeviationFast))
                { assert(BlockReading); break; }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }
    
    //KGW Original
    arith_uint256 kgw_dual1(PastDifficultyAverage);
    arith_uint256 kgw_dual2;
    kgw_dual2.SetCompact(pindexLast->nBits);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
         kgw_dual1 *= PastRateActualSeconds;
         kgw_dual1 /= PastRateTargetSeconds;
    }
    
    int64_t nActualTime1 = pindexLast->GetBlockTime() - pindexLast->pprev->GetBlockTime();
    int64_t nActualTimespanshort = nActualTime1;
    
    // Retarget BTC Original ...not exactly
    // Small Fix

    if(nActualTime1 < 0) nActualTime1 = Blocktime;

    if (nActualTime1 < Blocktime / 3)
        nActualTime1 = Blocktime / 3;
    if (nActualTime1 > Blocktime * 3)
        nActualTime1 = Blocktime * 3;
        
    kgw_dual2 *= nActualTime1;
    kgw_dual2 /= Blocktime;
    
    //Fusion from Retarget and Classic KGW3 (BitSend=)
    
    arith_uint256 bnNew;
    bnNew = ((kgw_dual2 + kgw_dual1)/2);
    // DUAL KGW3 increased rapidly the Diff if Blocktime to last block under Blocktime/6 sec.
    
    if(kgwdebug)LogPrintf("nActualTimespanshort = %d \n", nActualTimespanshort );
    if( nActualTimespanshort < Blocktime/6 )
        {
        if(kgwdebug)LogPrintf("Vordiff:%08x %s bnNew first  \n", bnNew.GetCompact(), bnNew.ToString().c_str());
        const int nLongShortNew1   = 85; const int nLongShortNew2   = 100;
        bnNew = bnNew * nLongShortNew1;	bnNew = bnNew / nLongShortNew2;	
        if(kgwdebug)LogPrintf("Erhöhte Diff:\n %08x %s bnNew second \n", bnNew.GetCompact(), bnNew.ToString().c_str() );
        }

    
    //BitBreak BitSend
    // Reduce difficulty if current block generation time has already exceeded maximum time limit.
    // Diffbreak 12 Hours
    const int nLongTimeLimit   = 12 * 60 * 60; 
    if(kgwdebug)
    {
    LogPrintf("Prediff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str());
    LogPrintf("Vordiff %d \n", nLongTimeLimit);
    LogPrintf(" %d Block", BlockReading->nHeight );
    }
    
    if ((pblock-> nTime - pindexLast->GetBlockTime()) > nLongTimeLimit)  //block.nTime 
    {
        bnNew = bnPowLimit;
       	if(kgwdebug)LogPrintf("<BSD> Maximum block time hit - cute diff %08x %s\n", bnNew.GetCompact(), bnNew.ToString().c_str()); 
    }

    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
    }
    return bnNew.GetCompact();
    */
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    /*
    unsigned int nBits = DarkGravityWave(pindexLast, pblock, params);

    // Dead lock protection will halve work every block spacing when no block for 2 * number of active algos * block spacing (BitCore: every two minutes if no block for 10 minutes)
    int nHalvings = (pblock->GetBlockTime() - pindexLast->GetBlockTime()) / (params.nPowTargetSpacing * 2) - ALGO_ACTIVE_COUNT + 1;
    if (nHalvings > 0)
    {
        const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
        arith_uint256 bnBits;
        bnBits.SetCompact(nBits);

        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2x block spacing
        // then allow mining of a min-difficulty block.
        // Also can not be less than PoW limit.
        if (params.fPowAllowMinDifficultyBlocks || (bnPowLimit >> nHalvings) < bnBits)
            bnBits = bnPowLimit;
        else
            bnBits <<= nHalvings;

        nBits = bnBits.GetCompact();
    }

    return nBits;
    */

    // Megacoin
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();
    if (params.fPowNoRetargeting && params.fPowAllowMinDifficultyBlocks )
    {
        return nProofOfWorkLimit;
    }

    int fork1 = 10000;
    int fork2 = 21000;
    
    if (pindexLast->nHeight+1 <= fork1)
    {
    return DUAL_KGW3(pindexLast, pblock, params);
    }
/*
    // Megacoin Miningalgo switch
	// 1571832146 Wednesday, 23. October 2019 12:02:26
	// please check also block.cpp:L62
    if(pblock->GetBlockTime() >= 1571832146 && pindexLast->GetBlockTime() <= 1571832146 + 86400) // We have a timerange from 24 hours  to find a new block
    {
        if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*24) 
            {
        //consensus.nPowTargetSpacing = 2.5 * 60; // Megacoin	
        //This should be one hour then is this function possible
    LogPrintf("Megacoin Hashalgoupdate HashX16R \n");
    return nProofOfWorkLimit;
            }
    }
*/


    if (pindexLast->nHeight+1 <= fork2)
    {
    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // MegaCoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
    }

    else
    {
        // Genesis block
        if (pindexLast == NULL)
            return nProofOfWorkLimit;

        // Only change once per difficulty adjustment interval
        if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2() != 0)
        {
            if (params.fPowAllowMinDifficultyBlocks)
            {
                // Special difficulty rule for testnet:
                // If the new block's timestamp is more than 2* 10 minutes
                // then allow mining of a min-difficulty block.
                if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                    return nProofOfWorkLimit;
                else
                {
                    // Return the last non-special-min-difficulty-rules-block
                    const CBlockIndex* pindex = pindexLast;
                    while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentIntervalV2() != 0 && pindex->nBits == nProofOfWorkLimit)
                        pindex = pindex->pprev;
                    return pindex->nBits;
                }
            }
            // LogPrintf("difficulty adjustment interval %d  \n",(pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2());
            return pindexLast->nBits;
        }

        // Go back by what we want to be 14 days worth of blocks
        // Litecoin: This fixes an issue where a 51% attack can change difficulty at will.
        // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
        int blockstogoback2 = params.DifficultyAdjustmentIntervalV2()-1;
        if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentIntervalV2())
            blockstogoback2 = params.DifficultyAdjustmentIntervalV2();

        // Go back by what we want to be 14 days worth of blocks
        const CBlockIndex* pindexFirst = pindexLast;
        for (int i = 0; pindexFirst && i < blockstogoback2; i++)
            pindexFirst = pindexFirst->pprev;

        assert(pindexFirst);
        return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
    }
}
/*
unsigned int GetNextWorkRequiredFXTC(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    unsigned int nBits = DarkGravityWave(pindexLast, pblock, params);

    // Dead lock protection will halve work every block spacing when no block for 2 * number of active algos * block spacing (BitCore: every two minutes if no block for 10 minutes)
    int nHalvings = (pblock->GetBlockTime() - pindexLast->GetBlockTime()) / (params.nPowTargetSpacing * 2) - ALGO_ACTIVE_COUNT + 1;
    if (nHalvings > 0)
    {
        const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
        arith_uint256 bnBits;
        bnBits.SetCompact(nBits);

        // Special difficulty rule for testnet:
        // If the new block's timestamp is more than 2x block spacing
        // then allow mining of a min-difficulty block.
        // Also can not be less than PoW limit.
        if (params.fPowAllowMinDifficultyBlocks || (bnPowLimit >> nHalvings) < bnBits)
            bnBits = bnPowLimit;
        else
            bnBits <<= nHalvings;

        nBits = bnBits.GetCompact();
    }

    return nBits;
}
*/

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
	// Bitcore
    int fork2 = 21000;
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Initial //64_15 Written by Limx Dev 04/2017
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    bool fShift;
    // Initial
    if (pindexLast->nHeight+1 <= fork2)
    {
        if (nActualTimespan < params.nPowTargetTimespan/4)
            nActualTimespan = params.nPowTargetTimespan/4;
        if (nActualTimespan > params.nPowTargetTimespan*4)
            nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // MegaCoin: intermediate uint256 can overflow by 1 bit
    fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();

    }
    else
    {
        if (nActualTimespan < params.nPowTargetTimespanV2/1.15)
            nActualTimespan = params.nPowTargetTimespanV2/1.15;
        if (nActualTimespan > params.nPowTargetTimespanV2*1.15)
            nActualTimespan = params.nPowTargetTimespanV2*1.15;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // MegaCoin: intermediate uint256 can overflow by 1 bit
    fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespanV2;
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
    }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

/*
// FXTC BEGIN
unsigned int GetHandbrakeForce(int32_t nVersion, int nHeight)
{
    int32_t nVersionAlgo = nVersion & ALGO_VERSION_MASK;

    // NIST5 braked and disabled
    if (nVersionAlgo == ALGO_NIST5)
    {
        if (nHeight >= 21000) return 4070908800;
        if (nHeight >= 19335) return 20;
    }

    if (nHeight >= sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_HEIGHT))
    {
        switch (nVersionAlgo)
        {
            case ALGO_SHA256D: return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_SHA256D);
            case ALGO_SCRYPT:  return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_SCRYPT);
            case ALGO_NIST5:   return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_NIST5);
            case ALGO_LYRA2Z:  return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_LYRA2Z);
            case ALGO_X11:     return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_X11);
            case ALGO_X16R:    return sporkManager.GetSporkValue(SPORK_BTX_01_HANDBRAKE_FORCE_X16R);
            default:           return 1; // FXTC TODO: we should not be here
        }
    }

    return 1; // FXTC TODO: we should not be here
}
// FXTC END
*/
