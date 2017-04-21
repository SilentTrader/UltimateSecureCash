// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t CChainParams::GetProofOfWorkReward(int nHeight, int64_t nFees) const
{
    // miner's coin base reward
    
    int64_t nSubsidy = 0;

    if (nHeight <= 0)
        nSubsidy = 0;
    else
    if (nHeight == 1)
        nSubsidy = 36015156 * COIN;
    else
    if (nHeight <= 1000)
        nSubsidy = 4500 * COIN;

    if (fDebug && GetBoolArg("-printcreation"))
        printf("GetProofOfWorkReward() : create=%s nSubsidy=%"PRId64"\n", FormatMoney(nSubsidy).c_str(), nSubsidy);

    return nSubsidy + nFees;
};


    int64_t CChainParams::GetProofOfStakeReward(const CBlockIndex* pindexPrev, int64_t nCoinAge, int64_t nFees) const
// miner's coin stake reward based on coin age spent (coin-days)
{
    int64_t nSubsidy = nCoinAge * (COIN_YEAR_REWARD * 2) * 33 / (365 * 33 + 8);

    if (IsProtocolV3(pindexPrev->nHeight))
 	nSubsidy = nCoinAge * (COIN_YEAR_REWARD * 2) * 33 / (365 * 33 + 8);
    else
        nSubsidy = nCoinAge * (COIN_YEAR_REWARD * 2) * 33 / (365 * 33 + 8);

    if (fDebug && GetBoolArg("-printcreation"))
	LogPrintf("GetProofOfStakeReward(): create=%s nCoinAge=%d\n", FormatMoney(nSubsidy).c_str(), nCoinAge);

    return nSubsidy + nFees;
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;

        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;

        memcpy(&ip, &t, sizeof(ip));

        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CBaseChainParams : public CChainParams {
public:
    CBaseChainParams() {
        const char* pszTimestamp = "Fight for segwit! BTU Bugs Unliimted - Ver has lost his mind";
        CTransaction txNew;
        txNew.nTime = GENESIS_BLOCK_TIME;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = GENESIS_BLOCK_TIME;

        vSeeds.push_back(CDNSSeedData("45.55.52.85", "45.55.52.85"));
    }
    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const std::vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
};

class CMainParams : public CBaseChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xef;
        pchMessageStart[3] = 0xb4;

        vAlertPubKey = ParseHex("031d5def92b2d59943e57aaa8b1adbb110ff215fc4ebdc6fb5c9a797e2b1dea527");

        nDefaultPort = 51737;
        nRPCPort = 51736;
        nBIP44ID = 0x80000023;

        nLastPOWBlock = 1000;
        nLastFairLaunchBlock = 1;

        nFirstPosv2Block = 25000;
        nFirstPosv3Block = 25010;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20); // "standard" scrypt target limit for proof of work, results with 0,000244140625 proof-of-work difficulty
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 48);

        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 816452253;
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x00000438d60fb1a01a92a141f86d367589fd6190727d246ad24ac4119d3e6691"));
        assert(genesis.hashMerkleRoot == uint256("0x269277d971a47872328ec3c009ea778b72e9b5f00dd44af7ccd35b17d61c8d60"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(68).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(125).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(191).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0xEE)(0x80)(0x28)(0x6A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0xEE)(0x80)(0x31)(0xE8).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(137).convert_to_container<std::vector<unsigned char> >();         // x
        base58Prefixes[EXT_ACC_HASH]        = list_of(83).convert_to_container<std::vector<unsigned char> >();          // a
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // xprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // xpub

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);
    }

    virtual Network NetworkID() const { return CChainParams::MAIN; }
};
static CMainParams mainParams;

//
// Testnet
//

class CTestNetParams : public CBaseChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strDataDir = "testnet";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xef;
        pchMessageStart[3] = 0xb4;

        vAlertPubKey = ParseHex("0373d8dce43eb98374bcfff2352cd559e6774fd6a87eef73b2fbdb39b2b0bc0082");

        nDefaultPort = 51997;
        nRPCPort = 51996;
        nBIP44ID = 0x80000001;

        nLastPOWBlock = 1000;
        nLastFairLaunchBlock = 1;

        nFirstPosv2Block = 25000;
        nFirstPosv3Block = 25010;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 16);

        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 816452253;
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000438d60fb1a01a92a141f86d367589fd6190727d246ad24ac4119d3e6691"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(127).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(196).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(255).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x76)(0xC0)(0xFD)(0xFB).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x76)(0xC1)(0x07)(0x7A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(75).convert_to_container<std::vector<unsigned char> >();          // X
        base58Prefixes[EXT_ACC_HASH]        = list_of(23).convert_to_container<std::vector<unsigned char> >();          // A
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); // tprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); // tpub

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
//class CRegTestParams : public CTestNetParams {
//public:
//CRegTestParams() {
//strNetworkID = "regtest";
//strDataDir = "regtest";
//nFirstPosv2Block = -1;
//nFirstPosv3Block = -1;
//pchMessageStart[0] = 0xfa;
//pchMessageStart[1] = 0xbf;
//pchMessageStart[2] = 0xb5;
//pchMessageStart[3] = 0xda;
//bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
//genesis.nTime = 1411111111;
//genesis.nBits = bnProofOfWorkLimit.GetCompact();
//genesis.nNonce = 2;
//hashGenesisBlock = genesis.GetHash();
//nDefaultPort = 18444;
//assert(hashGenesisBlock == uint256("0xb86b5854a5a77e1eb0a6023b7d6013ebc9840bd87aad7d9e56da458f87015d3f"));
//vSeeds.clear(); // Regtest mode doesn't have any DNS seeds.
//}
//virtual bool RequireRPCPassword() const { return false; }
//virtual Network NetworkID() const { return CChainParams::REGTEST; }
//};

//static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

const CChainParams &TestNetParams() {
    return testNetParams;
}

const CChainParams &MainNetParams() {
    return mainParams;
}

void SelectParams(CChainParams::Network network)
{
    switch (network)
    {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        //case CChainParams::REGTEST:
        //    pCurrentParams = &regTestParams;
        //    break;
        default:
            assert(false && "Unimplemented network");
            return;
    };
};

bool SelectParamsFromCommandLine()
{
    //bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    //if (fTestNet && fRegTest)
    //{
    //    return false;
    //};

    //if (fRegTest)
    //{
    //    SelectParams(CChainParams::REGTEST);
    if (fTestNet)
    {
        SelectParams(CChainParams::TESTNET);
    } else
    {
        SelectParams(CChainParams::MAIN);
    };

    return true;
}
