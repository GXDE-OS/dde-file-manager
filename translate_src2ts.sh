#!/bin/bash
cd `dirname $0`
lupdate -recursive \
 dde-desktop/ \
 dde-wallpaper-clooser \
 dde-zone \
 -ts dde-desktop/translations/dde-desktop_*.ts
lupdate -recursive dde-file-manager/ -ts dde-file-manager/translations/dde-file-manager-app_*.ts
lupdate -recursive \
 dialogs/ \
 dde-dock-plugins \
 dde-file-manager-lib/ \
 dde-advanced-property-plugin \
 dde-file-manager-daemon \
 dde-file-manager-plugins \
 dde-file-thumbnail-tool \
 deepin-anything-server-plugins \
 fileoperations \
 partman \
 utils \
 usershare \
 -ts dde-file-manager-lib/translations/dde-file-manager_*.ts
lupdate -recursive usb-device-formatter/ -ts usb-device-formatter/translations/usb-device-formatter_*.ts
