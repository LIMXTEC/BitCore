// Copyright (c) 2011-2014 The BitCore Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCORE_QT_BITCOREADDRESSVALIDATOR_H
#define BITCORE_QT_BITCOREADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BitCoreAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitCoreAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** BitCore address widget validator, checks for a valid bitcore address.
 */
class BitCoreAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BitCoreAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BITCORE_QT_BITCOREADDRESSVALIDATOR_H
