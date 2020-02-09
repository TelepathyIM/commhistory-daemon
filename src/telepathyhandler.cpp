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

#include "telepathyhandler.h"
#include "eventsmonitor.h"
#include "chatreadhandler.h"

#include <TelepathyQt/PendingReady>

#include <CommHistory/SingleEventModel>

TelepathyHandler::TelepathyHandler(QObject *parent, Tp::AccountManagerPtr manager, EventsMonitor *eventsMonitor)
    : QObject(parent),
      m_accountManager(manager),
      m_eventsMonitor(eventsMonitor)
{
    if (m_accountManager.isNull()) {
        qWarning() << "Account manager is null!";
    } else {
        if (!m_accountManager->isReady()) {
            connect(m_accountManager->becomeReady(), &Tp::PendingReady::finished,
                this, &TelepathyHandler::slotAccountManagerReady);
        } else {
            connectToAccounts();
        }
    }
}

void TelepathyHandler::slotAccountManagerReady(Tp::PendingOperation *op)
{
    if (op->isError()) {
        qWarning() << "Account manager cannot become ready:" << op->errorName() << "-" << op->errorMessage();
        return;
    }

    connectToAccounts();
}

void TelepathyHandler::slotConnectToSignals(const Tp::AccountPtr &account)
{
    if (!account.isNull() && !m_handlers.contains(account.data())) {
        auto chatReadHandler = new ChatReadHandler(this, account, m_eventsMonitor);
        m_handlers.insert(account.data(), chatReadHandler);

        connect(account.data(), &Tp::Account::removed, this,
            &TelepathyHandler::slotAccountRemoved, Qt::UniqueConnection);
    }
}

void TelepathyHandler::slotAccountRemoved()
{
    Tp::Account* account = qobject_cast<Tp::Account*>(sender());
    if (!account) {
        qWarning() << Q_FUNC_INFO << "Account is null";
        return;
    }

    if (m_handlers.contains(account)) {
        m_handlers[account]->deleteLater();
        m_handlers.remove(account);
    }
}

void TelepathyHandler::connectToAccounts()
{
    connect(m_accountManager.data(), &Tp::AccountManager::newAccount,
        this, &TelepathyHandler::slotConnectToSignals, Qt::UniqueConnection);

    foreach(const Tp::AccountPtr &account, m_accountManager->allAccounts()) {
        slotConnectToSignals(account);
    }
}
