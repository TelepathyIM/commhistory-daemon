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

#include "chatreadhandler.h"
#include "eventsmonitor.h"

#include <TelepathyQt/Account>

#include <CommHistory/SingleEventModel>

ChatReadHandler::ChatReadHandler(QObject *parent, Tp::AccountPtr account, EventsMonitor *eventsMonitor)
    : QObject(parent),
      m_account(account)
{
    connect(eventsMonitor, &EventsMonitor::eventsUpdated, this, &ChatReadHandler::onEventsUpdated);
    connect(eventsMonitor, &EventsMonitor::groupsUpdatedFull, this, &ChatReadHandler::onGroupsUpdatedFull);
}

void ChatReadHandler::onEventsUpdated(const QList<CommHistory::Event> &events)
{
    Q_UNUSED(events);
}

void ChatReadHandler::onGroupsUpdatedFull(const QList<CommHistory::Group> &groups)
{
    if (!m_account || !m_account->isValidAccount()) {
        qWarning() << Q_FUNC_INFO << "Account is invalid";
        return;
    }

    foreach (CommHistory::Group g, groups) {
        const auto accountUid = g.localUid();
        if (m_account->objectPath() != accountUid) {
            continue;
        }

        CommHistory::SingleEventModel lastEventModel;
        lastEventModel.getEventById(g.lastEventId());
        CommHistory::Event lastEvent = lastEventModel.event();

        const auto remoteUids = lastEvent.recipients().remoteUids();
        if (remoteUids.size() > 1) {
            qWarning() << Q_FUNC_INFO << "More than 1 recipient found, will process only the first as for now";
        } else if (remoteUids.size() < 1) {
            qCritical() << Q_FUNC_INFO << "No remote uids stated for the event" << lastEvent.id();
            continue;
        }

        const auto contactId = remoteUids.first();
        const auto lastMessageToken = lastEvent.messageToken();

        m_account->markTextChatRead(contactId, lastMessageToken);
    }
}
