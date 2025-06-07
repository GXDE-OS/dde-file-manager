#!/bin/bash
# this file is used to auto update ts file.

lupdate -pro gxde-desktop-panel.pro -ts translations/gxde-desktop-panel.ts -no-obsolete
lupdate -pro gxde-desktop-panel.pro -ts translations/gxde-desktop-panel_zh_CN.ts -no-obsolete
