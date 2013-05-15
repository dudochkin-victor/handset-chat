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




#ifndef DIALOGPAGE_H
#define DIALOGPAGE_H

#include <MApplicationPage>
#include <MButton>
#include <MTextEdit>
#include <MNotification>

#include <libmeegochat/meegochatcontact.h>
#include <libmeegochat/meegochatmessage.h>
#include <libmeegochat/meegochataccount.h>

#include "messagelistitem.h"
#include "messagelistitemcreator.h"
#include "dialogpageheader.h"

using namespace MeeGoChat;

class DialogPage : public MApplicationPage
{
    Q_OBJECT
public:
    /*DialogPage(ChatContact *contact, MessageListItemModel *msgListItemModel);*/
    DialogPage();
    ~DialogPage();
    void createContent();

public slots:
    void setContact(ChatContact *contact);
    void onMsgReceived(ChatContact *, Message *);

signals:
    void onNewMessage(bool);

private slots:
    void onBackButtonClicked();
    void onMsgEntryGainedFocus();
    void onMsgEntryLostFocus();
    void onMsgEntryTextChanged();
    void onClearClicked();
    void onSendClicked();
    void onAcctStatusChanged(Acct::AccountStatus status);

    void onAppeared();
    void onDisappeared();

    void onMLIDeleteMe(MessageListItem*);
    void onMLIDeleteAll();

protected:
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void updateTimeTexts();
    void sendMessage(Message *msg);
    void resizeViewport();
    void addMLI(Message *msg);


    MStylableWidget *entryBox;

//    QHash<ChatContact *, QList<Message *> *> mContactML;
    QList<Message *> *mCurMessageList;
    ChatContact *mCurContact;

#if 0
//    Switch back to MList/Model when MList can support
//    variably-heighted cells...
    MList *messageList;
    MessageListItemModel *messageListItemModel;
#endif

    DialogPageHeader *dlgPageHeader;
    MPannableViewport *msgArea;
    MLinearLayoutPolicy *msgAreaPolicy;
    MLayout *msgAreaLayout;
    MWidgetController *mCnt;
    MLayout *entryLayout;
    MLinearLayoutPolicy *entryPolicy;
    //MButton *clear;
    //MStylableWidget *separator;
    MButton *send;
    MTextEdit *mEntry;
   // MNotification *dnf;

    bool mIncomingText;
    int mTimerID;
    bool mOffline;
};

#endif // DIALOGPAGE_H
