elcome to Bitcore BTX 
=====================================

![GitHub Logo](https://i.imgur.com/jnpy4Me.jpg)
[ Release-Notes ](https://www.reddit.com/r/bitcore_btx/comments/fsbzog/odarhom_release_notes_short_overview_first_draft/) - [ FAQ ](https://www.reddit.com/r/bitcore_btx/wiki/index/odarhom_faq) - [ Website ](https://www.bitcore.cc) - [ Bitcointalk ](https://bitcointalk.org/index.php?topic=1883902.0) 

[![Build Status](https://travis-ci.org/LIMXTEC/BitCore.svg?branch=0.15)](https://travis-ci.org/LIMXTEC/BitCore)

What is BitCore?
----------------

Established on 24th of April 2017, BitCore from the community for the community. BitCore has one of the lowest transaction fees on the market while possibly providing the best on-chain scaling solution in the  entire crypto-sphere. Take a look at our [comparison chart](https://bitcore.cc/wp-content/uploads/2019/11/BitCoreVsAgosto2019.jpg).

Bitcore BTX preserves and implements Bitcoin features such as SegWit, which enables advanced scaling technologies like Lightning Network. Significant differences at launch time included:

- Unique wallet addresses (prefixes of 2 and S and btx (BIP 173 bench32)
- Command Fork System
- Masternode System Dash Core
- 220 Byte Datacarriersize (OP_RETURN)
- Hashalgorythm Timetravel10
- Bitcore Diffshield
- Travis CI support

For more information, as well as an immediately useable, binary version of
the BitCore software, see [https://bitcore.cc](https://bitcore.cc).

Claiming for Bitcoin holders
--
Claiming for the first snapshot (Bitcoin block #463604) ended 30th of October 2017. We made a new snapshot of the Bitcoin network on 2nd of November 2017 (Bitcoin block #492820). These ~5 million addresses are able to claim BitCore BTX for free, right now! Read how to [here](https://steemit.com/crypto-news/@xwerk/bitcore-btx-guide-the-2nd-snapshot-for-btc-hodlers-free-btx-or-how-it-works).




Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Release](https://github.com/LIMXTEC/bitcore/release) are created
regularly to indicate new official, stable release versions of BitCore.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The developer [mailing list](https://groups.google.com/forum/#!forum/bitcore-dev)
should be used to discuss complicated or controversial changes before working
on a patch set.

Developer IRC can be found on Freenode at #bitcore-dev.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

License
-------

BitCore is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.
