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


#include "addcontactpage.h"

#include <MLayout>
#include <MGridLayoutPolicy>
#include <MComboBox>
#include <MLabel>
#include <MMessageBox>

#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochatcontactmanager.h>


AddContactPage::AddContactPage(MeeGoChat::ContactManager *cm) :
    MApplicationPage(0),
    mContactMgr(cm)
{
    //% "Add Contact"
    setTitle(qtTrId("page_title_add_contact"));

    connect(mContactMgr,
            SIGNAL(AddContactSuccess(QString)),
            this,
            SLOT(onAddContactSuccess(QString)));
    connect(mContactMgr,
            SIGNAL(AddContactError(QString,QString)),
            this,
            SLOT(onAddContactError(QString, QString)));

    //TODO: Create proxyfilter model for ALIM -
    //filter acct types that shouldn't be listed:
    //not online/connected, no "add contact" support
    //(eg IRC, local-xmpp)

    MLayout *layout = new MLayout();
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);

    //% "Account:"
    MLabel *lblAcct = new MLabel(qtTrId("lbl_account"));
    lblAcct->setObjectName("lblAddContactAcct");
    MLabel *lblAcctName = new MLabel(mContactMgr->getAccount()->getDisplayName());
    lblAcctName->setObjectName("lblAddContactAcctName");
    policy->addItem(lblAcct, 0, 0, Qt::AlignLeft);
    policy->addItem(lblAcctName, 0, 1, Qt::AlignLeft);

    //% "Email/ID:"
    MLabel *lblContactID = new MLabel(qtTrId("lbl_chat_id"));
    mContactID = new MTextEdit();

    //See TODO note below...
    //MLabel *lblContactAlias = new MLabel("Alias:");
    //mContactAlias = new MTextEdit();

    mMessageLabel = new MLabel("");
    mMessageLabel->setObjectName("AddContactPageMessageLabel");


    //% "Add Contact"
    mBtnAdd = new MButton(qtTrId("btn_add_contact"));
    mBtnAdd->setEnabled(false);
    //% "Cancel"
    MButton *btnCancel = new MButton(qtTrId("btn_cancel"));

    connect(mBtnAdd,
            SIGNAL(clicked()),
            this,
            SLOT(onAddClicked()));
    connect(btnCancel,
            SIGNAL(clicked()),
            this,
            SLOT(dismiss()));

    connect(mContactID,
            SIGNAL(textChanged()),
            this,
            SLOT(onMContactIDTextChanged()));
    connect(mContactID,
            SIGNAL(returnPressed()),
            mBtnAdd,
            SIGNAL(clicked()));
//    connect(mContactAlias,
//            SIGNAL(returnPressed()),
//            mBtnAdd,
//            SIGNAL(clicked()));



    policy->addItem(lblContactID, 1, 0, Qt::AlignLeft);
    policy->addItem(mContactID, 1, 1, Qt::AlignLeft);
    //TODO: Telepathy-Qt4 doesn't seem to have a way right now
    //to set a Contact's alias - will investigate further...
//    policy->addItem(lblContactAlias, 2, 0, Qt::AlignLeft);
//    policy->addItem(mContactAlias, 2, 1, Qt::AlignLeft);
    policy->addItem(mMessageLabel, 2, 0, 2, 1, Qt::AlignLeft);
    policy->addItem(new MWidgetController(), 3, 0, 1, 2);   //Spacer
    policy->addItem(mBtnAdd, 4, 0, 1, 2, Qt::AlignCenter);
    policy->addItem(btnCancel, 5, 0, 1, 2, Qt::AlignCenter);

    layout->setPolicy(policy);
    centralWidget()->setLayout(layout);

}

void AddContactPage::onAddClicked()
{
    if (!mContactMgr || mContactID->text().isEmpty()) {
        //% "Please specify an Email/ID!"
        mMessageLabel->setText(qtTrId("lbl_contact_id_is_empty"));
        return;
    }
    //% "Processing..."
    mMessageLabel->setText(qtTrId("lbl_processing"));
    mBtnAdd->setEnabled(false);
    mContactMgr->addContact(mContactID->text());
}

void AddContactPage::onMContactIDTextChanged()
{
    mBtnAdd->setEnabled(!mContactID->text().isEmpty());
}

void AddContactPage::onAddContactSuccess(QString)
{
    //% "Success!"
    mMessageLabel->setText(qtTrId("lbl_success"));
    this->dismiss();
}

void AddContactPage::onAddContactError(QString contactID, QString errMsg)
{
    //% "Error adding contact %1: %2 - %1 is contact ID, %2 is error message"
    mMessageLabel->setText(qtTrId("Error adding contact %1: %2").arg(contactID, errMsg));
    mContactID->setFocus();
}
