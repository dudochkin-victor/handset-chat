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




#include "acctwizardpage.h"

#include <QProcess>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLabel>

#include <DuiControlPanelIf>

AcctWizardPage::AcctWizardPage() :
        MApplicationPage()
{
    //% "Instant messenger"
    setTitle(qtTrId("im_app_name"));

    MLayout *layout = new MLayout;
    MLinearLayoutPolicy *policy =
            new MLinearLayoutPolicy(layout, Qt::Vertical);

    btnNext = new MButton;
    btnNext->setObjectName("btnAcctWizardNext");
    //% "Set up your IM account"
    btnNext->setText(qtTrId("btn_account_setup"));
    btnNext->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    MLabel *lblHeader = new MLabel;
    lblHeader->setWrapMode(QTextOption::WordWrap);
    lblHeader->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    lblHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //% "You have no accounts set up. Would you like to set one up now?"
    lblHeader->setText(qtTrId("lbl_no_accounts_setup"));
    lblHeader->setObjectName("lblAcctWizardHeader");

    connect(btnNext,
            SIGNAL(clicked()),
            SLOT(btnNextClicked()));

    policy->addItem(lblHeader, Qt::AlignHCenter);
    policy->addItem(btnNext, Qt::AlignHCenter);
    layout->setPolicy(policy);
    centralWidget()->setLayout(layout);
}

AcctWizardPage::~AcctWizardPage()
{

}

void AcctWizardPage::btnNextClicked()
{
        //TODO - convert to meegotouchcontrolpanel
/*    QProcess *mcp = new QProcess();
    connect(mcp, SIGNAL(finished(int)),
            this, SLOT(onMCPFinished(int)));
    mcp->start("mcp");*/
    DuiControlPanelIf *dcpIf = new DuiControlPanelIf();
    dcpIf->appletPage("MeeGoChatApplet");
    this->dismiss();
}

void AcctWizardPage::onMCPFinished(int exitcode)
{
    Q_UNUSED(exitcode);
    btnNext->setEnabled(true);
    this->dismiss();
}
