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

#include "websocketclient.h"

#include <QTextStream>
#include <QJsonDocument>

WebSocketClient::WebSocketClient(const Deconz2MQTTConfig &config, QObject *parent)
    : QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::closed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    m_webSocket.open(config.websocketUrl());
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    QTextStream(stdout) << "Websocket message received: " << message << Qt::endl;

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &err);
    if (!doc.isNull())
    {
        QVariant var = doc.toVariant();
        if (var.type() == QVariant::Map)
        {
            QVariantMap map = var.toMap();
            QString role = map.value("r").toString();
            QString t = map.value("t").toString();
            QString event = map.value("e").toString();
            if (event == "changed" && t == "event" && role == "sensors")
            {
                const QString uniqueid = map.value("uniqueid").toString();
                if (!uniqueid.isEmpty())
                {
                    m_latestdata[uniqueid].insert(map);
                    const QVariantMap attr = m_latestdata[uniqueid].value("attr").toMap();
                    const QString type = attr.value("type").toString();
                    if (!type.isEmpty())
                    {
                        emit messageReceived(uniqueid, type, m_latestdata[uniqueid]);
                    }
                }
                else
                {
                    QTextStream(stderr) << "Json error: message does not contain a uniqueid" << Qt::endl;
                }
            }
            else
            {
                QTextStream(stderr) << "Json error: message does not contain: r = sensors, t = event and e = changed" << Qt::endl;
            }
        }
        else
        {
            QTextStream(stderr) << "Json error: message is not a QVariantMap" << Qt::endl;
        }
    }
    else
    {
        QTextStream(stderr) << "Json error: error while parsing message: " << message << ", Error message: " << err.errorString() << Qt::endl;
    }
}
