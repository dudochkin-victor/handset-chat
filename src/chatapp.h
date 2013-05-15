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



#ifndef CHATAPP_H
#define CHATAPP_H

#include <QObject>
#include <MApplication>
#include <MApplicationWindow>
#include <libmeegochat/meegochatclienthandler.h>

class ContactListPage;

class ChatApp : public MApplication
{
    Q_OBJECT

public:
        ChatApp(int &argc, char **argv);
        ~ChatApp();

private:
        MeeGoChat::ClientHandler *mClient;
        MApplicationWindow *mWindow;
        ContactListPage *mContactListPage;

};


#endif // CHATAPP_H
