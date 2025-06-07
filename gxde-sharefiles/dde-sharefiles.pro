TEMPLATE = subdirs
CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../gxde-file-manager-lib/release -lgxde-file-manager
SUBDIRS = helpers models views controllers

controllers.depends = helpers models
models.depends = helpers
views.depends = helpers models


APPSHAREDIR = /usr/share/gxde-file-manager/sharefiles

config.files = $$PWD/config/*
config.path = $$APPSHAREDIR/config


public.files = $$PWD/public/*
public.path = $$APPSHAREDIR/public

INSTALLS += config public
