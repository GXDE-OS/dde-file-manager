#!/usr/bin/env python3
import os
for i in os.listdir("."):
    os.rename(i, i.replace("dde-desktop", "gxde-desktop-panel").replace("dde-", "gxde-").replace("deepin", "gxde"))
