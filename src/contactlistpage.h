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



#ifndef CONTACTLISTPAGE_H
#define CONTACTLISTPAGE_H

#include <MLinearLayoutPolicy>
#include <MComboBox>
#include <MList>

#include <libmeegochat/meegochataccount.h>

#include "acctwizardpage.h"
#include "contactlistitemcreator.h"
#include "acctlistitem.h"
#include "addcontactpage.h"
#include "approvecontactwidget.h"
#include "dialogpage.h"

namespace MeeGoChat {
    class ChatContact;
    class ContactModel;
    class ContactSortFilterProxyModel;
    class AccountModel;
    class AccountManager;
}

using namespace MeeGoChat;

class ContactListPage : public MApplicationPage
{
Q_OBJECT
public:
    ContactListPage() {}
    ContactListPage(DialogPage *dlgPage);
    ~ContactListPage();
    void createContent();

protected:
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
    void onAccountManagerAccountsReady();
    void onAccountManagerAccountCreated(Acct *acct);
    void onAccountCreatedReady(Acct *);
    void onContactManagerReady(ContactManager *cm);
    void contactListItemClicked(const QModelIndex &);
    void acctListItemClicked(const QModelIndex &);
    void onCntActionTriggered();
    void onInbActionTriggered();
    void onCboAcctStatusIdxChanged(int idx);
    void onNoAccounts();
    void onAcctHeaderClicked();
    void onAcctFooterClicked();
    void onAddContactClicked(Acct *acct);
    void resizeViewports();
    void onPresencePublicationRequested(ChatContact *contact);
    void onRequestHandled(ApproveContactWidget *);
    void onFirstStatus(MeeGoChat::Acct::AccountStatus status);
    void onSettingsActionTriggered();

private:
    void updateTimeTexts();
    void setupAcctHeaderFooter(MeeGoChat::Acct *acct);

    AccountManager *mAccountManager;
    QHash<Acct *, ContactModel *> mAcctContactModels;
    ContactSortFilterProxyModel *mContactListModelSorted;
    AccountModel *mAcctListItemModel;
    AddContactPage *mPgAddContact;
    DialogPage *mDialogPage;


    MLayout *mLayout;

    MLinearLayoutPolicy *mMsgPolicy;
    MLinearLayoutPolicy *mCntPolicy;
    MLinearLayoutPolicy *mAcctPolicy;

    MAction *mCntAction;
    MAction *mInbAction;

    MComboBox *mCboAcctStatus;

    MWidgetController *mApproveContactCnt;
    MLayout *mApproveContactLayout;
    MLinearLayoutPolicy *mApproveContactPolicy;

    AcctListItem *mAcctHeader;
    AcctListItem *mAcctFooter;

    MPannableViewport *mContactArea;
    MPannableViewport *mContactMessageArea;
    MPannableViewport *mAcctArea;

    MList *mContactList;
    MList *mContactMessageList;
    MList *mAcctList;

    int mTimerID;
    bool mContactListVisible;
    bool mAccountsReady;    //This is a hack, but needed to avoid a race condition...

};

#endif // CONTACTLISTPAGE_H
