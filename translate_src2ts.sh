#!/bin/bash
cd `dirname $0`
lupdate -pro gxde-desktop-panel/gxde-desktop-panel.pro -ts gxde-desktop-panel/translations/gxde-desktop-panel_*.ts
lupdate -pro gxde-file-manager/gxde-file-manager.pro -ts gxde-file-manager/translations/gxde-file-manager-app_*.ts
# 先临时处理 gxde-file-manager-lib/configure/global-setting-template.js
cp gxde-file-manager-lib/configure/global-setting-template.js gxde-file-manager-lib/configure/global-setting-template.js-bak
sed -i '2s/"groups":/groups:/g' gxde-file-manager-lib/configure/global-setting-template.js
lupdate -pro gxde-file-manager-lib/gxde-file-manager-lib.pro -ts gxde-file-manager-lib/translations/gxde-file-manager_*.ts
# 处理完后重新改回
mv gxde-file-manager-lib/configure/global-setting-template.js-bak gxde-file-manager-lib/configure/global-setting-template.js
lupdate -pro usb-device-formatter/usb-device-formatter.pro -ts usb-device-formatter/translations/usb-device-formatter_*.ts
