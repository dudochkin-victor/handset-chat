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



#ifndef DIALOGPAGEHEADER_H
#define DIALOGPAGEHEADER_H

#include <MStylableWidget>
#include <MImageWidget>
#include <MLayout>
#include <MLabel>

#include "dialogpageheaderstyle.h"

namespace MeeGoChat {
    class ChatContact;
}

using namespace MeeGoChat;

class DialogPageHeader : public MStylableWidget
{
Q_OBJECT
public:
    explicit DialogPageHeader(ChatContact *contact,
                              MWidget *parent = 0);
    void setContact(ChatContact *contact);
    void setOffline(bool offline);

signals:

public slots:
    void onContactChanged(ChatContact *contact);

private:
    void setContactAvatar(QPixmap *contactAvatar);
    void setContactName(QString name);
    void setContactStatus();
//    void setContactStatusMsg(QString statusMsg);

    MLayout *mLayout;
    MImageWidget *mPic;
    MImageWidget *mStatusPic;
    MLabel *mName;
    MLabel *mStatusMsg;
    QPixmap qpm;
    bool mOffline;
    QString mStrStatusMessage;
    ChatContact *mContact;

    M_STYLABLE_WIDGET(DialogPageHeaderStyle);

};

#endif // DIALOGPAGEHEADER_H
