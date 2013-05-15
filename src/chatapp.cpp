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



#include "chatapp.h"
#include "contactlistpage.h"
#include "dialogpage.h"
#include "chatdbusadaptor.h"

ChatApp::ChatApp(int &argc, char**argv)
        : MApplication(argc, argv),
        mClient(MeeGoChat::ClientHandler::createClient(CLIENTNAME, true, true))
{

    mWindow = new MApplicationWindow();
    if (!this->arguments().contains("--hidden"))
        mWindow->show();

    DialogPage *dlgPage = new DialogPage();

    new ChatDBusAdaptor(mWindow, dlgPage, this);
    QDBusConnection::sessionBus().registerObject(DBUSOBJPATH, this);
    QDBusConnection::sessionBus().registerService(DBUSINTFNAME);


    mContactListPage = new ContactListPage(dlgPage);
    mContactListPage->appear();

}

ChatApp::~ChatApp()
{
    delete mContactListPage;
    delete mWindow;
    delete mClient;
}
