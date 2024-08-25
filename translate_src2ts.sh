#!/bin/bash
cd `dirname $0`
lupdate -pro dde-desktop/dde-desktop.pro -ts dde-desktop/translations/dde-desktop_*.ts
lupdate -pro dde-file-manager/dde-file-manager.pro -ts dde-file-manager/translations/dde-file-manager-app_*.ts
lupdate -pro dde-file-manager-lib/dde-file-manager-lib.pro -ts dde-file-manager-lib/translations/dde-file-manager_*.ts
lupdate -pro usb-device-formatter/usb-device-formatter.pro -ts usb-device-formatter/translations/usb-device-formatter_*.ts
#lupdate -pro  -ts dde-zone/translations/dde-zone*.ts
