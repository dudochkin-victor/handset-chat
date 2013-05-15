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



#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include <QString>
#include <QPixmap>
#include <QGraphicsSceneMouseEvent>
#include <MListItem>
#include <MStylableWidget>
#include <MWidget>
#include <MLayout>
#include <MImageWidget>
#include <MLabel>
#include <MStylableWidget>
#include <MAction>

#include <libmeegochat/meegochatcontact.h>

#include "contactlistitemstyle.h"

using namespace MeeGoChat;

class ContactListItem : public MListItem
{
Q_OBJECT
public:
    ContactListItem(MWidget *parent = 0);
    void setName(QString contactName);
    void setStatusMsg(QString contactStatusMsg);
    void setStatus();
    void setAvatar(QPixmap *avatar);
    void setContact(ChatContact *contact);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void cancelEvent(MCancelEvent*);

signals:
    void clicked();

public slots:

private slots:
    void onActDelContactClicked();
    void onActOpenPeopleClicked();


private:
    MLayout *mLayout;
    MImageWidget *mContactPic;
    MLabel *mContactName;
    MImageWidget *mContactStatusPic;
    MLabel *mContactStatusMsg;
    MeeGoChat::ChatContact *mContact;
    MAction *mActDelContact;
    MAction *mActOpenPeople;
    bool mPressed;

    //M_STYLABLE_WIDGET(ContactListItemStyle);

};

#endif // CONTACTLISTITEM_H
