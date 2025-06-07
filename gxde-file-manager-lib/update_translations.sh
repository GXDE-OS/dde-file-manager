#!/bin/bash
# this file is used to auto update ts file.

lupdate -pro gxde-file-manager-lib.pro -ts translations/gxde-file-manager.ts -no-obsolete
lupdate -pro gxde-file-manager-lib.pro -ts translations/gxde-file-manager_zh_CN.ts -no-obsolete