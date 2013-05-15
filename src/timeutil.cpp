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

#include <MLocale>

#include "timeutil.h"

QString toFuzzyTime(const QDateTime &datetime)
{
    QDateTime now = QDateTime::currentDateTime();

    if (datetime.addSecs(30) > now) {
        //% "about 30 seconds ago"
        return qtTrId("time_30_seconds_ago");
    } else if (datetime.addSecs(60) > now) {
        //% "about 1 min ago"
        return qtTrId("time_1_minute_ago");
    } else if (datetime.addSecs(2 * 60) > now) {
        //% "about 2 mins ago"
        return qtTrId("time_2_minutes_ago");
    } else if (datetime.addSecs(5 * 60) > now) {
        //% "about 5 mins ago"
        return qtTrId("time_5_minutes_ago");
    } else if (datetime.addSecs(10 * 60) > now) {
        //% "about 10 mins ago"
        return qtTrId("time_10_minutes_ago");
    } else if (datetime.addSecs(20 * 60) > now) {
        //% "about 20 mins ago"
        return qtTrId("time_20_minutes_ago");
    } else if (datetime.addSecs(30 * 60) > now) {
        //% "about 30 mins ago"
        return qtTrId("time_30_minutes_ago");
    } else {
        MLocale locale;
        return locale.formatDateTime(datetime);
    }
}

QString toFuzzyTime2(const QDateTime &datetime)
{
    QString dtDay, dtTime;
    MLocale locale;
    if (datetime.date() == QDate::currentDate()) {
        //% "today"
        dtDay = qtTrId("day_string_today");
    } else if (datetime.date() == QDate::currentDate().addDays(-1)) {
        //% "yesterday"
        dtDay = qtTrId("day_string_yesterday");
    } else if (datetime.date() >= QDate::currentDate().addDays(-6)) {
        dtDay = locale.formatDateTime(datetime, "%a");
    } else {
        dtDay = locale.formatDateTime(datetime, "%x");
    }
    dtTime = locale.formatDateTime(datetime, "%X");
    //% "%1 at %2 - 1 is date component, 2 is time component"
    QString result = qtTrId("string_date_at_time").arg(dtDay, dtTime);
    return result;
}
