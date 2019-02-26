#-------------------------------------------------
#
# Project created by QtCreator 2018-04-12T16:18:52
#
#-------------------------------------------------

QT       += core gui sql
QT       += network
QT += serialport

QT += axcontainer #导出Excel

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PropertyManagementSystem
TEMPLATE = app

RC_FILE += propertymanagementsystem.rc


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    login_widget.cpp \
    admin_mainwindow.cpp \
    user_mainwindow.cpp \
    connectionpool.cpp \
    usermanager.cpp \
    carmanage.cpp \
    housemanage.cpp \
    mattermange.cpp \
    chargemanage.cpp \
    pay_manage.cpp \
    complaint_manage.cpp \
    log_manage.cpp \
    xiaoqu_manage.cpp \
    userfamily_manage.cpp \
    export_excel.cpp \
    room_manage.cpp \
    add_room.cpp \
    admin_update_dialog.cpp \
    admin_signup_dialog.cpp \
    user_signup_dialog.cpp \
    xqinfo_user.cpp \
    carinfo_user.cpp \
    complaintinfo_user.cpp \
    kftel_dialog.cpp \
    complaints_managebyuser.cpp \
    userfamilyinfo.cpp \
    changeuserpwd.cpp \
    view_chargeitems.cpp \
    view_userlogs.cpp \
    backupsql.cpp \
    pay_dialog.cpp

HEADERS += \
    login_widget.h \
    admin_mainwindow.h \
    user_mainwindow.h \
    connectionpool.h \
    usermanager.h \
    carmanage.h \
    housemanage.h \
    mattermange.h \
    chargemanage.h \
    pay_manage.h \
    complaint_manage.h \
    log_manage.h \
    xiaoqu_manage.h \
    userfamily_manage.h \
    export_excel.h \
    room_manage.h \
    add_room.h \
    admin_update_dialog.h \
    admin_signup_dialog.h \
    user_signup_dialog.h \
    xqinfo_user.h \
    carinfo_user.h \
    complaintinfo_user.h \
    kftel_dialog.h \
    complaints_managebyuser.h \
    userfamilyinfo.h \
    changeuserpwd.h \
    view_chargeitems.h \
    view_userlogs.h \
    backupsql.h \
    pay_dialog.h

FORMS += \
    login_widget.ui \
    admin_mainwindow.ui \
    user_mainwindow.ui \
    usermanager.ui \
    carmanage.ui \
    housemanage.ui \
    mattermange.ui \
    chargemanage.ui \
    pay_manage.ui \
    complaint_manage.ui \
    log_manage.ui \
    xiaoqu_manage.ui \
    userfamily_manage.ui \
    room_manage.ui \
    add_room.ui \
    admin_update_dialog.ui \
    admin_signup_dialog.ui \
    user_signup_dialog.ui \
    xqinfo_user.ui \
    carinfo_user.ui \
    complaintinfo_user.ui \
    kftel_dialog.ui \
    complaints_managebyuser.ui \
    userfamilyinfo.ui \
    changeuserpwd.ui \
    view_chargeitems.ui \
    view_userlogs.ui \
    backupsql.ui \
    pay_dialog.ui

RESOURCES += \
    mysource.qrc

