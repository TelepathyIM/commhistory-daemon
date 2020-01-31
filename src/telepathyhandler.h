/******************************************************************************
**
** This file is part of commhistory-daemon.
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License version 2.1 as
** published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
** License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
**
******************************************************************************/

#ifndef TELEPATHYHANDLER_H
#define TELEPATHYHANDLER_H

#include <QObject>

#include <CommHistory/event.h>
#include <CommHistory/group.h>

class EventsMonitor;

class TelepathyHandler : public QObject
{
    Q_OBJECT

public:
    explicit TelepathyHandler(QObject *parent, EventsMonitor *eventsMonitor);

private Q_SLOTS:
    void onEventsUpdated(const QList<CommHistory::Event> &events);
    void onGroupsUpdatedFull(const QList<CommHistory::Group> &groups);
};

#endif // TELEPATHYHANDLER_H
