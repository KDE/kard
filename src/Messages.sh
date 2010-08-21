#! /usr/bin/env bash
$EXTRACTRC *.rc *.ui *.kcfg >> rc.cpp
$XGETTEXT `find . -name '*.cpp'` -o $podir/kard.pot


