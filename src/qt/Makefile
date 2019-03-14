.PHONY: FORCE
all: FORCE
        $(MAKE) -C .. bitcore_qt test_bitcore_qt
clean: FORCE
        $(MAKE) -C .. bitcore_qt_clean test_bitcore_qt_clean
check: FORCE
        $(MAKE) -C .. test_bitcore_qt_check
bitcore-qt bitcore-qt.exe: FORCE
         $(MAKE) -C .. bitcore_qt
