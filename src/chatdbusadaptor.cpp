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

#include "chatdbusadaptor.h"
#include "dialogpage.h"

#include <MApplicationWindow>

#include <libmeegochat/meegochataccountmanager.h>
#include <libmeegochat/meegochatcontact.h>
#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochatcontactmanager.h>
#include <libmeegochat/meegochatmessage.h>

using namespace MeeGoChat;

ChatDBusAdaptor::ChatDBusAdaptor(MApplicationWindow *appWin, DialogPage *dlgPage, QObject *parent) :
    QDBusAbstractAdaptor(parent),
    mAppWin(appWin),
    mAcctMgr(AccountManager::getInstance()),
    mDlgPage(dlgPage)
{}

//Public slots/interfaces
bool ChatDBusAdaptor::showDialogPageForContact(QString tpAcctPath, QString tpChatID)
{
    return showDialogPage(getContact(tpAcctPath, tpChatID));
}

bool ChatDBusAdaptor::showDialogPageAndSendMessage(QString tpAcctPath, QString tpChatID, QString message)
{
    ChatContact *contact = getContact(tpAcctPath, tpChatID);
    if (!contact)
        return false;
    sendMessage(contact, message);
    //TODO - change in the future to notify of message success/state (sent/pending)
    return showDialogPage(contact);
}

bool ChatDBusAdaptor::sendMessage(QString tpAcctPath, QString tpChatID, QString message)
{
    Message *msg = sendMessage(getContact(tpAcctPath, tpChatID), message);
    return (msg);
    //TODO - change this in the future to notify of the message state - sent/pending
}

//Private functions

ChatContact * ChatDBusAdaptor::getContact(QString tpAcctPath, QString tpChatID)
{
    if (!mAcctMgr)
        return 0;

    return mAcctMgr->getContactByPathAndID(tpAcctPath, tpChatID);
}

bool ChatDBusAdaptor::showDialogPage(ChatContact *contact)
{
    if (!mAppWin || !mDlgPage || !contact)
        return false;

    mDlgPage->setContact(contact);
    mDlgPage->appear();
    //In case we're running in headless mode...
    mAppWin->show();
    mAppWin->activateWindow();
    return true;
}

Message * ChatDBusAdaptor::sendMessage(ChatContact *contact, QString message)
{
    if (!contact)
        return false;

    return contact->sendMessage(message);
}
