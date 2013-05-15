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


#include "messagelistitem.h"

#include <MWidgetCreator>
#include <MAction>

#include <libmeegochat/meegochatcontact.h>


M_REGISTER_WIDGET(MessageListItem);

MessageListItem::MessageListItem(MWidget *parent) :
    MListItem(parent),
    mMsg(0)
{
    setObjectName("messageListItem");
    mLayout = new MLayout();
    mPolicy = new MLinearLayoutPolicy(mLayout, Qt::Vertical);

    mMsgText = new MLabel("");
    mMsgText->setTextElide(false);
    mMsgText->setObjectName("messageListItemMsg");
    mMsgText->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mMsgText->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    mMsgText->setWordWrap(true);
    mMsgText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mPolicy->addItem(mMsgText, Qt::AlignLeft);

    mMsgRcvd = new MLabel("");
    mMsgRcvd->setObjectName("messageListItemRcvd");
    mMsgRcvd->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Fixed);
    mPolicy->addItem(mMsgRcvd, Qt::AlignLeft);

    //% "Delete this message"
    MAction *actDelMsg = new MAction(qtTrId("action_delete_message"), this);
    connect(actDelMsg, SIGNAL(triggered()), SLOT(deleteMessage()));
    addAction(actDelMsg);

    //% "Delete entire thread"
    MAction *actDelThread = new MAction(qtTrId("action_delete_thread"), this);
    connect(actDelThread, SIGNAL(triggered()), SLOT(deleteThread()));
    addAction(actDelThread);


    mLayout->setPolicy(mPolicy);
    setLayout(mLayout);
}

void MessageListItem::setMsg(Message *msg)
{
    //Set status message
    mMsg = msg;
    //Word wrapping in MLabel is currently broken
    //unless you prefix a random HTML tag...
    QString randomTag = QString("<a></a>");
    //% "Me: "
    QString name = qtTrId("name_me");
    QString dtDir;

    if (mMsg->getStatus() == Message::RECEIVED) {
        name = mMsg->getContact()->getName().append(": ");
        //style().setModeIncoming();
        //% "Received "
        dtDir = qtTrId("msg_direction_received");
    } else {
/*        if (mMsg->getStatus() == Message::PENDINGSEND)
            style().setModePendingSend();
        else*/
            //style().setModeOutgoing();
            //% "Sent "
            dtDir = qtTrId("msg_direction_sent");
    }

    mMsgText->setText(randomTag.append(name.append(mMsg->getText())));


/* If we're the first message, then show the time. If last message
    direction is opposite of ours, or if it's been > 1 hour since the last
    message, then show the time. Otherwise we don't...
*/
//    if ((prevMsg == 0) ||
//        (
//                (prevMsg->getDirection() != mMsg->getDirection()) ||
//            (prevMsg->getDateTime().addSecs(60 * 60) < mMsg->getDateTime())
//        ))
//    {
//        mShowTime = true;
//        mMsgRcvd->setText(toFuzzyTime(mMsg->getDateTime()));
//    } else
//    {
//        mShowTime = false;
//    }

    mShowTime = true;

    mMsgRcvd->setText(dtDir + toFuzzyTime2(mMsg->getDateTime()));


    //TODO-UI-LT: currently MFastList doesn't support cells of varying heights...
//    this->setPreferredHeight(mMsg->preferredHeight() + mMsgRcvd->preferredHeight() + (mUseSpacer ? spacer->preferredHeight() : 0.0));
//    this->setMinimumHeight(mMsg->preferredHeight() + mMsgRcvd->preferredHeight() + (mUseSpacer ? spacer->preferredHeight() : 0.0));
//    this->adjustSize();
}

Message * MessageListItem::getMsg()
{
    return mMsg;
}

void MessageListItem::deleteMessage()
{
    if (!mMsg)
        return;
//    mMsg->getContact()->deleteMsg(mMsg);
    emit this->deleteMe(this);
}

void MessageListItem::deleteThread()
{
    if (!mMsg)
        return;
    //mMsg->getContact()->deleteThread();
    emit this->deleteAll();
}

void MessageListItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mPressed = true;
    style().setModePressed();
    update();
    ev->accept();
}

void MessageListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (mPressed) {
        mPressed = false;
      //  if (mMsg->getStatus() == Message::RECEIVED)
            //style().setModeIncoming();
     //   else
            //style().setModeOutgoing();
        update();
        ev->accept();
        emit clicked();
    } else {
        ev->ignore();
    }
}

void MessageListItem::cancelEvent(MCancelEvent *ev)
{
    mPressed = false;
   // if (mMsg->getStatus() == Message::RECEIVED)
   //     style().setModeIncoming();
  //  else
  //      style().setModeOutgoing();
    update();
    ev->accept();
}
