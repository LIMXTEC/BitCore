#!/usr/bin/env bash
#
# Copyright (c) 2018 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#
# Check for circular dependencies

export LC_ALL=C

EXPECTED_CIRCULAR_DEPENDENCIES=(
    "chainparamsbase -> util -> chainparamsbase"
    "checkpoints -> validation -> checkpoints"
    "index/txindex -> validation -> index/txindex"
    "policy/fees -> txmempool -> policy/fees"
    "policy/policy -> validation -> policy/policy"
    "qt/addresstablemodel -> qt/walletmodel -> qt/addresstablemodel"
    "qt/bantablemodel -> qt/clientmodel -> qt/bantablemodel"
    "qt/bitcoingui -> qt/utilitydialog -> qt/bitcoingui"
    "qt/bitcoingui -> qt/walletframe -> qt/bitcoingui"
    "qt/bitcoingui -> qt/walletview -> qt/bitcoingui"
    "qt/clientmodel -> qt/peertablemodel -> qt/clientmodel"
    "qt/paymentserver -> qt/walletmodel -> qt/paymentserver"
    "qt/recentrequeststablemodel -> qt/walletmodel -> qt/recentrequeststablemodel"
    "qt/sendcoinsdialog -> qt/walletmodel -> qt/sendcoinsdialog"
    "qt/transactiontablemodel -> qt/walletmodel -> qt/transactiontablemodel"
    "qt/walletmodel -> qt/walletmodeltransaction -> qt/walletmodel"
    "rpc/rawtransaction -> wallet/rpcwallet -> rpc/rawtransaction"
    "txmempool -> validation -> txmempool"
    "validation -> validationinterface -> validation"
    "wallet/coincontrol -> wallet/wallet -> wallet/coincontrol"
    "wallet/fees -> wallet/wallet -> wallet/fees"
    "wallet/wallet -> wallet/walletdb -> wallet/wallet"
    "policy/fees -> policy/policy -> validation -> policy/fees"
    "policy/rbf -> txmempool -> validation -> policy/rbf"
    "qt/addressbookpage -> qt/bitcoingui -> qt/walletview -> qt/addressbookpage"
    "qt/guiutil -> qt/walletmodel -> qt/optionsmodel -> qt/guiutil"
    "txmempool -> validation -> validationinterface -> txmempool"
    "qt/addressbookpage -> qt/bitcoingui -> qt/walletview -> qt/receivecoinsdialog -> qt/addressbookpage"
    "qt/addressbookpage -> qt/bitcoingui -> qt/walletview -> qt/signverifymessagedialog -> qt/addressbookpage"
    "qt/guiutil -> qt/walletmodel -> qt/optionsmodel -> qt/intro -> qt/guiutil"
    "qt/addressbookpage -> qt/bitcoingui -> qt/walletview -> qt/sendcoinsdialog -> qt/sendcoinsentry -> qt/addressbookpage"

    # Dash
    "activemasternode -> masternode -> activemasternode"
    "activemasternode -> masternode-sync -> activemasternode"
    "activemasternode -> masternodeman -> activemasternode"
    "governance -> governance-classes -> governance"
    "governance -> governance-object -> governance"
    "governance -> masternode-sync -> governance"
    "governance -> masternodeman -> governance"
    "governance -> net_processing -> governance"
    "governance-object -> governance-vote -> governance-object"
    "init -> privatesend-client -> init"
    "init -> privatesend-server -> init"
    "instantx -> net -> instantx"
    "instantx -> wallet/wallet -> instantx"
    "masternode -> masternode-payments -> masternode"
    "masternode -> masternode-sync -> masternode"
    "masternode -> masternodeman -> masternode"
    "masternode-payments -> masternode-sync -> masternode-payments"
    "masternode-payments -> masternodeman -> masternode-payments"
    "masternode-payments -> net_processing -> masternode-payments"
    "masternode-sync -> masternodeman -> masternode-sync"
    "masternode-sync -> net -> masternode-sync"
    "masternodeman -> privatesend-client -> masternodeman"
    "net -> netmessagemaker -> net"
    "net_processing -> spork -> net_processing"
    "privatesend-client -> wallet/wallet -> privatesend-client"
    "spork -> sporkdb -> spork"
    "activemasternode -> masternode -> init -> activemasternode"
    "activemasternode -> masternode -> masternode-payments -> activemasternode"
    "activemasternode -> net -> privatesend -> activemasternode"
    "dsnotificationinterface -> privatesend -> init -> dsnotificationinterface"
    "governance -> governance-classes -> init -> governance"
    "governance -> net -> privatesend -> governance"
    "governance -> masternode -> wallet/wallet -> governance"
    "governance-classes -> init -> masternode-payments -> governance-classes"
    "init -> privatesend-client -> masternode -> init"
    "init -> net -> privatesend -> init"
    "instantx -> net -> privatesend -> instantx"
    "masternode -> masternodeman -> privatesend-client -> masternode"
    "masternode-payments -> masternode-sync -> validation -> masternode-payments"
    "masternode-sync -> net -> privatesend -> masternode-sync"
    "masternode-sync -> masternodeman -> privatesend-client -> masternode-sync"
    "masternodeman -> privatesend-client -> privatesend -> masternodeman"
    "masternodeman -> messagesigner -> validation -> masternodeman"
    "pow -> spork -> validation -> pow"
    "masternode-payments -> masternode-sync -> net -> privatesend -> masternode-payments"
    "masternodeman -> privatesend-client -> wallet/wallet -> net -> masternodeman"
    "pow -> spork -> validation -> txdb -> pow"
    "activemasternode -> masternode -> masternode-payments -> net_processing -> instantx -> activemasternode"
    "activemasternode -> masternode -> masternode-payments -> net_processing -> privatesend-server -> activemasternode"
)

EXIT_CODE=0

CIRCULAR_DEPENDENCIES=()

IFS=$'\n'
for CIRC in $(cd src && ../contrib/devtools/circular-dependencies.py {*,*/*,*/*/*}.{h,cpp} | sed -e 's/^Circular dependency: //'); do
    CIRCULAR_DEPENDENCIES+=($CIRC)
    IS_EXPECTED_CIRC=0
    for EXPECTED_CIRC in "${EXPECTED_CIRCULAR_DEPENDENCIES[@]}"; do
        if [[ "${CIRC}" == "${EXPECTED_CIRC}" ]]; then
            IS_EXPECTED_CIRC=1
            break
        fi
    done
    if [[ ${IS_EXPECTED_CIRC} == 0 ]]; then
        echo "A new circular dependency in the form of \"${CIRC}\" appears to have been introduced."
        echo
        EXIT_CODE=1
    fi
done

for EXPECTED_CIRC in "${EXPECTED_CIRCULAR_DEPENDENCIES[@]}"; do
    IS_PRESENT_EXPECTED_CIRC=0
    for CIRC in "${CIRCULAR_DEPENDENCIES[@]}"; do
        if [[ "${CIRC}" == "${EXPECTED_CIRC}" ]]; then
            IS_PRESENT_EXPECTED_CIRC=1
            break
        fi
    done
    if [[ ${IS_PRESENT_EXPECTED_CIRC} == 0 ]]; then
        echo "Good job! The circular dependency \"${EXPECTED_CIRC}\" is no longer present."
        echo "Please remove it from EXPECTED_CIRCULAR_DEPENDENCIES in $0"
        echo "to make sure this circular dependency is not accidentally reintroduced."
        echo
        EXIT_CODE=1
    fi
done

exit ${EXIT_CODE}
