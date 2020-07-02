// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2019 Limxtec developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <utilstrencodings.h>
#include <crypto/common.h>

/*
#include <crypto/lyra2z.h>
#include <crypto/nist5.h>
#include <crypto/x11.h>
*/
#include <crypto/mega-btx.h>


// BTX BEGINN
#include <crypto/hashblock.h> // BitCore TimeTravel
#include <crypto/scrypt.h>
// BTX END

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{
    uint256 powHash = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    //BitCore
    if(GetBlockTime() >= HASH_FORK_TIME_1) //Wednesday, 23. October 2019 12:02:26
    {
        return Mega_Btx(BEGIN(nVersion), END(nNonce), GetBlockTime());
    }
    else
    {
        if(GetBlockTime() >= 1493124696) //Human time (GMT): Tue, 25 Apr 2017 12:51:36 GMT
            {
                return HashTimeTravel(BEGIN(nVersion), END(nNonce), GetBlockTime()); // BitCore TimeTravel
            }
            else
            {

                scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(powHash)); // BitCore Scrypt
                return powHash;
            }
    }
    return powHash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
