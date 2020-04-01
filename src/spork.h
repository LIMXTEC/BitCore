// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2019 Limxtec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCORE_SPORK_H
#define BITCORE_SPORK_H

#include <hash.h>
#include <net.h>
#include <utilstrencodings.h>

// FXTC BEGIN
class CSporkDB;

/** Global variable that points to the spork database (protected by cs_main) */
extern std::unique_ptr<CSporkDB> pSporkDB;
//FXTC END

class CSporkMessage;
class CSporkManager;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what
*/
static const int SPORK_START                                            = 10001;
static const int SPORK_END                                              = 10013;
// FXTC BEGIN
static const int SPORK_BTX_START                                    = 94680010;
static const int SPORK_BTX_END                                      = 94680033;
// FXTC END

static const int SPORK_2_INSTANTSEND_ENABLED                            = 10001;
static const int SPORK_3_INSTANTSEND_BLOCK_FILTERING                    = 10002;
static const int SPORK_5_INSTANTSEND_MAX_VALUE                          = 10004;
static const int SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT                 = 10007;
static const int SPORK_9_SUPERBLOCKS_ENABLED                            = 10008;
static const int SPORK_10_MASTERNODE_PAY_UPDATED_NODES                  = 10009;
static const int SPORK_12_RECONSIDER_BLOCKS                             = 10011;
static const int SPORK_13_OLD_SUPERBLOCK_FLAG                           = 10012;
static const int SPORK_14_REQUIRE_SENTINEL_FLAG                         = 10013;
// FXTC BEGIN
static const int SPORK_BTX_01_HANDBRAKE_HEIGHT                      = 94680010;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_SHA256D               = 94680011;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_SCRYPT                = 94680012;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_NIST5                 = 94680013;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_LYRA2Z                = 94680014;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_X11                   = 94680015;
static const int SPORK_BTX_01_HANDBRAKE_FORCE_X16R                  = 94680016;

static const int SPORK_BTX_15_COINBASE_MATURITY_STAGE_2       = 94680021;
static const int SPORK_BTX_02_IGNORE_EXTRA_REWARD_CHECK           = 94680022;
static const int SPORK_BTX_02_IGNORE_EXTRA_REWARD_VALUE           = 94680023;
static const int SPORK_BTX_16_UNKNOW        = 94680024;
static const int SPORK_BTX_17_UNKNOW        = 94680025;

static const int SPORK_BTX_18_MIN_PEER_PROTO_VERSION     = 94680031;
static const int SPORK_BTX_19_SHOW_UPDATE_MESSAGE     = 94680032;
static const int SPORK_BTX_20_UNKNOW        = 94680026;
static const int SPORK_BTX_21_UNKNOW        = 94680027;
static const int SPORK_BTX_22_UNKNOW        = 94680028;
// FXTC END

static const int64_t SPORK_2_INSTANTSEND_ENABLED_DEFAULT                = 0;            // ON
static const int64_t SPORK_3_INSTANTSEND_BLOCK_FILTERING_DEFAULT        = 0;            // ON
static const int64_t SPORK_5_INSTANTSEND_MAX_VALUE_DEFAULT              = 1000;         // 1000 BitCore
static const int64_t SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT     = 4070908800ULL;// OFF
static const int64_t SPORK_9_SUPERBLOCKS_ENABLED_DEFAULT                = 4070908800ULL;// OFF
static const int64_t SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_12_RECONSIDER_BLOCKS_DEFAULT                 = 0;            // 0 BLOCKS
static const int64_t SPORK_13_OLD_SUPERBLOCK_FLAG_DEFAULT               = 4070908800ULL;// OFF
static const int64_t SPORK_14_REQUIRE_SENTINEL_FLAG_DEFAULT             = 4070908800ULL;// OFF
// FXTC BEGIN
static const int64_t SPORK_BTX_01_HANDBRAKE_HEIGHT_DEFAULT             = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_SHA256D_DEFAULT      = 1;            // 1x
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_SCRYPT_DEFAULT       = 1;            // 1x
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_NIST5_DEFAULT        = 1;            // 1x
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_LYRA2Z_DEFAULT       = 1;            // 1x
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_X11_DEFAULT          = 1;            // 1x
static const int64_t SPORK_BTX_01_HANDBRAKE_FORCE_X16R_DEFAULT         = 1;            // 1x

static const int64_t SPORK_BTX_15_COINBASE_MATURITY_STAGE_2_DEFAULT  = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_02_IGNORE_EXTRA_REWARD_CHECK_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_02_IGNORE_EXTRA_REWARD_VALUE_DEFAULT      = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_16_UNKNOW_DEFAULT   = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_17_UNKNOW_DEFAULT   = 4070908800ULL;// OFF

static const int64_t SPORK_BTX_18_MIN_PEER_PROTO_VERSION_DEFAULT  = 70016;// OFF
static const int64_t SPORK_BTX_19_SHOW_UPDATE_MESSAGE_DEFAULT  = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_20_UNKNOW_DEFAULT   = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_21_UNKNOW_DEFAULT   = 4070908800ULL;// OFF
static const int64_t SPORK_BTX_22_UNKNOW_DEFAULT   = 4070908800ULL;// OFF



extern std::map<uint256, CSporkMessage> mapSporks;
extern CSporkManager sporkManager;

//
// Spork classes
// Keep track of all of the network spork settings
//

class CSporkMessage
{
private:
    std::vector<unsigned char> vchSig;

public:
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    CSporkMessage(int nSporkID, int64_t nValue, int64_t nTimeSigned) :
        nSporkID(nSporkID),
        nValue(nValue),
        nTimeSigned(nTimeSigned)
        {}

    CSporkMessage() :
        nSporkID(0),
        nValue(0),
        nTimeSigned(0)
        {}


    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }

    uint256 GetHash() const
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << nSporkID;
        ss << nValue;
        ss << nTimeSigned;
        return ss.GetHash();
    }

    bool Sign(std::string strSignKey);
    bool CheckSignature();
    void Relay(CConnman& connman);
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;
    std::map<int, CSporkMessage> mapSporksActive;

public:

    CSporkManager() {}

    // FXTC BEGIN
    void LoadSporksFromDB();
    // FXTC END
    void ProcessSpork(CNode* pfrom, const std::string& strCommand, CDataStream& vRecv, CConnman& connman);
    void ExecuteSpork(int nSporkID, int nValue);
    bool UpdateSpork(int nSporkID, int64_t nValue, CConnman& connman);

    bool IsSporkActive(int nSporkID);
    int64_t GetSporkValue(int nSporkID);
    int GetSporkIDByName(std::string strName);
    std::string GetSporkNameByID(int nSporkID);

    bool SetPrivKey(std::string strPrivKey);
};

#endif // BITCORE_SPORK_H
