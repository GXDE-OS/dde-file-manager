#!/bin/bash
cd `dirname $0`
lupdate -pro dde-desktop/dde-desktop.pro -ts dde-desktop/translations/dde-desktop_*.ts
lupdate -pro dde-file-manager/dde-file-manager.pro -ts dde-file-manager/translations/dde-file-manager-app_*.ts
# 先临时处理 dde-file-manager-lib/configure/global-setting-template.js
cp dde-file-manager-lib/configure/global-setting-template.js dde-file-manager-lib/configure/global-setting-template.js-bak
sed -i '2s/"groups":/groups:/g' dde-file-manager-lib/configure/global-setting-template.js
lupdate -pro dde-file-manager-lib/dde-file-manager-lib.pro -ts dde-file-manager-lib/translations/dde-file-manager_*.ts
# 处理完后重新改回
mv dde-file-manager-lib/configure/global-setting-template.js-bak dde-file-manager-lib/configure/global-setting-template.js
lupdate -pro usb-device-formatter/usb-device-formatter.pro -ts usb-device-formatter/translations/usb-device-formatter_*.ts
