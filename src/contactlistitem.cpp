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



#include <MWidgetCreator>
#include <MWidget>
#include <MGridLayoutPolicy>
#include <MCancelEvent>

#include <libmeegochat/meegochataccount.h>

#include "contactlistitem.h"

M_REGISTER_WIDGET(ContactListItem);

ContactListItem::ContactListItem(MWidget *parent) :
    MListItem(parent),
    mActDelContact(0),
    mActOpenPeople(0),
    mPressed(false)
{
    setObjectName("contactListItem");
   /* MLayout *hLayout = new MLayout();
    MLinearLayoutPolicy *hPolicy = new MLinearLayoutPolicy(hLayout,
                                                              Qt::Horizontal);
    MLayout *vLayout = new MLayout();
    MLinearLayoutPolicy *vPolicy = new MLinearLayoutPolicy(vLayout,
                                                               Qt::Vertical);
*/
    MLayout *layout = new MLayout();
    MGridLayoutPolicy *gPolicy = new MGridLayoutPolicy(layout);

    mContactPic = new MImageWidget();
    mContactPic->setImage("icon-m-content-avatar-placeholder");
    mContactPic->setObjectName("contactListItemPic");
    mContactPic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    hPolicy->addItem(mContactPic, Qt::AlignCenter);
    gPolicy->addItem(mContactPic, 0, 0, 2, 1, Qt::AlignHCenter | Qt::AlignTop);


    mContactStatusPic = new MImageWidget();
    mContactStatusPic->setObjectName("contactListItemStatusPic");
    mContactStatusPic->setSizePolicy(QSizePolicy::Fixed,
                                     QSizePolicy::Expanding);
//    hPolicy->addItem(mContactStatusPic, Qt::AlignCenter);
    gPolicy->addItem(mContactStatusPic, 0, 1);

    mContactName = new MLabel("");
    mContactName->setTextElide(false);
    mContactName->setObjectName("contactListItemName");
    mContactName->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
//    hPolicy->addItem(mContactName, Qt::AlignLeft);
    gPolicy->addItem(mContactName, 0, 2, Qt::AlignLeft);

    mContactStatusMsg = new MLabel("");
    mContactStatusMsg->setTextElide(true);
    mContactStatusMsg->setObjectName("contactListItemStatusMsg");
    mContactStatusMsg->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
//    hPolicy->addItem(mContactStatusMsg, Qt::AlignLeft);
    gPolicy->addItem(mContactStatusMsg, 1, 2, Qt::AlignLeft);

//    hLayout->setPolicy(hPolicy);
    layout->setPolicy(gPolicy);

    /*MStylableWidget *spacer = new MStylableWidget();
    spacer->setObjectName("dialogItemSpacer");
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    vPolicy->addItem(hLayout);
    //vPolicy->addItem(spacer);
    vLayout->setPolicy(vPolicy);*/

    setLayout(layout);

}

void ContactListItem::onActDelContactClicked()
{
    mContact->removeContact();
}

void ContactListItem::onActOpenPeopleClicked()
{
    mContact->openPeople();
}


void ContactListItem::setName(QString contactName)
{
    //Set contact name
    mContactName->setText(contactName);
}

void ContactListItem::setStatus()
{
    mContactStatusPic->setImage(Acct::mapAccountStatusToIconName(mContact->getStatus()), QSize(24, 24));
}

void ContactListItem::setAvatar(QPixmap *avatar)
{
    QPixmap qpm;
    if (!avatar) {
        mContactPic->setImage("icon-m-content-avatar-placeholder");
        return;
    }
    if (avatar->height() > avatar->width())
        qpm = avatar->scaledToHeight(48);
    else
        qpm = avatar->scaledToWidth(48);
    mContactPic->setPixmap(qpm);
}

void ContactListItem::setContact(ChatContact *contact)
{
    mContact = contact;
    setName(mContact->getName());
    setStatus();
    setStatusMsg(mContact->getStatusMsg());
    setAvatar(mContact->getAvatar());

    if (mContact->getQContact() && !mActOpenPeople) {
        //% "View Contact Details"
        mActOpenPeople = new MAction(qtTrId("action_view_contact_details"), this);
        this->addAction(mActOpenPeople);
        connect(mActOpenPeople,
                SIGNAL(triggered()),
                this,
                SLOT(onActOpenPeopleClicked()));
    }

    if (!mActDelContact) {
        //% "Delete Contact"
        mActDelContact = new MAction(qtTrId("action_delete_contact"), this);
        this->addAction(mActDelContact);
        connect(mActDelContact,
                SIGNAL(triggered()),
                this,
                SLOT(onActDelContactClicked()));
    }


}

void ContactListItem::setStatusMsg(QString contactStatusMsg)
{
    //Set status message
    if (contactStatusMsg.isEmpty() || contactStatusMsg.isNull())
        mContactStatusMsg->setText(" ");
    else
        mContactStatusMsg->setText(contactStatusMsg);
}

void ContactListItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mPressed = true;
    style().setModePressed();
    update();
    ev->accept();
}

void ContactListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (mPressed) {
        mPressed = false;
        style().setModeDefault();
        update();
        ev->accept();
        emit clicked();
    } else {
        ev->ignore();
    }
}

void ContactListItem::cancelEvent(MCancelEvent *ev)
{
    mPressed = false;
    style().setModeDefault();
    update();
    ev->accept();
}
