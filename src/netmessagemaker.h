// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2009-2019 The Litecoin Core developers
// Copyright (c) 2017-2019 The Bitcore Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCORE_NETMESSAGEMAKER_H
#define BITCORE_NETMESSAGEMAKER_H

#include "net.h"
#include "serialize.h"

class CNetMsgMaker
{
public:
    CNetMsgMaker(int nVersionIn) : nVersion(nVersionIn){}

    template <typename... Args>
    CSerializedNetMsg Make(int nFlags, std::string sCommand, Args&&... args) const
    {
        CSerializedNetMsg msg;
        msg.command = std::move(sCommand);
        CVectorWriter{ SER_NETWORK, nFlags | nVersion, msg.data, 0, std::forward<Args>(args)... };
        return msg;
    }

    template <typename... Args>
    CSerializedNetMsg Make(std::string sCommand, Args&&... args) const
    {
        return Make(0, std::move(sCommand), std::forward<Args>(args)...);
    }

private:
    const int nVersion;
};

#endif // BITCORE_NETMESSAGEMAKER_H
