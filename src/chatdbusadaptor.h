/*
 * meego-handset-chat - Meego Handset Chat application
 *
 * Copyright (c) 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef CHATDBUSADAPTOR_H
#define CHATDBUSADAPTOR_H

#include <QObject>
#include <QString>
#include <QDBusAbstractAdaptor>

#define CLIENTNAME "meego_handset_chat"
#define DBUSINTFNAME QString("com.meego.").append(CLIENTNAME)
#define DBUSOBJPATH QString("/com/meego/").append(CLIENTNAME)


namespace MeeGoChat {
    class AccountManager;
    class Message;
    class ChatContact;
}

class DialogPage;

class MApplicationWindow;

class ChatDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.meego_handset_chat")

public:
    ChatDBusAdaptor(MApplicationWindow *appWin, DialogPage *dlgPage, QObject *parent = 0);

public slots:
    bool showDialogPageForContact(QString tpAcctPath, QString tpChatID);
    bool showDialogPageAndSendMessage(QString tpAcctPath, QString tpChatID, QString message);
    bool sendMessage(QString tpAcctPath, QString tpChatID, QString message);

private:
    MeeGoChat::ChatContact * getContact(QString tpAcctPath, QString tpChatID);
    bool showDialogPage(MeeGoChat::ChatContact *contact);
    MeeGoChat::Message * sendMessage(MeeGoChat::ChatContact *contact, QString message);

    MApplicationWindow *mAppWin;
    MeeGoChat::AccountManager *mAcctMgr;
    DialogPage *mDlgPage;

};

#endif // CHATDBUSADAPTOR_H
