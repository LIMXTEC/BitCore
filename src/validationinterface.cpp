// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2019 Limxtec developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <validationinterface.h>

#include <primitives/block.h>
#include <scheduler.h>
#include <sync.h>
#include <txmempool.h>
#include <util.h>
#include <validation.h>

#include <list>
#include <atomic>
#include <future>

#include <boost/signals2/signal.hpp>

struct MainSignalsInstance {
    // Dash
    boost::signals2::signal<void (const CBlockIndex *)> AcceptedBlockHeader;
    boost::signals2::signal<void (const CBlockIndex *, bool fInitialDownload)> NotifyHeaderTip;
    //

    boost::signals2::signal<void (const CBlockIndex *, const CBlockIndex *, bool fInitialDownload)> UpdatedBlockTip;

    // Dash
    boost::signals2::signal<void (const CTransaction &, const CBlock *)> SyncTransaction;
    //

    boost::signals2::signal<void (const CTransactionRef &)> TransactionAddedToMempool;
    boost::signals2::signal<void (const std::shared_ptr<const CBlock> &, const CBlockIndex *pindex, const std::vector<CTransactionRef>&)> BlockConnected;
    boost::signals2::signal<void (const std::shared_ptr<const CBlock> &)> BlockDisconnected;
    boost::signals2::signal<void (const CTransactionRef &)> TransactionRemovedFromMempool;

    // Dash
    boost::signals2::signal<void (const CTransaction &)> NotifyTransactionLock;
    //

    boost::signals2::signal<void (const CBlockLocator &)> ChainStateFlushed;
    boost::signals2::signal<void (int64_t nBestBlockTime, CConnman* connman)> Broadcast;
    boost::signals2::signal<void (const CBlock&, const CValidationState&)> BlockChecked;
    boost::signals2::signal<void (const CBlockIndex *, const std::shared_ptr<const CBlock>&)> NewPoWValidBlock;

    // We are not allowed to assume the scheduler only runs in one thread,
    // but must ensure all callbacks happen in-order, so we end up creating
    // our own queue here :(
    SingleThreadedSchedulerClient m_schedulerClient;

    explicit MainSignalsInstance(CScheduler *pscheduler) : m_schedulerClient(pscheduler) {}
};

static CMainSignals g_signals;

void CMainSignals::RegisterBackgroundSignalScheduler(CScheduler& scheduler) {
    assert(!m_internals);
    m_internals.reset(new MainSignalsInstance(&scheduler));
}

void CMainSignals::UnregisterBackgroundSignalScheduler() {
    m_internals.reset(nullptr);
}

void CMainSignals::FlushBackgroundCallbacks() {
    if (m_internals) {
        m_internals->m_schedulerClient.EmptyQueue();
    }
}

size_t CMainSignals::CallbacksPending() {
    if (!m_internals) return 0;
    return m_internals->m_schedulerClient.CallbacksPending();
}

void CMainSignals::RegisterWithMempoolSignals(CTxMemPool& pool) {
    pool.NotifyEntryRemoved.connect(boost::bind(&CMainSignals::MempoolEntryRemoved, this, _1, _2));
}

void CMainSignals::UnregisterWithMempoolSignals(CTxMemPool& pool) {
    pool.NotifyEntryRemoved.disconnect(boost::bind(&CMainSignals::MempoolEntryRemoved, this, _1, _2));
}

CMainSignals& GetMainSignals()
{
    return g_signals;
}

void RegisterValidationInterface(CValidationInterface* pwalletIn) {
    // Dash
    g_signals.m_internals->AcceptedBlockHeader.connect(boost::bind(&CValidationInterface::AcceptedBlockHeader, pwalletIn, _1));
    g_signals.m_internals->NotifyHeaderTip.connect(boost::bind(&CValidationInterface::NotifyHeaderTip, pwalletIn, _1, _2));
    //

    g_signals.m_internals->UpdatedBlockTip.connect(boost::bind(&CValidationInterface::UpdatedBlockTip, pwalletIn, _1, _2, _3));

    // Dash
    g_signals.m_internals->SyncTransaction.connect(boost::bind(&CValidationInterface::SyncTransaction, pwalletIn, _1, _2));
    //

    g_signals.m_internals->TransactionAddedToMempool.connect(boost::bind(&CValidationInterface::TransactionAddedToMempool, pwalletIn, _1));
    g_signals.m_internals->BlockConnected.connect(boost::bind(&CValidationInterface::BlockConnected, pwalletIn, _1, _2, _3));
    g_signals.m_internals->BlockDisconnected.connect(boost::bind(&CValidationInterface::BlockDisconnected, pwalletIn, _1));
    g_signals.m_internals->TransactionRemovedFromMempool.connect(boost::bind(&CValidationInterface::TransactionRemovedFromMempool, pwalletIn, _1));

    // Dash
    g_signals.m_internals->NotifyTransactionLock.connect(boost::bind(&CValidationInterface::NotifyTransactionLock, pwalletIn, _1));
    //

    g_signals.m_internals->ChainStateFlushed.connect(boost::bind(&CValidationInterface::ChainStateFlushed, pwalletIn, _1));
    g_signals.m_internals->Broadcast.connect(boost::bind(&CValidationInterface::ResendWalletTransactions, pwalletIn, _1, _2));
    g_signals.m_internals->BlockChecked.connect(boost::bind(&CValidationInterface::BlockChecked, pwalletIn, _1, _2));
    g_signals.m_internals->NewPoWValidBlock.connect(boost::bind(&CValidationInterface::NewPoWValidBlock, pwalletIn, _1, _2));
}

void UnregisterValidationInterface(CValidationInterface* pwalletIn) {
    g_signals.m_internals->BlockChecked.disconnect(boost::bind(&CValidationInterface::BlockChecked, pwalletIn, _1, _2));
    g_signals.m_internals->Broadcast.disconnect(boost::bind(&CValidationInterface::ResendWalletTransactions, pwalletIn, _1, _2));

    // Dash
    g_signals.m_internals->NotifyTransactionLock.disconnect(boost::bind(&CValidationInterface::NotifyTransactionLock, pwalletIn, _1));
    //

    g_signals.m_internals->ChainStateFlushed.disconnect(boost::bind(&CValidationInterface::ChainStateFlushed, pwalletIn, _1));
    g_signals.m_internals->TransactionAddedToMempool.disconnect(boost::bind(&CValidationInterface::TransactionAddedToMempool, pwalletIn, _1));
    g_signals.m_internals->BlockConnected.disconnect(boost::bind(&CValidationInterface::BlockConnected, pwalletIn, _1, _2, _3));
    g_signals.m_internals->BlockDisconnected.disconnect(boost::bind(&CValidationInterface::BlockDisconnected, pwalletIn, _1));
    g_signals.m_internals->TransactionRemovedFromMempool.disconnect(boost::bind(&CValidationInterface::TransactionRemovedFromMempool, pwalletIn, _1));
    g_signals.m_internals->UpdatedBlockTip.disconnect(boost::bind(&CValidationInterface::UpdatedBlockTip, pwalletIn, _1, _2, _3));

    // Dash
    g_signals.m_internals->SyncTransaction.disconnect(boost::bind(&CValidationInterface::SyncTransaction, pwalletIn, _1, _2));
    //

    // Dash
    g_signals.m_internals->NotifyHeaderTip.disconnect(boost::bind(&CValidationInterface::NotifyHeaderTip, pwalletIn, _1, _2));
    g_signals.m_internals->AcceptedBlockHeader.disconnect(boost::bind(&CValidationInterface::AcceptedBlockHeader, pwalletIn, _1));
    //

    g_signals.m_internals->NewPoWValidBlock.disconnect(boost::bind(&CValidationInterface::NewPoWValidBlock, pwalletIn, _1, _2));
}

void UnregisterAllValidationInterfaces() {
    if (!g_signals.m_internals) {
        return;
    }
    g_signals.m_internals->BlockChecked.disconnect_all_slots();
    g_signals.m_internals->Broadcast.disconnect_all_slots();

    // Dash
    g_signals.m_internals->NotifyTransactionLock.disconnect_all_slots();
    //

    g_signals.m_internals->ChainStateFlushed.disconnect_all_slots();
    g_signals.m_internals->TransactionAddedToMempool.disconnect_all_slots();
    g_signals.m_internals->BlockConnected.disconnect_all_slots();
    g_signals.m_internals->BlockDisconnected.disconnect_all_slots();
    g_signals.m_internals->TransactionRemovedFromMempool.disconnect_all_slots();

    // Dash
    g_signals.m_internals->SyncTransaction.disconnect_all_slots();
    //

    g_signals.m_internals->UpdatedBlockTip.disconnect_all_slots();

    // Dash
    g_signals.m_internals->NotifyHeaderTip.disconnect_all_slots();
    g_signals.m_internals->AcceptedBlockHeader.disconnect_all_slots();
    //

    g_signals.m_internals->NewPoWValidBlock.disconnect_all_slots();
}

void CallFunctionInValidationInterfaceQueue(std::function<void ()> func) {
    g_signals.m_internals->m_schedulerClient.AddToProcessQueue(std::move(func));
}

void SyncWithValidationInterfaceQueue() {
    AssertLockNotHeld(cs_main);
    // Block until the validation queue drains
    std::promise<void> promise;
    CallFunctionInValidationInterfaceQueue([&promise] {
        promise.set_value();
    });
    promise.get_future().wait();
}

void CMainSignals::MempoolEntryRemoved(CTransactionRef ptx, MemPoolRemovalReason reason) {
    if (reason != MemPoolRemovalReason::BLOCK && reason != MemPoolRemovalReason::CONFLICT) {
        m_internals->m_schedulerClient.AddToProcessQueue([ptx, this] {
            m_internals->TransactionRemovedFromMempool(ptx);
        });
    }
}

// Dash
void CMainSignals::AcceptedBlockHeader(const CBlockIndex *pindexNew) {
    m_internals->m_schedulerClient.AddToProcessQueue([pindexNew, this] {
        m_internals->AcceptedBlockHeader(pindexNew);
    });
}

void CMainSignals::NotifyHeaderTip(const CBlockIndex *pindexNew, bool fInitialDownload) {
    m_internals->m_schedulerClient.AddToProcessQueue([pindexNew, fInitialDownload, this] {
        m_internals->NotifyHeaderTip(pindexNew, fInitialDownload);
    });
}
//

void CMainSignals::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) {
    // Dependencies exist that require UpdatedBlockTip events to be delivered in the order in which
    // the chain actually updates. One way to ensure this is for the caller to invoke this signal
    // in the same critical section where the chain is updated

    m_internals->m_schedulerClient.AddToProcessQueue([pindexNew, pindexFork, fInitialDownload, this] {
        m_internals->UpdatedBlockTip(pindexNew, pindexFork, fInitialDownload);
    });
}

// Dash
void CMainSignals::SyncTransaction(const CTransaction &tx, const CBlock *pblock) {
    m_internals->m_schedulerClient.AddToProcessQueue([tx, pblock, this] {
        m_internals->SyncTransaction(tx, pblock);
    });
}
//

void CMainSignals::TransactionAddedToMempool(const CTransactionRef &ptx) {
    m_internals->m_schedulerClient.AddToProcessQueue([ptx, this] {
        m_internals->TransactionAddedToMempool(ptx);
    });
}

void CMainSignals::BlockConnected(const std::shared_ptr<const CBlock> &pblock, const CBlockIndex *pindex, const std::shared_ptr<const std::vector<CTransactionRef>>& pvtxConflicted) {
    m_internals->m_schedulerClient.AddToProcessQueue([pblock, pindex, pvtxConflicted, this] {
        m_internals->BlockConnected(pblock, pindex, *pvtxConflicted);
    });
}

void CMainSignals::BlockDisconnected(const std::shared_ptr<const CBlock> &pblock) {
    m_internals->m_schedulerClient.AddToProcessQueue([pblock, this] {
        m_internals->BlockDisconnected(pblock);
    });
}

// Dash
void CMainSignals::NotifyTransactionLock(const CTransaction &tx) {
    m_internals->m_schedulerClient.AddToProcessQueue([tx, this] {
        m_internals->NotifyTransactionLock(tx);
    });
}
//

void CMainSignals::ChainStateFlushed(const CBlockLocator &locator) {
    m_internals->m_schedulerClient.AddToProcessQueue([locator, this] {
        m_internals->ChainStateFlushed(locator);
    });
}

void CMainSignals::Broadcast(int64_t nBestBlockTime, CConnman* connman) {
    m_internals->Broadcast(nBestBlockTime, connman);
}

void CMainSignals::BlockChecked(const CBlock& block, const CValidationState& state) {
    m_internals->BlockChecked(block, state);
}

void CMainSignals::NewPoWValidBlock(const CBlockIndex *pindex, const std::shared_ptr<const CBlock> &block) {
    m_internals->NewPoWValidBlock(pindex, block);
}
