/**
 * @file statemanager.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief
 * @version 0.1
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "mainwindow.h"

StateManager::StateManager() {}

/**
 * @brief This method saves all Rumba's, RumbaRC's and Obstacle's data into json file.
 *
 *This method uses getter methods:
 *  rumba->getAtributes()
 *  rumbaRC->getAtributes()
 *  obstacle->getAtributes()
 *
 * @param fileName
 * @param perent
 */
int StateManager::saveStateToJson(const QString &fileName, MainWindow *perent)
{
    qDebug() << "path:        .." + QDir::separator() + "examples" + QDir::separator() + fileName;
    QFile file(".." + QDir::separator() + "examples" + QDir::separator() + fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Couldn't open file for writing:" << file.errorString();
        return 1;
    }
    QJsonArray jsonRumbaArray;
    for (auto rumba : perent->getRumbas())
    {
        QJsonObject jsonObject = structToJson(rumba->getAtributes()); // Serialize Rumba attributes
        jsonRumbaArray.append(jsonObject);
    }

    QJsonArray jsonRumbaRCArray;
    for (auto rumbaRC : perent->getRumbasRC())
    {
        QJsonObject jsonObject = structToJson(rumbaRC->getAtributes()); // Serialize RumbaRC attributes
        jsonRumbaRCArray.append(jsonObject);
    }

    QJsonArray jsonObstacleArray;
    for (auto obstacle : perent->getObstacles())
    {
        QJsonObject jsonObject = structToJson(obstacle->getAtributes()); // Serialize Rumba attributes
        jsonObstacleArray.append(jsonObject);
    }

    QJsonObject jsonObject;
    jsonObject["rumbas"] = jsonRumbaArray;
    jsonObject["rumbasRC"] = jsonRumbaRCArray;
    jsonObject["obstacles"] = jsonObstacleArray;

    QJsonDocument doc(jsonObject);
    file.write(doc.toJson());
    file.close();
    return 0;
}

/**
 * @brief This method loads all Rumba's, RumbaRC's and Obstacle's data from json file and returns them to perent using setter methods.
 *
 *The setter methods:
 *  parent->setObstacles(obstacles);
 *  parent->setRumbasRC(rumbaRCs);
 *  parent->setRumbas(rumbas);
 *
 * @param fileName
 * @param parent
 */
int StateManager::loadStateFromJson(const QString &fileName, MainWindow *parent)
{
    QFile file(".." + QDir::separator() + "examples" + QDir::separator() + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Couldn't open file for reading:" << file.errorString();
        return 1;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));

    // Check if the file parsing was successful
    if (doc.isNull())
    {
        qWarning() << "Failed to parse JSON file";
        file.close();
        return 1;
    }

    QJsonObject jsonObject = doc.object();

    QJsonArray rumbaArray = jsonObject["rumbas"].toArray();
    std::vector<Rumba::s> rumbas;
    for (int i = 0; i < rumbaArray.size(); ++i)
    {
        QJsonObject rumbaObject = rumbaArray[i].toObject();
        Rumba::s rumbaStruct = jsonToRumbaStruct(rumbaObject);
        rumbas.push_back(rumbaStruct);
    }

    QJsonArray rumbaRCArray = jsonObject["rumbasRC"].toArray();
    std::vector<RumbaRC::s> rumbaRCs;
    for (int i = 0; i < rumbaRCArray.size(); ++i)
    {
        QJsonObject rumbaRCObject = rumbaRCArray[i].toObject();
        RumbaRC::s rumbaRCStruct = jsonToRumbaRCStruct(rumbaRCObject);
        rumbaRCs.push_back(rumbaRCStruct);
    }

    QJsonArray obstacleArray = jsonObject["obstacles"].toArray();
    std::vector<Obstacle::s> obstacles;
    for (int i = 0; i < obstacleArray.size(); ++i)
    {
        QJsonObject obstacleObject = obstacleArray[i].toObject();
        Obstacle::s obstacleStruct = jsonToObstacleStruct(obstacleObject);
        obstacles.push_back(obstacleStruct);
    }

    parent->setObstacles(obstacles);
    parent->setRumbasRC(rumbaRCs);
    parent->setRumbas(rumbas);

    file.close();
    return 0;
}

/**
 * @brief Converts a QWidget subclass instance (Rumba, RumbaRC, or Obstacle) to a QJsonObject representation of its attributes.
 *
 * This function performs a dynamic cast to determine the specific type of the QWidget subclass (`obj`).
 * Based on the type, it calls the corresponding `structToJson` function for that subclass to convert
 * its attributes to a QJsonObject.
 *
 * @param obj The QWidget subclass instance to convert.
 * @return A QJsonObject containing the attributes of the provided QWidget subclass.
 */
QJsonObject StateManager::structToJson(QWidget *obj)
{
    Rumba *isActiveR = dynamic_cast<Rumba *>(obj);
    RumbaRC *isActiveRRC = dynamic_cast<RumbaRC *>(obj);
    Obstacle *isActiveO = dynamic_cast<Obstacle *>(obj);

    if (isActiveR)
        return structToJson(isActiveR->getAtributes());
    else if (isActiveRRC)
        return structToJson(isActiveRRC->getAtributes());
    else
        return structToJson(isActiveO->getAtributes());
}

/**
 * @brief Converts a Rumba struct to a QJsonObject representation of its attributes.
 *
 * @param Rumba
 * @return QJsonObject
 */
QJsonObject StateManager::structToJson(Rumba::s Rumba)
{
    QJsonObject jsonObject;
    jsonObject["x"] = Rumba.x;
    jsonObject["y"] = Rumba.y;
    jsonObject["last_x"] = Rumba.last_x;
    jsonObject["last_y"] = Rumba.last_y;
    jsonObject["radius"] = Rumba.radius;
    jsonObject["speed"] = Rumba.speed;
    jsonObject["rotationStep"] = Rumba.rotationStep;
    jsonObject["rotation"] = Rumba.rotation;
    jsonObject["detectionLen"] = Rumba.detectionLen;
    jsonObject["direction"] = Rumba.direction;
    return jsonObject;
}

/**
 * @brief Converts a QJsonObject to a Rumba struct representation.
 *
 * @param jsonObject
 * @return Rumba::s
 */
Rumba::s StateManager::jsonToRumbaStruct(const QJsonObject &jsonObject)
{
    Rumba::s myStruct;
    myStruct.x = jsonObject["x"].toDouble();
    myStruct.y = jsonObject["y"].toDouble();
    myStruct.last_x = jsonObject["last_x"].toDouble();
    myStruct.last_y = jsonObject["last_y"].toDouble();
    myStruct.radius = jsonObject["radius"].toInt();
    myStruct.speed = jsonObject["speed"].toInt();
    myStruct.rotationStep = jsonObject["rotationStep"].toInt();
    myStruct.rotation = jsonObject["rotation"].toInt();
    myStruct.detectionLen = jsonObject["detectionLen"].toInt();
    myStruct.direction = jsonObject["direction"].toBool();
    return myStruct;
}

/**
 * @brief Converts a RumbaRC struct to a QJsonObject representation of its attributes.
 *
 * @param rumbaRc
 * @return QJsonObject
 */
QJsonObject StateManager::structToJson(RumbaRC::s rumbaRc)
{
    QJsonObject jsonObject;
    jsonObject["x"] = rumbaRc.x;
    jsonObject["y"] = rumbaRc.y;
    jsonObject["last_x"] = rumbaRc.last_x;
    jsonObject["last_y"] = rumbaRc.last_y;
    jsonObject["radius"] = rumbaRc.radius;
    jsonObject["speed"] = rumbaRc.speed;
    jsonObject["rotation"] = rumbaRc.rotation;
    jsonObject["detectionLen"] = rumbaRc.detectionLen;
    return jsonObject;
}

/**
 * @brief Converts a QJsonObject to a RumbaRC struct representation.
 *
 * @param jsonObject
 * @return RumbaRC::s
 */
RumbaRC::s StateManager::jsonToRumbaRCStruct(const QJsonObject &jsonObject)
{
    RumbaRC::s myStruct;
    myStruct.x = jsonObject["x"].toDouble();
    myStruct.y = jsonObject["y"].toDouble();
    myStruct.last_x = jsonObject["last_x"].toDouble();
    myStruct.last_y = jsonObject["last_y"].toDouble();
    myStruct.radius = jsonObject["radius"].toInt();
    myStruct.speed = jsonObject["speed"].toInt();
    myStruct.rotation = jsonObject["rotation"].toInt();
    myStruct.detectionLen = jsonObject["detectionLen"].toInt();
    return myStruct;
}

/**
 * @brief Converts a Obstacle struct to a QJsonObject representation of its attributes.
 *
 * @param obstacle
 * @return QJsonObject
 */
QJsonObject StateManager::structToJson(Obstacle::s obstacle)
{
    QJsonObject jsonObject;
    jsonObject["x"] = obstacle.x;
    jsonObject["y"] = obstacle.y;
    jsonObject["width"] = obstacle.width;
    jsonObject["height"] = obstacle.height;
    return jsonObject;
}

/**
 * @brief Converts a QJsonObject to a Obstacle struct representation.
 *
 * @param jsonObject
 * @return Obstacle::s
 */
Obstacle::s StateManager::jsonToObstacleStruct(const QJsonObject &jsonObject)
{
    Obstacle::s myStruct;
    myStruct.x = jsonObject["x"].toDouble();
    myStruct.y = jsonObject["y"].toDouble();
    myStruct.width = jsonObject["width"].toInt();
    myStruct.height = jsonObject["height"].toInt();
    return myStruct;
}
