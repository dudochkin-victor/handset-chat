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



#ifndef APPROVECONTACTWIDGET_H
#define APPROVECONTACTWIDGET_H

#include <MStylableWidget>
#include <MImageWidget>
#include <MLabel>

namespace MeeGoChat {
    class ChatContact;
}

using namespace MeeGoChat;

class ApproveContactWidget : public MStylableWidget
{
Q_OBJECT
public:
    explicit ApproveContactWidget(ChatContact *cnt, MWidget *parent = 0);

signals:
    void RequestDenied(ChatContact *);
    void RequestApproved(ChatContact *);
    void RequestHandled(ApproveContactWidget *);

public slots:

private slots:
    void onChatContactChanged();
    void onDenyClicked();
    void onApproveClicked();

private:
    void setAvatar();

    ChatContact *mContact;

    MImageWidget *mContactAvatarImg;
    MLabel *mContactNameLabel;
};

#endif // APPROVECONTACTWIDGET_H
