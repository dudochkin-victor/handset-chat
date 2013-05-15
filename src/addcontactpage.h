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



#ifndef ADDCONTACTPAGE_H
#define ADDCONTACTPAGE_H

#include <MApplicationPage>
#include <MTextEdit>
#include <MButton>
#include <MLabel>

namespace MeeGoChat {
    class ContactManager;
}

using namespace MeeGoChat;

class AddContactPage : public MApplicationPage
{
Q_OBJECT
public:
    AddContactPage() {}
    AddContactPage(ContactManager *cm);

private slots:
    void onAddClicked();
    void onMContactIDTextChanged();
    void onAddContactSuccess(QString);
    void onAddContactError(QString, QString);
private:
    ContactManager *mContactMgr;
    MTextEdit *mContactID;
    //MTextEdit *mContactAlias;
    MLabel *mMessageLabel;
    MButton *mBtnAdd;
};

#endif // ADDCONTACTPAGE_H
