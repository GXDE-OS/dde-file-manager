#!/bin/bash
cd `dirname $0`
lupdate -recursive dde-desktop/ -ts dde-desktop/translations/dde-desktop_*.ts
lupdate -recursive dde-file-manager/ -ts dde-file-manager/translations/dde-file-manager-app_*.ts
lupdate -recursive dde-file-manager-lib/ -ts dde-file-manager-lib/translations/dde-file-manager_*.ts
lupdate -recursive usb-device-formatter/ -ts usb-device-formatter/translations/usb-device-formatter_*.ts
