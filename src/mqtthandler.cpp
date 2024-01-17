/**
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

#include "mqtthandler.h"

#include <QCoreApplication>
#include <QTextStream>
#include <QJsonDocument>

MqttHandler::MqttHandler(const Deconz2MQTTConfig &config, const QString &topic, QObject *parent)
    : QObject(parent)
    , m_topic(topic)
{
    m_client.setProtocolVersion(config.mqttVersion());
    m_client.setHostname(config.mqttHostname());
    m_client.setPort(config.mqttPort());
    QMqttConnectionProperties props;
    m_client.setConnectionProperties(props);
    connect(&m_client, &QMqttClient::errorChanged, this, &MqttHandler::onConnectionError);
    //connect(&m_client, &QMqttClient::connected, this, &MqttSubscription::subscribe);

    if (!config.mqttUsername().isEmpty() && !config.mqttPassword().isEmpty())
    {
        m_client.setUsername(config.mqttUsername());
        m_client.setPassword(config.mqttPassword());
    }

    if (config.mqttUseTls())
    {
        QSslConfiguration sslconfig;
        sslconfig.defaultConfiguration();
        sslconfig.setProtocol(QSsl::TlsV1_2);
        sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        m_client.connectToHostEncrypted(sslconfig);
    }
    else
    {
        m_client.connectToHost();
    }
}

void MqttHandler::handleMessage(const QVariant &msgContent)
{
    if (msgContent.type() == QVariant::Map)
    {
        QVariantMap map = msgContent.toMap();
        QString role = map.value("r").toString();
        QString type = map.value("t").toString();
        QString event = map.value("e").toString();
        if (event == "changed" && type == "event" && role == "sensors")
        {
            QString uniqueid = map.value("uniqueid").toString();
            if (map.contains("state"))
            {
                QVariant state = map.value("state");

                QMqttTopicName topic(QString("deconz/%1/state").arg(uniqueid));
                m_client.publish(topic, QJsonDocument::fromVariant(state).toJson());
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

void MqttHandler::subscribe()
{
    QTextStream(stdout) << "MQTT connection established" << Qt::endl;

    m_subscription = m_client.subscribe(m_topic);
    if (!m_subscription) {
        qDebug() << "Failed to subscribe to " << m_topic;
        emit errorOccured(m_client.error());
    }

    connect(m_subscription, &QMqttSubscription::stateChanged, this,
            [](QMqttSubscription::SubscriptionState s) {
        QTextStream(stdout) << "Subscription state changed: " << s << Qt::endl;
    });

    connect(m_subscription, &QMqttSubscription::messageReceived, this,
            [this](QMqttMessage msg) {
//        handleMessage(msg.payload());
        QTextStream(stdout) << "MQTT message received: " << msg.topic().name() << ", " << msg.payload() << Qt::endl;
    });
}

void MqttHandler::onConnectionError(QMqttClient::ClientError error)
{
    if (error != QMqttClient::NoError)
    {
        QTextStream(stderr) << "MQTT error: " << error << Qt::endl;
        emit errorOccured(error);
    }
}
