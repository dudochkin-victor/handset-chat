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


#include "contactmessagelistitem.h"

#include <MWidgetCreator>
#include <MGridLayoutPolicy>
#include <libmeegochat/meegochatcontact.h>
#include <libmeegochat/meegochataccount.h>



M_REGISTER_WIDGET(ContactMessageListItem);

ContactMessageListItem::ContactMessageListItem(MWidget *parent) :
    MListItem(parent),
    mPressed(false),
    mUnread(false),
    mContactLastMsg(QString("")),
    mLastMsgDir(Message::NONE)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setObjectName("contactMessageListItem");
    MLayout *layout = new MLayout();
    MGridLayoutPolicy *policy = new MGridLayoutPolicy(layout);

    mContactPic = new MImageWidget();
    mContactPic->setImage("icon-m-content-avatar-placeholder");
    mContactPic->setObjectName("contactMessageListItemPic");
    mContactPic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    policy->addItem(mContactPic, 0, 0, 2, 1, Qt::AlignTop);

    mContactStatusPic = new MImageWidget();
    mContactStatusPic->setObjectName("contactMessageListItemStatusPic");
    mContactStatusPic->setSizePolicy(QSizePolicy::Fixed,
                                     QSizePolicy::Fixed);
    policy->addItem(mContactStatusPic, 0, 1,Qt::AlignVCenter);

    mContactName = new MLabel("");
    mContactName->setTextElide(false);
    mContactName->setObjectName("contactMessageListItemName");
    mContactName->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
    policy->addItem(mContactName, 0, 2, Qt::AlignLeft);

    mLastMsg = new MLabel();
    mLastMsg->setTextElide(true);
    mLastMsg->setObjectName("contactMessageListItemLastMsgRead");
    mLastMsg->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
    policy->addItem(mLastMsg, 1, 2, Qt::AlignLeft);

    mLastMsgRcvd = new MLabel();
    mLastMsgRcvd->setObjectName("contactMessageListItemLastMsgRcvdRead");
    mLastMsgRcvd->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
    policy->addItem(mLastMsgRcvd, 2, 2, Qt::AlignLeft);

//    MStylableWidget *spacer = new MStylableWidget();
//    spacer->setObjectName("dialogItemSpacer");
//    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    policy->addItem(spacer, 3, 0, 1, 3);

    layout->setPolicy(policy);
    setLayout(layout);
}


void ContactMessageListItem::setName(QString contactName)
{
    //Set contact name
    mContactName->setText(contactName);
}

void ContactMessageListItem::setStatus()
{
    mContactStatusPic->setImage(Acct::mapAccountStatusToIconName(mContact->getStatus()), QSize(24, 24));
}

void ContactMessageListItem::setAvatar(QPixmap *avatar)
{
    QPixmap qpm;
    if (!avatar)
        return;
    if (avatar->height() > avatar->width())
        qpm = avatar->scaledToHeight(48);
    else
        qpm = avatar->scaledToWidth(48);
    mContactPic->setPixmap(qpm);
}

void ContactMessageListItem::setLastMsg(QString contactLastMsg)
{
    //Set status message
    mContactLastMsg = contactLastMsg;
    setContactLastMsg();
}

void ContactMessageListItem::setContactLastMsg()
{
    mLastMsg->setText(mContactLastMsg);
    //TODO: Convert mLastMsg to a ReadUnreadLabel
    if (mUnread)
        mLastMsg->setObjectName("contactMessageListItemLastMsgUnread");
    else
        mLastMsg->setObjectName("contactMessageListItemLastMsgRead");
}

void ContactMessageListItem::setUnread(bool unRead)
{
    //Set unread msg status
    mUnread = unRead;
    setContactLastMsg();
    setDTText();
}

void ContactMessageListItem::setLastMsgRcvd(QDateTime lastMsgRcvd)
{
    //Set last message datetime
    mLastMsgRcvdDT = lastMsgRcvd;
    setDTText();
}

void ContactMessageListItem::setLastMsgDir(Message::Status mDir)
{
    mLastMsgDir = mDir;
    setDTText();
}

void ContactMessageListItem::setDTText()
{
    QString dtDir;

    if (mUnread)
    {
	//% "New Message!"
        mLastMsgRcvd->setText(qtTrId("messages_view_new_message_for_contact"));
        mLastMsgRcvd->setObjectName("contactMessageListItemLastMsgRcvdUnread");
        return;
    } else
    {
        mLastMsgRcvd->setObjectName("contactMessageListItemLastMsgRcvdRead");
    }

    if (mLastMsgDir == Message::RECEIVED) {
        //% "Received "
        dtDir = qtTrId("msg_direction_received");
    } else if (mLastMsgDir == Message::SENT) {
        //% "Sent "
        dtDir = qtTrId("msg_direction_sent");
    } else {
        mLastMsgRcvd->setText(" ");
        return;
    }

//    if (mLastMsgRcvdDT.date() == QDate::currentDate())
//        dtDay = "today at ";
//    else if (mLastMsgRcvdDT.date() == QDate::currentDate().addDays(-1))
//        dtDay = "yesterday at ";
//    else if (mLastMsgRcvdDT.date() >= QDate::currentDate().addDays(-6))
//        dtDay = mLastMsgRcvdDT.toString("dddd").append(" at ");
//    else
//        dtDay = mLastMsgRcvdDT.toString("m/d/yy").append(" at ");
//    dtTime = mLastMsgRcvdDT.toString("h:mmap");
    mLastMsgRcvd->setText(dtDir + toFuzzyTime2(mLastMsgRcvdDT));
}

void ContactMessageListItem::setContact(ChatContact *contact)
{
    mContact = contact;
    setName(mContact->getName());
    setStatus();
    //TODO: implement unread in libmeegochat
//    setUnread(mContact->getUnread());
    setAvatar(mContact->getAvatar());

    Message *msg = mContact->getLastMessage();
    if (msg){
        setLastMsg(msg->getText());
        setLastMsgRcvd(msg->getDateTime());
        setLastMsgDir(msg->getStatus());
    } else {
        setLastMsg("");
        setLastMsgRcvd(QDateTime());
        setLastMsgDir(Message::NONE);
    }

    connect(mContact,
            SIGNAL(ContactChanged(ChatContact*)),
            this,
            SLOT(setContact(ChatContact*)));

}

void ContactMessageListItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mPressed = true;
    style().setModePressed();
    update();
    ev->accept();
}

void ContactMessageListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (mPressed) {
        mPressed = false;
        if (mUnread)
            style().setModeDefault();
//        else
//            style().setModeRead();
        update();
        ev->accept();
        emit clicked();
    } else {
        ev->ignore();
    }
}

void ContactMessageListItem::cancelEvent(MCancelEvent *ev)
{
    mPressed = false;
    if (mUnread)
        style().setModeDefault();
//    else
//        style().setModeRead();
    update();
    ev->accept();
}
