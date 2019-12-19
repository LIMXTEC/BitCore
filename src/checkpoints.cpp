// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <checkpoints.h>

#include <chain.h>
#include <chainparams.h>
#include <reverse_iterator.h>
#include <validation.h>

#include <stdint.h>


namespace Checkpoints {

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        for (const MapCheckpoints::value_type& i : reverse_iterate(checkpoints))
        {
            const uint256& hash = i.second;
            CBlockIndex* pindex = LookupBlockIndex(hash);
            if (pindex) {
                return pindex;
            }
        }
        return nullptr;
    }

    // FXTC BEGIN
    bool IsExpectedCheckpoint(const CCheckpointData& data, int nHeight, const uint256& hash)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        for (const MapCheckpoints::value_type& i : reverse_iterate(checkpoints))
        {
            // valid if block itself is checkpoint
            if (i.first == nHeight)
                return (i.second == hash);

           // valid if block is above checkpoint
            if (i.first < nHeight)
                return (mapBlockIndex.find(i.second) != mapBlockIndex.end());
        }

        // valid if there are no checkpoints
        return true;
    }
    // FXTC END

} // namespace Checkpoints
