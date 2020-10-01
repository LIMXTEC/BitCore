// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2019 Limxtec developers
// Copyright (c) 2019 BitCore developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // BTX
    const char* pszTimestamp = "Powerde by Bitsend-Europecoin-Diamond-MAC-B3 23/Apr/2017";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000; // BTX
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 50;
        consensus.nMasternodePaymentsIncreaseBlock = 50; //- not used 03-2020 in this build
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year - not used 03-2020 in this build
        consensus.nMasternodeCollateralMinimum = 2100; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 21000; // MN collateral at infinity - not used 03-2020 in this build

        consensus.nInstantSendKeepLock = 24;
        
        // We need a seperate Update for that!! Limxdev 2019
        consensus.nBudgetPaymentsStartBlock = 99999999; // 1 common year // tested 03-2020
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day
        
        // We need a seperate Update for that!! Limxdev 2019
        consensus.nSuperblockStartBlock = 99999999; // 1 common year
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb");
        consensus.BIP34Height = 2; // BTX
        consensus.BIP34Hash = uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb"); // BTX
        consensus.BIP65Height = 2; // BTX
        consensus.BIP66Height = 2; // BTX
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // BTX
        consensus.nPowTargetTimespanV2 = 160 * 60; // BTX
        consensus.nPowTargetSpacing = 2.5 * 60; // BTX
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 250; // BTX
        consensus.nMinerConfirmationWindow = 1000; // BTX
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 4032;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 3226; // 80% of 4032
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000bb36f8a4420338fb"); // BTX Diff 8.12
        // consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000005dfab1790ba9fdd"); // BTX mindiff

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb"); // BTX

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        // BTX
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;
        nDefaultPort = 8555;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1492973331, 9377, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x604148281e5c4b7f2487e5d03cd60d8e6f69411d613f6448034508cea52e9574"));
        assert(genesis.hashMerkleRoot == uint256S("0x1669526520b4e037738825c5f09c01c8f6ef6a3a5ee552e65e6d0141507987c7"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        // BTX
        vSeeds.emplace_back("seed.bitcore.biz");
        vSeeds.emplace_back("37.120.190.76");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("185.194.140.60");
        vSeeds.emplace_back("188.71.223.206");
        vSeeds.emplace_back("185.194.142.122");
        //vSeeds.emplace_back("185.194.142.122", false);

        // BTX prefix '2'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,3);
        // BTX prefix 's'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125);
        // BTX prefix 's'
        //base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,125); //s 34 letters
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "btx"; // BTX

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04C94699010E62A7A67E30C64BB61F53E93D03A668E19176563260A6167AB102B605E071CDB52C488E6F3F4AB284391539A917B3017581DA37097694EC176DCB36";

        founderAddress = "2S5FDKVg7tj3LYC5UQkoYF4BXkjoHm72Ta";

        // BTX
        checkpointData = {
            {
                { 2, uint256S("0x580a107e111bea326b64dc098c057a3b96622c1521c23e4f5b51647aa5e41ecb")},
                { 50000, uint256S("0xd3cdc4bd5bdafa908fae273f5cca9fda527b49e3fe727c6fe9122f28f2afbf6d")},
                { 100000, uint256S("0x99103dc00414fd4c90b03cd1a770626e2ac681baa10e5f7f7ce089a6d98f0dad")},
                { 150000, uint256S("0xa4245ff8d8a3d7fbbfde37a961fc74228ab142282d3c4f6104c69d28a5414c7c")},
                { 175000, uint256S("0x4b6cc8d2b186d4aaecab3b72bfa88469690e775ae586c35947d77bd6dabd1607")},
                { 200000, uint256S("0x65c89c662dbdad4aeeb4215076884be736a7512984741b353bbdef47478305e5")},
                { 215000, uint256S("0x18accc496518e6f3008eeaab7e04123939d66eb7f8a02f2fcb3d69093c1df38c")},
                { 356548, uint256S("0xf62b6c5645997d078d9cb130cfe96d64ec2c047bbceca3ae8ffc373eef80b866")},
                { 550000, uint256S("0xa901848fe9d22814dbb3e0f62892d3bd7a1e70f6a48a459e68ebaf24ea5d5867")},
                { 590000, uint256S("0x81dc076e3b07fbc50a691ddbbe0b53ece5fe95562e54b289c589cd333f7a282a")},
                { 659006, uint256S("0xdb16bdb35d197193c18ab42259b55f0b91c4c84da94e2a49e681c4eb43d96ddb")},
            }
        };

        // BTX for faster loading
        consensus.nlastValidPowHashHeight = 659006;
        chainTxData = ChainTxData{
            // BTX: data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1588381219, // * UNIX timestamp of last known number of transactions
            2103714,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after that timestamp
        };

        // FXTC TODO: we need to resolve fee calculation bug and disable fallback
        ///* disable fallback fee on mainnet */
        //m_fallback_fee_enabled = false;
        m_fallback_fee_enabled = true;
        //
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000; // BTX
        consensus.nMinimumSubsidy = 0.00100000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 10; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 10;
        consensus.nMasternodePaymentsIncreasePeriod = 25; //- not used 03-2020 in this build
        consensus.nMasternodeCollateralMinimum = 10; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 1000; // MN collateral at infinity - not used 03-2020 in this build

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 92000; // zusammen mit BudgetPaymentsStartBlock SuperblockStartBlock
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 92000;  // zusammen mit BudgetPaymentsStartBlock SuperblockStartBlock
        consensus.nSuperblockCycle = 24;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 0; // genesis
        consensus.BIP34Hash = uint256S("0x5b77f70e30731bbf8c4b6c44cf62170cc559b6015aa9b7988d8d44a30c4a4614"); // BTX
        consensus.BIP65Height = 0; // genesis
        consensus.BIP66Height = 0; // genesis
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // BTX
        consensus.nPowTargetTimespanV2 = 160 * 60; // BTX
        consensus.nPowTargetSpacing = 2.5 * 60; // BTX
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // BTX
        consensus.nMinerConfirmationWindow = 2; // BTX
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nWindowSize = 100;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nThreshold = 50; // 50% of 100
        //
        //

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // BTX

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xe833c399f04b0e7fa4ad143cdc8d311fe5a654c42c95b36c6c4dd92adf41f88f"); // BTX

        // BTX
        pchMessageStart[0] = 0xfd;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xc8;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 8666;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1493124695, 1728920, 0x1e0ffff0, 1, 12.5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x02c5d66e8edb49984eb743c798bca069466ce457b7febfa3c3a01b33353b7bc6"));
        assert(genesis.hashMerkleRoot == uint256S("0x0ac58b2cf882f0d25b640d4b22411a22246962aff702064255b2611f48e300ff"));

        /*
        // BTX
        vFixedSeeds.clear();
        vSeeds.clear();
        */
        vSeeds.emplace_back("188.68.52.172");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("37.120.186.85");
        vSeeds.emplace_back("188.71.223.206");
        vSeeds.emplace_back("185.194.142.122");
        vSeeds.emplace_back("51.15.84.165");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "testnet1btx";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04C637C5DBCA831BF02266B23E0F271B2689A5754A83BBE00B901C821D567AD23343007E21DA062F2D1C9231B23FDE29D34C7FC8F58292735CBB348F48F768C6F3";

        founderAddress = "mv5hH1xC3KQJr4ATDr1yQLmFhRYgVmjua6";

        // BTX
        checkpointData = {
            {
                {0, uint256S("0xe02c5d66e8edb49984eb743c798bca069466ce457b7febfa3c3a01b33353b7bc6")},
            }
        };

        consensus.nlastValidPowHashHeight = 0;
        
        chainTxData = ChainTxData{
            // BTX: data as of block 0x8da1c7f79018fac8acac69a57b2f8b5d2743af67976a4525fdedc8c85a3a1418 (height 410476).
            1490495580, // * UNIX timestamp of last known number of transactions
            449775,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500.0       // * estimated number of transactions per second after that timestamp
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMinimumSubsidy = 10000.00000000 * COIN;

        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nMasternodeCollateralMinimum = 1; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 6;

        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;

        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;

        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;

        // FXTC TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 100000000; // BTX: BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256(); // BTX
        consensus.BIP65Height = 1351; // BTX: BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BTX: BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // BTX
        consensus.nPowTargetSpacing = 10 * 60; // BTX
        consensus.nPowTargetTimespanV2 = 160 * 60; // BTX
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // FXTC TODO:
        // Dash
        // Deployment of DIP0001
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        //consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        //
        //

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // BTX
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1492973331, 9377, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x604148281e5c4b7f2487e5d03cd60d8e6f69411d613f6448034508cea52e9574"));
        assert(genesis.hashMerkleRoot == uint256S("0x1669526520b4e037738825c5f09c01c8f6ef6a3a5ee552e65e6d0141507987c7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        founderAddress = "cXdevX1jzA1Bd6hZVTcTEPh5NH8iDx1s4Z";

        // BTX
        checkpointData = {
            {
                {0, uint256S("0x604148281e5c4b7f2487e5d03cd60d8e6f69411d613f6448034508cea52e9574")},
            }
        };

        consensus.nlastValidPowHashHeight = 0;
        
        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "regtest1btx";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
