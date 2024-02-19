/*
    QDeconz2MQTT publishes deconz events to a MQTT broker.
    Copyright (C) 2024  Thomas Zimmermann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QVariant>

#include "deconz2mqttconfig.h"

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const Deconz2MQTTConfig &config, QObject *parent = nullptr);
    
signals:
    void closed();
    void messageReceived(const QString & uniqueid, const QString & type, const QVariant & data);
    
private slots:
    void onTextMessageReceived(const QString & message);
    
private:
    QWebSocket m_webSocket;
    QMap<QString, QVariantMap> m_latestdata; ///< uniqueid, JSON as QVariantMap
};

#endif // WEBSOCKETCLIENT_H
