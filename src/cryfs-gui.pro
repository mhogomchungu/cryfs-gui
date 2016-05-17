
SOURCES += lxqt_wallet/frontend/password_dialog.cpp \
    lxqt_wallet/frontend/task.cpp \
    lxqt_wallet/frontend/lxqt_wallet.cpp \
    lxqt_wallet/frontend/lxqt_secret_service.cpp \
    lxqt_wallet/frontend/lxqt_kwallet.cpp \
    lxqt_wallet/frontend/lxqt_internal_wallet.cpp \
    lxqt_wallet/frontend/changepassworddialog.cpp \
    checkforupdates.cpp \
    favorites.cpp \
    oneinstance.cpp \
    utility.cpp  \
    cryfs-gui.cpp  \
    keydialog.cpp  \
    plugin.cpp   \
    walletconfig.cpp   \
    cryfstask.cpp  \
    main.cpp  \
    readonlywarning.cpp \
    walletconfiginput.cpp \
    dialogmsg.cpp \
    monitor_mountinfo.cpp \
    tablewidget.cpp

HEADERS += checkforupdates.h \
    favorites.h \
    oneinstance.h \
    utility.h  \
    cryfs-gui.h  \
    keydialog.h  \
    plugin.h   \
    walletconfig.h   \
    cryfstask.h  \
    readonlywarning.h \
    walletconfiginput.h \
    dialogmsg.h \
    monitor_mountinfo.h \
    tablewidget.h \
    lxqt_wallet/backend/lxqtwallet.h \
    lxqt_wallet/frontend/password_dialog.h \
    lxqt_wallet/frontend/task.h \
    lxqt_wallet/frontend/lxqt_wallet.h \
    lxqt_wallet/frontend/lxqt_secret_service.h \
    lxqt_wallet/frontend/lxqt_kwallet.h \
    lxqt_wallet/frontend/lxqt_internal_wallet.h \
    lxqt_wallet/frontend/changepassworddialog.h \

FORMS += cryfs-gui.ui \
    favorites.ui \
    plugin.ui \
    walletconfiginput.ui \
    dialogmsg.ui \
    keydialog.ui \
    readonlywarning.ui \
    walletconfig.ui \
    lxqt_wallet/frontend/password_dialog.ui \
    lxqt_wallet/frontend/changepassworddialog.ui

TRANSLATIONS = ../translations/en_US.ts
