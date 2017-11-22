#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

BITCORED=${BITCORED:-$SRCDIR/bitcored}
BITCORECLI=${BITCORECLI:-$SRCDIR/bitcore-cli}
BITCORETX=${BITCORETX:-$SRCDIR/bitcore-tx}
BITCOREQT=${BITCOREQT:-$SRCDIR/qt/bitcore-qt}

[ ! -x $BITCORED ] && echo "$BITCORED not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($BITCORECLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for bitcored if --version-string is not set,
# but has different outcomes for bitcore-qt and bitcore-cli.
echo "[COPYRIGHT]" > footer.h2m
$BITCORED --version | sed -n '1!p' >> footer.h2m

for cmd in $BITCORED $BITCORECLI $BITCORETX $BITCOREQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
