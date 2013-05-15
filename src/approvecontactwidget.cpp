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


#include "approvecontactwidget.h"

#include <MLayout>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MWidgetController>
#include <MButton>

#include <libmeegochat/meegochatcontact.h>


ApproveContactWidget::ApproveContactWidget(ChatContact *cnt, MWidget *parent) :
    MStylableWidget(parent),
    mContact(cnt),
    mContactAvatarImg(new MImageWidget(this)),
    mContactNameLabel(new MLabel("", this))
{
    //Connect this first thing, so we don't have as much of a race window
    //in the Create ChatContact->Request Avatar->Get Avatar (async) path
    connect(mContact,
            SIGNAL(ContactChanged(ChatContact*)),
            this,
            SLOT(onChatContactChanged()));

    this->setObjectName("ApproveContactWidget");


    MLayout *gLayout = new MLayout();
    MGridLayoutPolicy *gPolicy = new MGridLayoutPolicy(gLayout);


    //Header text:
    //% "New Contact Request!"
    MLabel *lblHeaderText = new MLabel(qtTrId("lbl_new_contact_request"));
    lblHeaderText->setAlignment(Qt::AlignLeft);
    lblHeaderText->setObjectName("ApproveContactWidget_HeaderLabel");
    gPolicy->addItem(lblHeaderText, 0, 0, 1, 2, Qt::AlignLeft);


    //Account info:
    MLayout *hAcctLayout = new MLayout();
    MLinearLayoutPolicy *hAcctPolicy = new MLinearLayoutPolicy(hAcctLayout, Qt::Horizontal);
    MWidgetController *mwcAcctInfo = new MWidgetController();
    mwcAcctInfo->setObjectName("ApproveContactWidget_AcctInfoController");

    //% "Account: "
    MLabel *lblAcctText = new MLabel(qtTrId("lbl_account"));
    lblAcctText->setObjectName("ApproveContactWidget_AcctTextLabel");
    lblAcctText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hAcctPolicy->addItem(lblAcctText);

    MLabel *lblAcctName = new MLabel(mContact->getAccount()->getDisplayName());
    lblAcctName->setObjectName("ApproveContactWidget_AcctNameLabel");
    hAcctPolicy->addItem(lblAcctName, Qt::AlignLeft);

    hAcctLayout->setPolicy(hAcctPolicy);
    mwcAcctInfo->setLayout(hAcctLayout);
    gPolicy->addItem(mwcAcctInfo, 1, 0, 1, 2, Qt::AlignLeft);


    //Contact info:
    MLayout *hContactLayout = new MLayout();
    MLinearLayoutPolicy *hContactPolicy = new MLinearLayoutPolicy(hContactLayout, Qt::Horizontal);
    MWidgetController *mwcContactInfo = new MWidgetController();
    mwcContactInfo->setObjectName("ApproveContactWidget_ContactInfoController");

    mContactAvatarImg->setObjectName("ApproveContactWidget_ContactAvatarImg");
    mContactAvatarImg->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mContactNameLabel->setObjectName("ApproveContactWidget_ContactNameLabel");
    hContactPolicy->addItem(mContactAvatarImg, Qt::AlignCenter | Qt::AlignVCenter);
    hContactPolicy->addItem(mContactNameLabel, Qt::AlignLeft | Qt::AlignVCenter);

    hContactLayout->setPolicy(hContactPolicy);
    mwcContactInfo->setLayout(hContactLayout);
    gPolicy->addItem(mwcContactInfo, 2, 0, 1, 2, Qt::AlignLeft);


    //Approve/Deny buttons:
    //% "Decline"
    MButton *btnDeny = new MButton(qtTrId("btn_decline"));
    btnDeny->setObjectName("ApproveContactWidget_DenyButton");
    connect(btnDeny,
            SIGNAL(clicked()),
            this,
            SLOT(onDenyClicked()));
    gPolicy->addItem(btnDeny, 3, 0, Qt::AlignCenter);

    //% "Approve"
    MButton *btnApprove = new MButton(qtTrId("btn_approve"));
    btnApprove->setObjectName("ApproveContactWidget_ApproveButton");
    connect(btnApprove,
            SIGNAL(clicked()),
            this,
            SLOT(onApproveClicked()));
    gPolicy->addItem(btnApprove, 3, 1, Qt::AlignCenter);


    gLayout->setPolicy(gPolicy);
    this->setLayout(gLayout);

    //Set up initial avatar and contact name
    onChatContactChanged();
}

void ApproveContactWidget::setAvatar()
{
    //Set up our avatar, properly scaled to correct size
    QPixmap *qpm = mContact->getAvatar();
    if (!qpm)
        return;
    if (qpm->height() > qpm->width())
        mContactAvatarImg->setPixmap(qpm->scaledToHeight(48));
    else
        mContactAvatarImg->setPixmap(qpm->scaledToWidth(48));
}

void ApproveContactWidget::onChatContactChanged()
{
    mContactNameLabel->setText(mContact->getName());
    setAvatar();
}

void ApproveContactWidget::onApproveClicked()
{
    mContact->getContactManager()->approveContact(mContact);
    emit this->RequestHandled(this);
}

void ApproveContactWidget::onDenyClicked()
{
    mContact->getContactManager()->denyContact(mContact);
    emit this->RequestHandled(this);
}
