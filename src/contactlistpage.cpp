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



#include <QDebug>
#include <MWidgetCreator>
#include <MAction>
#include <MPannableViewport>

#include <DuiControlPanelIf>

#include <libmeegochat/meegochatcontact.h>
#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochataccountmanager.h>
#include <libmeegochat/meegochataccountmodel.h>
#include <libmeegochat/meegochatcontactmodel.h>
#include <libmeegochat/meegochatcontactsortfilterproxymodel.h>
#include <libmeegochat/meegochatcontactmanager.h>

#include "contactlistpage.h"
#include "contactmessagelistitemcreator.h"
#include "acctlistitemcreator.h"

M_REGISTER_WIDGET(ContactListPage);

ContactListPage::ContactListPage(DialogPage *dlgPage) :
    MApplicationPage(),
    mAccountManager(MeeGoChat::AccountManager::getInstance()),
    mAcctListItemModel(0),
    mPgAddContact(0),
    mDialogPage(dlgPage),
    mApproveContactCnt(new MWidgetController(this)),
    mAcctHeader(0),
    mAcctFooter(0),
    mContactArea(0),
    mContactMessageArea(0),
    mAcctArea(0),
    mTimerID(0),
    mContactListVisible(false),
    mAccountsReady(false)
{
    mContactListModelSorted = new MeeGoChat::ContactSortFilterProxyModel(0);
    mContactListModelSorted->setDynamicSortFilter(true);
    mContactListModelSorted->setSourceModel(0);
    //mContactListModelSorted->sort(0);

    connect(this,
            SIGNAL(appeared()),
            this,
            SLOT(resizeViewports()));

}

ContactListPage::~ContactListPage()
{
}

void ContactListPage::createContent()
{
    //% "Contacts"
    setTitle(qtTrId("page_title_contacts"));
    MApplicationPage::createContent();
    setPannable(false);

    mLayout = new MLayout();
    mMsgPolicy =
            new MLinearLayoutPolicy(mLayout, Qt::Vertical);
    mCntPolicy =
            new MLinearLayoutPolicy(mLayout, Qt::Vertical);
    mAcctPolicy =
            new MLinearLayoutPolicy(mLayout, Qt::Vertical);


    mMsgPolicy->setSpacing(0);
    mCntPolicy->setSpacing(0);
    mAcctPolicy->setSpacing(0);

//    mCntAction = new MAction("icon-m-toolbar-addressbook", "", this);
    //% "Contact List"
    mCntAction = new MAction(QString("<b>").append(qtTrId("action_contact_list")).append("</b>"), this);
    mCntAction->setLocation(MAction::ApplicationMenuLocation);
    mCntAction->setObjectName("mCntAction");
    addAction(mCntAction);
    connect(mCntAction,
            SIGNAL(triggered()),
            this,
            SLOT(onCntActionTriggered()));

//    mInbAction = new MAction("icon-m-content-chat", "", this);
    //% "Messages"
    mInbAction = new MAction(qtTrId("action_conversation_list"), this);
    mInbAction->setLocation(MAction::ApplicationMenuLocation);
    mInbAction->setObjectName("inbAction");
    addAction(mInbAction);
    connect(mInbAction,
            SIGNAL(triggered()),
            this,
            SLOT(onInbActionTriggered()));

    //% "Settings"
    MAction *actSettings = new MAction(qtTrId("action_settings"), this);
    actSettings->setLocation(MAction::ApplicationMenuLocation);
    actSettings->setObjectName("settingsAction");
    addAction(actSettings);
    connect(actSettings,
            SIGNAL(triggered()),
            this,
            SLOT(onSettingsActionTriggered()));


    mCboAcctStatus = new MComboBox();
    mCboAcctStatus->setObjectName("mCboAcctStatus");
    //% "Available"
    mCboAcctStatus->addItem("icon-m-common-presence-online", qtTrId("status_available"));
    //% "Away"
    mCboAcctStatus->addItem("icon-m-common-presence-away", qtTrId("status_away"));
    //% "Busy"
    mCboAcctStatus->addItem("icon-m-common-presence-busy",qtTrId("status_busy"));
    //% "Offline"
    mCboAcctStatus->addItem("icon-m-common-presence-offline",qtTrId("status_offline"));
    //% "Status:"
    mCboAcctStatus->setTitle(qtTrId("cbo_header_status"));
    connect(mCboAcctStatus,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onCboAcctStatusIdxChanged(int)));

    mMsgPolicy->addItem(mCboAcctStatus);
    mCntPolicy->addItem(mCboAcctStatus);
    mAcctPolicy->addItem(mCboAcctStatus);

    //Approve Contact Widget container area...
    mApproveContactLayout = new MLayout();
    mApproveContactPolicy = new MLinearLayoutPolicy(mApproveContactLayout, Qt::Vertical);
    mApproveContactLayout->setPolicy(mApproveContactPolicy);
    mApproveContactCnt->setLayout(mApproveContactLayout);

    mCntPolicy->addItem(mApproveContactCnt);
    mAcctPolicy->addItem(mApproveContactCnt);

    //Used to fake the effect that the contact list view is just
    //part of an expanded tree-view account list..
    mAcctHeader = new AcctListItem();
    mAcctHeader->setExpanded(true);
    mAcctHeader->setObjectName("acctListItem");
    mAcctHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mCntPolicy->addItem(mAcctHeader);

    connect(mAcctHeader,
            SIGNAL(clicked()),
            this,
            SLOT(onAcctHeaderClicked()));

    connect(mAcctHeader,
            SIGNAL(AddContactClicked(Acct*)),
            this,
            SLOT(onAddContactClicked(Acct*)));

    mAcctFooter = new AcctListItem();
    mAcctFooter->setExpanded(false);
    mAcctFooter->setObjectName("acctListItem");
    mAcctFooter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(mAcctFooter,
            SIGNAL(clicked()),
            this,
            SLOT(onAcctFooterClicked()));

    //Begin mContactList pannable area...
    mContactArea = new MPannableViewport();
    mContactArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mContactArea->setObjectName("mContactListViewport");

    mContactList = new MList;
    mContactList->setViewType("fast");
    mContactList->setObjectName("contactList");
    mContactList->setCellCreator(new ContactListItemCreator());
    mContactList->setItemModel(mContactListModelSorted);
    mContactList->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Fixed);

    connect(mContactListModelSorted,
            SIGNAL(layoutChanged()),
            this,
            SLOT(resizeViewports()));

    connect(mContactList,
            SIGNAL(itemClicked(QModelIndex)),
            this,
            SLOT(contactListItemClicked(const QModelIndex &)));

    mContactArea->setWidget(mContactList);
    //End mContactList pannable area...


    //Begin ContactMessageList pannable area...
    mContactMessageArea = new MPannableViewport();
    mContactMessageArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mContactMessageArea->setObjectName("ContactMessageListViewport");

    mContactMessageList = new MList();
    mContactMessageList->setViewType("fast");
    mContactMessageList->setObjectName("contactMessageList");
    mContactMessageList->setCellCreator(new ContactMessageListItemCreator());
    mContactMessageList->setItemModel(mContactListModelSorted);

    connect(mContactMessageList,
            SIGNAL(itemClicked(QModelIndex)),
            this,
            SLOT(contactListItemClicked(const QModelIndex &)));

    mContactMessageArea->setWidget(mContactMessageList);
    //End mContactMessageList pannable area...

    //Begin acctList pannable area...
    mAcctArea = new MPannableViewport();
    mAcctArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mAcctArea->setObjectName("acctListViewport");

    mAcctListItemModel = mAccountManager->getAccountModel();

    mAcctList = new MList;
    mAcctList->setViewType("fast");
    mAcctList->setObjectName("acctList");
    mAcctList->setCellCreator(new AcctListItemCreator());
    //mAcctList->setItemModel(mAcctListItemModel);
    mAcctList->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);
    mAcctList->setItemModel(mAcctListItemModel);


    connect(mAcctList,
            SIGNAL(itemClicked(QModelIndex)),
            this,
            SLOT(acctListItemClicked(const QModelIndex &)));

    mAcctArea->setWidget(mAcctList);
    //End acctList pannable area...


    mCntPolicy->addItem(mContactArea);
    mMsgPolicy->addItem(mContactMessageArea);
    mAcctPolicy->addItem(mAcctArea);

    mLayout->setPolicy(mAcctPolicy);
    centralWidget()->setLayout(mLayout);
    this->resizeViewports();

    //Moved the following code to this section to avoid the occasional race
    //condition where we'd hit the AccountsReady/FirstAccountStatus
    //handling code (which sets the index of cboAcctStatus based off current
    //status) befure cboAcctStatus was valid.

    //In case the AccountsReady/NoAccounts signal happens before we connect...
    if (mAccountManager->areAccountsReady()) {
        onAccountManagerAccountsReady();
        if (mAccountManager->getAccountList().count() == 0)
            //Can't open another page from the createContent() context, otherwise
            //Bad Things (c) happen...
            //This is all really just a hack to last until I have time
            //to properly rewrite the whole app into a proper MVC-structued app...
            QTimer::singleShot(50, this, SLOT(onNoAccounts()));
    } else {
        connect(mAccountManager,
                SIGNAL(AccountsReady()),
                this,
                SLOT(onAccountManagerAccountsReady()));
        connect(mAccountManager,
                SIGNAL(NoAccounts()),
                this,
                SLOT(onNoAccounts()));
    }

    connect(mAccountManager,
            SIGNAL(AccountCreated(Acct *)),
            this,
            SLOT(onAccountManagerAccountCreated(Acct*)));
    //End moved code snippet...

}

//Private slots

void ContactListPage::onAccountManagerAccountsReady()
{
    mAccountsReady = true;
    bool gotStatus = false;
    //qDebug() << QString("Row count: ") << mAcctListItemModel->rowCount(QModelIndex());

    //Ugly hack to set global status! Based off of "first" account status...
    foreach(MeeGoChat::Acct *acct, mAccountManager->getAccountList()) {
        if (!gotStatus && acct->getEnabled()) {
            this->onFirstStatus(acct->getStatus());
            gotStatus = true;
        }
        ContactManager *cm = acct->getContactManager();
        if (cm->isReady()) {
            onContactManagerReady(cm);
        } else {
            connect(acct->getContactManager(),
                    SIGNAL(Ready(ContactManager*)),
                    this,
                    SLOT(onContactManagerReady(ContactManager *)));
        }
    }

    //If none of our accounts were enabled, then show as offline...
    if (!gotStatus)
        this->onFirstStatus(MeeGoChat::Acct::OFFLINE);
}

void ContactListPage::onAccountManagerAccountCreated(Acct *acct)
{
    if (!acct->isReady()) {
        connect(acct,
                SIGNAL(Ready(Acct*)),
                this,
                SLOT(onAccountCreatedReady(Acct*)));
    } else {
        onAccountCreatedReady(acct);
    }
}

void ContactListPage::onAccountCreatedReady(Acct *acct)
{
    connect(acct->getContactManager(),
            SIGNAL(Ready(ContactManager*)),
            this,
            SLOT(onContactManagerReady(ContactManager*)));
}

void ContactListPage::onContactManagerReady(ContactManager *cm)
{
    if (!mAcctContactModels.contains(cm->getAccount())) {
        mAcctContactModels.insert(cm->getAccount(), cm->getContactModel(this));
        QList<ChatContact *> cList = cm->getPresPubReqContactList();
        connect(cm,
                SIGNAL(PresencePublicationRequested(ChatContact*)),
                this,
                SLOT(onPresencePublicationRequested(ChatContact*)));
        connect(cm,
                SIGNAL(ContactReceivedMessage(ChatContact*, Message*)),
                mDialogPage,
                SLOT(onMsgReceived(ChatContact*,Message*)));
        foreach(ChatContact *contact, cList) {
            onPresencePublicationRequested(contact);
        }
    }
}

void ContactListPage::contactListItemClicked(const QModelIndex &index)
{
    MeeGoChat::ChatContact *contact =
            index.data(Qt::UserRole).value<MeeGoChat::ChatContact *>();
    mDialogPage->setContact(contact);
    mDialogPage->appear();
    //contact->setUnread(false);
    //contact->showDialogPage();
}

void ContactListPage::acctListItemClicked(const QModelIndex &index)
{
    setupAcctHeaderFooter(index.data(Qt::UserRole).value<Acct *>());
}

void ContactListPage::onAcctHeaderClicked()
{
    mContactListVisible = false;
    mLayout->setPolicy(mAcctPolicy);
}

void ContactListPage::onAcctFooterClicked()
{
    setupAcctHeaderFooter(mAcctFooter->getAccount());
}

void ContactListPage::setupAcctHeaderFooter(Acct *acct)
{
    if (acct) {
/*        if (!acct->isReady())
            qDebug() << QString("Account %1 not ready!").arg(acct->getDisplayName());
        else
            qDebug() << QString("Account %1 ready!").arg(acct->getDisplayName());*/
        if (mAcctContactModels.contains(acct)) {
            mContactListModelSorted->setSourceModel(mAcctContactModels.value(acct));
            mContactList->setItemModel(mContactListModelSorted);
            mAcctHeader->setAccount(acct);
            mContactListVisible = true;
            QModelIndex index = mAcctListItemModel->getIndexForAccount(acct);

            //Set up our AcctListItem footer, if there is a "next" account...
            QModelIndex indexNext = mAcctListItemModel->index(index.row()+1, 0, index.parent());
            if (indexNext.isValid())
            {
                mAcctFooter->setAccount(indexNext.data(Qt::UserRole).value<Acct *>());
                if (mLayout->indexOf(mAcctFooter) == -1)
                {
                    mCntPolicy->addItem(mAcctFooter);
                    mAcctFooter->show();
                }
            } else if (mLayout->indexOf(mAcctFooter) != -1)
            {
                mLayout->removeItem(mAcctFooter);
                mAcctFooter->hide();
            }

            mLayout->setPolicy(mCntPolicy);
            resizeViewports();
        } else {
            if (!mAccountsReady && mAccountManager->areAccountsReady()) {
                this->onAccountManagerAccountsReady();
            }
            if (mAcctContactModels.contains(acct)) {
                setupAcctHeaderFooter(acct);
            } else {
                ContactManager *cm = acct->getContactManager();
/*                if (!cm)
                    qDebug() << QString("CM is null for account %1!").arg(acct->getDisplayName());
                else
                    qDebug() << QString("CM is not null for account %1!").arg(acct->getDisplayName());*/
                //TODO - display something here like "You must go online to see your contacts!"
                if (cm->isReady()) {
                    onContactManagerReady(cm);
                    setupAcctHeaderFooter(acct);
                } else {
                    connect(acct->getContactManager(),
                            SIGNAL(Ready(ContactManager*)),
                            this,
                            SLOT(onContactManagerReady(ContactManager*)));
                }
            }
        }
    }

}

void ContactListPage::onAddContactClicked(Acct *acct)
{
    mPgAddContact = new AddContactPage(acct->getContactManager());
    //TODO - figure out...
    connect(mPgAddContact,
            SIGNAL(NewContactAdded(Tp::AccountPtr,Tp::ContactPtr)),
            this,
            SLOT(onAddContactNewContact(Tp::AccountPtr,Tp::ContactPtr)));
    mPgAddContact->appear(MSceneWindow::DestroyWhenDone);

}

void ContactListPage::onInbActionTriggered()
{
    //If mTimerID != 0, then we're already in the Inbox view...
    if (!mTimerID)
    {
        //% "Messages"
        setTitle(qtTrId("page_title_messages"));
        //mCntAction->setIconID("people-inactive");
        //mInbAction->setIconID("messages-icon_pressed");
        //% "Contact List"
        mCntAction->setText(qtTrId("action_contact_list"));
        //% "Conversation List"
        mInbAction->setText(QString("<b>").append(qtTrId("action_conversation_list")).append("</b>"));
        mContactListModelSorted->setSort(ContactSortFilterProxyModel::SORT_LASTMSGRCVD);
        mContactListModelSorted->setFilter(ContactSortFilterProxyModel::FILTER_INBOX);
        mLayout->setPolicy(mMsgPolicy);
        mTimerID = this->startTimer(30 * 1000);
    }
}

void ContactListPage::onCntActionTriggered()
{
    //If mTimerID != 0, then we're in the Inbox view, switch to Account/Contact view
    if (mTimerID)
    {
        setTitle(qtTrId("page_title_contacts"));
        //% "Contact List"
        mCntAction->setText(QString("<b>").append(qtTrId("action_contact_list")).append("</b>"));
        //% "Conversation List"
        mInbAction->setText(qtTrId("action_conversation_list"));
        //mCntAction->setIconID("people-pressed");
        //mInbAction->setIconID("messages-icon_inactive");
        this->killTimer(mTimerID);
        mTimerID = 0;
        if (mContactListVisible)
        {
            mContactListModelSorted->setSort(ContactSortFilterProxyModel::SORT_STATUS);
            mContactListModelSorted->setFilter(ContactSortFilterProxyModel::FILTER_NONE);
            mLayout->setPolicy(mCntPolicy);
        } else
        {
            mLayout->setPolicy(mAcctPolicy);
        }
    }
}

void ContactListPage::onCboAcctStatusIdxChanged(int idx)
{
    mAccountManager->setGlobalStatus((Acct::AccountStatus)idx);
};

void ContactListPage::onNoAccounts()
{
    AcctWizardPage *acctWizPage = new AcctWizardPage();
    acctWizPage->appear(MSceneWindow::DestroyWhenDone);
}

void ContactListPage::updateTimeTexts()
{
    //Only bother updating when we're visible
    if (mLayout->policy() == mMsgPolicy)
    {
        QModelIndex first = mContactMessageList->firstVisibleItem();
        QModelIndex last = mContactMessageList->lastVisibleItem();
        int i, l = last.row();
        if (l != -1)
        {
            for (i = first.row(); i <= l; ++i)
            {
                QModelIndex cur = mContactListModelSorted->index(
                        i, 0, QModelIndex());
                QVariant data = cur.data(Qt::UserRole); //ChatContact *
                ChatContact *contact = data.value<ChatContact *>();
                contact->triggerChanged();
                //contact->updateFuzzyDateTime();
                //TODO - make this work w/ libmeegochat
            }
        }
    }
}

void ContactListPage::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    this->updateTimeTexts();
}

void ContactListPage::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MApplicationPage::resizeEvent(event);
    resizeViewports();
}

void ContactListPage::resizeViewports()
{
    if (mContactArea && mContactMessageArea && mAcctArea)
    {
        //If both the following are 0, then the items haven't
        //actually displayed yet - try again later...
        if ((this->centralWidget()->geometry().top() == 0)
            && (mCboAcctStatus->size().height() == 0))
        {
            QTimer::singleShot(50, this, SLOT(resizeViewports()));
            return;
        }

        qreal nh = this->size().height();
//        qDebug() << QString("this->size: %1").arg(nh);
        nh -= this->centralWidget()->geometry().top();
//        qDebug() << QString("this->size - cW->size: %1").arg(nh);

        //Subtract the Account Status combobox height for all views
        nh -= mCboAcctStatus->size().height();
//        qDebug() << QString("after mCboAcctStatus height: %1").arg(nh);

        if (nh < 0)
            nh = 0;

//        qDebug() << QString("Setting mContactMessageArea height to: %1").arg(nh);
        mContactMessageArea->setMinimumHeight(nh);
        mContactMessageArea->setMaximumHeight(nh);
        mContactMessageArea->setPreferredHeight(nh);

        //For AcctList view and Contact List view,
        //also subtract incoming chat/contact request area height,
        //if it is currently being displayed...
        nh -= mApproveContactCnt->size().height();

//        qDebug() << QString("Subtracting mApproveContactCnt size of %1!").arg(mApproveContactCnt->size().height());

        if (nh < 0)
            nh = 0;

//        qDebug() << QString("Setting mAcctArea height to: %1").arg(nh);
        mAcctArea->setMinimumHeight(nh);
        mAcctArea->setMaximumHeight(nh);
        mAcctArea->setPreferredHeight(nh);

        //Finally, also subtract the height of the
        //Account header in Contact List view, and the Account footer,
        //if it's currently visible...
        nh -= mAcctHeader->size().height();
        if (mCntPolicy->indexOf(mAcctFooter) != -1)
            nh -= mAcctFooter->size().height();
        if (nh < 0)
            nh = 0;

//        qDebug() << QString("Setting mContactArea height to: %1").arg(nh);
        mContactArea->setMinimumHeight(nh);
        mContactArea->setMaximumHeight(nh);
        mContactArea->setPreferredHeight(nh);
    }
}

void ContactListPage::onPresencePublicationRequested(ChatContact *contact)
{
    ApproveContactWidget *acw = new ApproveContactWidget(contact, this);
    connect(acw,
            SIGNAL(RequestHandled(ApproveContactWidget*)),
            this,
            SLOT(onRequestHandled(ApproveContactWidget*)));

    mApproveContactPolicy->addItem(acw);
    //The new height of mApproveContactCnt hasn't actually taken yet, so do our resize in a bit...
    QTimer::singleShot(50, this, SLOT(resizeViewports()));
}

void ContactListPage::onRequestHandled(ApproveContactWidget *acw)
{
    mApproveContactLayout->animatedDeleteItem(acw);
    //The new height of mApproveContactCnt hasn't actually taken yet, so do our resize in a bit...
    QTimer::singleShot(50, this, SLOT(resizeViewports()));
}

void ContactListPage::onFirstStatus(MeeGoChat::Acct::AccountStatus status)
{
    mCboAcctStatus->setCurrentIndex(status);
}

void ContactListPage::onSettingsActionTriggered()
{
    DuiControlPanelIf *dcpIf = new DuiControlPanelIf();
    dcpIf->appletPage("MeeGoChatApplet");
}

