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


#include "dialogpageheader.h"

#include <MLinearLayoutPolicy>

#include <libmeegochat/meegochatcontact.h>
#include <libmeegochat/meegochataccount.h>


DialogPageHeader::DialogPageHeader(ChatContact *contact,
                                   MWidget *parent) :
    MStylableWidget(parent),
    mOffline(false),
    mContact(0)
{
    MLayout *headerLayout = new MLayout();
    //MGridLayoutPolicy *headerPolicy = new MGridLayoutPolicy(headerLayout);
    MLinearLayoutPolicy *headerPolicy = new MLinearLayoutPolicy(headerLayout, Qt::Horizontal);

    mPic = new MImageWidget();
    mPic->setPreferredSize(60, 60);
    mPic->setMinimumSize(60, 60);
    mPic->setMaximumSize(60, 60);
    mPic->setObjectName("dialogPageContactPic");
    mPic->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //headerPolicy->addItem(mPic, 0, 0, 2, 1, Qt::AlignVCenter);
    headerPolicy->addItem(mPic, Qt::AlignCenter);

    mStatusPic = new MImageWidget();
    mStatusPic->setObjectName("dialogPageStatusPic");
    mStatusPic->setSizePolicy(QSizePolicy::Fixed,
                                     QSizePolicy::Fixed);
    mStatusPic->setContentsMargins(0,0,0,0);
    //headerPolicy->addItem(mStatusPic, 0, 1, Qt::AlignVCenter);
    headerPolicy->addItem(mStatusPic, Qt::AlignCenter);

    mName = new MLabel();
    mName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mName->setObjectName("dialogPageNameLabel");
    //headerPolicy->addItem(mName, 0, 2, Qt::AlignLeft | Qt::AlignBottom);
    headerPolicy->addItem(mName, Qt::AlignLeft | Qt::AlignVCenter);
/*
    mStatusMsg = new MLabel();
    mStatusMsg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mStatusMsg->setObjectName("dialogPageStatusMsgLabel");
    headerPolicy->addItem(mStatusMsg, 1, 2, Qt::AlignLeft);
    this->setContactStatusMsg(mStrStatusMessage);
*/
    headerLayout->setPolicy(headerPolicy);
    setLayout(headerLayout);
    setContact(contact);

}

void DialogPageHeader::setContactAvatar(QPixmap *contactAvatar)
{
    //If we don't have a contact avatar for them, just go w/ default and return
    if (!contactAvatar) {
        mPic->setImage("icon-m-content-avatar-placeholder");
        return;
    }
    if (contactAvatar->height() > contactAvatar->width())
        qpm = contactAvatar->scaledToHeight(60);
    else
        qpm = contactAvatar->scaledToWidth(60);
    mPic->setPixmap(qpm);
}

void DialogPageHeader::setContactName(QString name)
{
    mName->setText(name);
}

void DialogPageHeader::setContactStatus()
{
    mStatusPic->setImage(
            Acct::mapAccountStatusToIconName(mContact->getStatus()),
            QSize(24, 24));
}

/*
void DialogPageHeader::setContactStatusMsg(QString statusMsg)
{
    //mStrStatusMessage = statusMsg;
    setOffline(mOffline);
}
*/

void DialogPageHeader::setOffline(bool offline)
{
    mOffline = offline;
    /*if (mOffline)
        mStatusMsg->setText(mStrStatusMessage.append(" (offline, reconnecting...)"));
    else
        mStatusMsg->setText(mStrStatusMessage);*/
}

void DialogPageHeader::setContact(ChatContact *contact)
{
    if (mContact)
        disconnect(mContact,
                   SIGNAL(ContactChanged(ChatContact*)),
                   this,
                   SLOT(onContactChanged(ChatContact*)));
    mContact = contact;
    if (mContact) {
        setContactName(mContact->getName());
        setContactStatus();
    //    setContactStatusMsg(mContact->getStatusMsg());
        setContactAvatar(mContact->getAvatar());

        connect(mContact,
                SIGNAL(ContactChanged(ChatContact*)),
                this,
                SLOT(onContactChanged(ChatContact*)));
    }

}

void DialogPageHeader::onContactChanged(ChatContact *contact)
{
    setContact(contact);
}
