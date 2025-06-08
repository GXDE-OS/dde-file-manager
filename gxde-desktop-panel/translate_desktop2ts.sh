#!/bin/bash

DESKTOP_COMPUTER_SOURCE_FILE=data/applications/gxde-computer.desktop
DESKTOP_COMPUTER_TS_DIR=translations/gxde-computer-desktop/

/usr/bin/deepin-desktop-ts-convert desktop2ts $DESKTOP_COMPUTER_SOURCE_FILE $DESKTOP_COMPUTER_TS_DIR


DESKTOP_TRASH_SOURCE_FILE=data/applications/gxde-trash.desktop
DESKTOP_TRASH_TS_DIR=translations/gxde-trash-desktop/

/usr/bin/deepin-desktop-ts-convert desktop2ts $DESKTOP_TRASH_SOURCE_FILE $DESKTOP_TRASH_TS_DIR
