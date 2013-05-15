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



#ifndef CONTACTMESSAGELISTITEM_H
#define CONTACTMESSAGELISTITEM_H

#include <QGraphicsSceneMouseEvent>
#include <MStylableWidget>
#include <MListItem>
#include <MCancelEvent>
#include <MLayout>
#include <MImageWidget>
#include <MLabel>

#include <libmeegochat/meegochatmessage.h>

#include "contactmessagelistitemstyle.h"
#include "timeutil.h"

namespace MeeGoChat {
    class ChatContact;
}

using namespace MeeGoChat;

class ContactMessageListItem : public MListItem
{
Q_OBJECT
public:
    ContactMessageListItem(MWidget *parent = 0);
    void setContactPic(QString contactPic);
    void setName(QString contactName);
    void setLastMsg(QString contactLastMsg);
    void setLastMsgRcvd(QDateTime lastMsgRcvd);
    void setLastMsgDir(Message::Status mDir);
    void setUnread(bool unRead);
    void setStatus();
    void setAvatar(QPixmap *avatar);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void cancelEvent(MCancelEvent*);

signals:
    void clicked();

public slots:
    void setContact(ChatContact *contact);

private:
    void setContactLastMsg();
    void setDTText();

    MLayout *mLayout;
    MImageWidget *mContactPic;
    MLabel *mContactName;
    MImageWidget *mContactStatusPic;
    MLabel *mLastMsg;
    MLabel *mLastMsgRcvd;
    bool mPressed;
    bool mUnread;
    QString mContactLastMsg;
    Message::Status mLastMsgDir;
    QDateTime mLastMsgRcvdDT;
    ChatContact *mContact;

    //M_STYLABLE_WIDGET(ContactMessageListItemStyle);

signals:

public slots:

};

#endif // CONTACTMESSAGELISTITEM_H
