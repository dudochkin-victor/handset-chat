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


#include "dialogpage.h"
#include "chatdbusadaptor.h"

#include <MContainer>
#include <MPannableViewport>
#include <MRemoteAction>

DialogPage::DialogPage() :
        MApplicationPage(),
        mCurMessageList(new QList<Message *>()),
        mCurContact(0),
        mIncomingText(false),
        mTimerID(0),
        mOffline(true)
{
    //this->setTitle("IM");
    this->setComponentsDisplayMode(MApplicationPage::HomeButton
                                   | MApplicationPage::NavigationBar,
                                   MApplicationPageModel::Hide);
    setEscapeMode(MApplicationPageModel::EscapeManualBack);
    connect(this,
            SIGNAL(backButtonClicked()),
            SLOT(onBackButtonClicked()));

    this->setPannable(false);
    MLayout *vLayout = new MLayout();
    MLinearLayoutPolicy *vPolicy = new MLinearLayoutPolicy(vLayout, Qt::Vertical);


    dlgPageHeader = new DialogPageHeader(mCurContact);
    dlgPageHeader->setObjectName("dialogPageHeader");
    dlgPageHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    vPolicy->addItem(dlgPageHeader);

    msgArea = new MPannableViewport();
    msgArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    msgArea->setObjectName("dialogViewport");
    msgArea->setPanDirection(Qt::Vertical);

    msgAreaLayout = new MLayout();
    msgAreaPolicy = new MLinearLayoutPolicy(msgAreaLayout, Qt::Vertical);
    msgAreaLayout->setPolicy(msgAreaPolicy);

    mCnt = new MWidgetController();
    mCnt->setLayout(msgAreaLayout);
    msgArea->setWidget(mCnt);
    //msgArea->setLayout(msgAreaLayout);

    /*messageList = new MList;
    messageList->setObjectName("dialogPageMessageList");
    messageList->setViewType("fast");
    messageList->setCellCreator(new MessageListItemCreator());
    messageList->setItemModel(messageListItemModel);
    messageList->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);
    msgArea->setWidget(messageList);*/
    vPolicy->addItem(msgArea);

    /* begin message entry area */

        entryBox = new MStylableWidget();
        entryBox->setObjectName("entryBox");

        entryLayout = new MLayout();
        entryPolicy = new MLinearLayoutPolicy(entryLayout, Qt::Horizontal);

        mEntry = new MTextEdit(MTextEditModel::SingleLine,
                QString());
        //% "Tap here to start typing"
        mEntry->setPrompt(qtTrId("edit_prompt_tap_start_typing"));
        mEntry->setObjectName("textEntry");
        //mEntry->setEnabled(false);  //Don't enable text entry until the Tp channel is ready to go...

/*        clear = new MButton();
        clear->setObjectName("clearButton");
        clear->setEnabled(false);*/

        send = new MButton();
        send->setIconID("icon-m-toolbar-send");
        send->setObjectName("sendButton");
        send->setEnabled(false);

//        separator = new MStylableWidget();
//        separator->setObjectName("textSeparator");

        entryPolicy->addItem(mEntry);
//        entryPolicy->addItem(clear);
//        entryPolicy->addItem(separator, Qt::AlignVCenter);
        entryPolicy->addItem(send);

        entryLayout->setPolicy(entryPolicy);
        entryBox->setLayout(entryLayout);

        connect(mEntry, SIGNAL(gainedFocus(Qt::FocusReason)),
                this, SLOT(onMsgEntryFocused()));
        connect(mEntry, SIGNAL(textChanged()),
                this, SLOT(onMsgEntryTextChanged()));
        connect(mEntry, SIGNAL(lostFocus(Qt::FocusReason)),
                this, SLOT(onMsgEntryLostFocus()));
        connect(mEntry, SIGNAL(returnPressed()),
                this, SLOT(onSendClicked()));
//        connect(clear, SIGNAL(clicked()),
//                this, SLOT(onClearClicked()));
        connect(send, SIGNAL(clicked()),
                this, SLOT(onSendClicked()));

    connect(msgArea, SIGNAL(displayEntered()),
            this, SLOT(onViewportGainedFocus()));

    vPolicy->addItem(entryBox);
    vLayout->setPolicy(vPolicy);
    centralWidget()->setLayout(vLayout);

    connect(this,
            SIGNAL(appeared()),
            this,
            SLOT(onAppeared()));
    connect(this,
            SIGNAL(disappeared()),
            this,
            SLOT(onDisappeared()));

}

DialogPage::~DialogPage()
{
}

void DialogPage::createContent()
{
}

void DialogPage::setContact(ChatContact *contact)
{
    //Only go through this if we're actually switching contacts...
    if ((!mCurContact) || (mCurContact != contact)) {
        if (mCurContact) {
            disconnect(mCurContact,
                       SIGNAL(ContactChanged(ChatContact*)),
                       this,
                       SLOT(onContactChanged()));
        }
        mCurContact = contact;
        //Since this is connected to us through the ContactManager::ContactReceivedMessage
        //in ContactListPage::onContactManagerReady, we don't need to connect here...
//        connect(mCurContact,
//                SIGNAL(ReceivedMessage(ChatContact*,Message*)),
//                this,
//                SLOT(onMsgReceived(ChatContact*, Message*)));
        dlgPageHeader->setContact(contact);
        mCurMessageList->clear();
        mCurMessageList->append(mCurContact->getMessageList());
//        mCurMessageList = new QList<MeeGoChat::Message *>(mCurContact->getMessageList());
        while (!msgAreaLayout->isEmpty())
            msgAreaLayout->animatedDeleteAt(0);

        foreach (Message *msg, *mCurMessageList) {
            addMLI(msg);
        }
    }
}

void DialogPage::onBackButtonClicked()
{
    dismiss();
}

void DialogPage::onMsgEntryGainedFocus()
{
//    if (entryLayout->indexOf(clear) == -1)
//        entryPolicy->addItem(clear);
//    if (entryLayout->indexOf(separator) == -1)
//        entryPolicy->addItem(separator);
//    if (entryLayout->indexOf(send) == -1)
//        entryPolicy->addItem(send);
}

void DialogPage::onMsgEntryLostFocus()
{

}

void DialogPage::onMsgEntryTextChanged()
{
    if (mEntry->text().isEmpty())
    {
        send->setEnabled(false);
        //clear->setEnabled(false);
    } else
    {
        send->setEnabled(true);
       // clear->setEnabled(true);
    }
}

void DialogPage::onClearClicked()
{
    mEntry->clear();
    send->setEnabled(false);
   // clear->setEnabled(false);
    mEntry->setFocus(Qt::OtherFocusReason);
}

void DialogPage::onSendClicked()
{
    Message *msg = mCurContact->sendMessage(mEntry->text());
    mEntry->clear();
    send->setEnabled(false);
    //clear->setEnabled(false);
    mEntry->setFocus(Qt::OtherFocusReason);
    //messageListItemModel->addMessage(msg);
    msgArea->setPosition(QPointF(0,
          mCnt->preferredHeight() - msgArea->size().height()));

    mCurMessageList->append(msg);
    addMLI(msg);
}

void DialogPage::onAcctStatusChanged(Acct::AccountStatus status)
{
    if (status == Acct::OFFLINE)
    {
        mOffline = true;
        dlgPageHeader->setOffline(mOffline);
    }
}


void DialogPage::onMsgReceived(ChatContact *contact, Message *msg)
{

    if (mCurContact == contact) {
        mCurMessageList->append(msg);
        addMLI(msg);
        //TODO: make this better...
        msgArea->setPosition(QPointF(0,
              mCnt->preferredHeight() - msgArea->size().height()));
    }

    emit this->onNewMessage(!this->isVisible());

    //TODO - test on Netbook-M or Aava device to see how notifications work...
    if (!this->isVisible() || !mCurContact || (mCurContact != contact))
    {
        MNotification *note = new MNotification(MNotification::ImReceivedEvent,
                       QString(/*% "Message from " */qtTrId("notification_message_from")).append(contact->getName()),
                       msg->getText()
                       );
        QList<QVariant> actionArgs;
        actionArgs.append(QVariant::fromValue<QString>(contact->getAccount()->getTpAcctPath()));
        actionArgs.append(QVariant::fromValue<QString>(contact->getTpID()));
        MRemoteAction action(DBUSINTFNAME,
                             DBUSOBJPATH,
                             DBUSINTFNAME,
                             "showDialogPageForContact",
                             actionArgs);
        note->setAction(action);
        note->publish();
    }
}

void DialogPage::onAppeared()
{
    mTimerID = this->startTimer(30 * 1000);
    resizeViewport();
    mEntry->setFocus();
}

void DialogPage::onDisappeared()
{
    this->killTimer(mTimerID);
    mTimerID = 0;
}

void DialogPage::updateTimeTexts()
{
    //TODO-JEA999 - this is going to be ugly w/o an MList
    /*
    QModelIndex first = messageList->firstVisibleItem();
    QModelIndex last = messageList->lastVisibleItem();
    int i, l = last.row();
    if (l == -1)
        return;
    for (i = first.row(); i <= l; ++i)
    {
        QModelIndex cur = messageListItemModel->index(
                i, 0, QModelIndex());
        QVariant data = cur.data(Qt::UserRole); //Message*
        Message *msg = data.value<Message *>();
        msg->updateFuzzyDateTime();
    }*/

    int i;
    for (i = 0; i < msgAreaLayout->count(); ++i)
    {
        QGraphicsLayoutItem *gfxItem = msgAreaLayout->itemAt(i);
        MessageListItem *mli = dynamic_cast<MessageListItem *>(gfxItem);
        if (mli->isVisible())
        {
            //TODO: Figure out how to do this with libmeegochat
            //mli->getMsg()->updateFuzzyDateTime();
        }
    }
}

void DialogPage::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    this->updateTimeTexts();
}

void DialogPage::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MApplicationPage::resizeEvent(event);
    resizeViewport();
}

void DialogPage::resizeViewport()
{
    if (dlgPageHeader && entryBox)
    {
        qreal newHeight = this->size().height();
        newHeight -= centralWidget()->geometry().top();
        newHeight -= dlgPageHeader->size().height();
        newHeight -= entryBox->size().height();
        msgArea->setMinimumHeight(newHeight);
        msgArea->setMaximumHeight(newHeight);
        msgArea->setPreferredHeight(newHeight);
    }

}

void DialogPage::addMLI(Message *msg)
{
    MessageListItem *mli = new MessageListItem(msgArea);
    mli->setMsg(msg);

    connect(mli,
            SIGNAL(deleteMe(MessageListItem*)),
            this,
            SLOT(onMLIDeleteMe(MessageListItem*)));
    connect(mli,
            SIGNAL(deleteAll()),
            this,
            SLOT(onMLIDeleteAll()));

    msgAreaPolicy->addItem(mli);
}

void DialogPage::onMLIDeleteMe(MessageListItem *mli)
{
    mCurMessageList->removeAll(mli->getMsg());
    msgAreaLayout->animatedDeleteItem(mli);
}

void DialogPage::onMLIDeleteAll()
{
    mCurMessageList->clear();
    while (!msgAreaLayout->isEmpty())
        msgAreaLayout->animatedDeleteAt(0);
}
