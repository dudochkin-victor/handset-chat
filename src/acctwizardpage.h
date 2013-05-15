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



#ifndef ACCTWIZARDPAGE_H
#define ACCTWIZARDPAGE_H

#include <QObject>
#include <MApplicationPage>
#include <MButton>

class AcctWizardPage : public MApplicationPage
{
    Q_OBJECT
public:
    AcctWizardPage();
    ~AcctWizardPage();

private slots:
    void btnNextClicked();
    void onMCPFinished(int exitcode);

private:
    MButton *btnNext;
};

#endif // ACCTWIZARDPAGE_H
