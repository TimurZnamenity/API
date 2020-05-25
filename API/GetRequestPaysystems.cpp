#include "GetRequestPaysystems.hpp"



using namespace PROJECT;


GetRequestPaysystems::GetRequestPaysystems(QObject *parent) : GetRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
void GetRequestPaysystems::parseResponse(const QJsonObject& jsonObject)
{
    QJsonObject   dataObject;
    QJsonArray    dataArray;

    if( jsonObject["data"].isArray() )
        dataArray = jsonObject["data"].toArray();
    else
    {
        PROJECT_QDEBUG << "Paysystems request does not contain JsonArray";
        return; // обработка ошибки
    }

    // теперь разбираем по-настоящему
    auto paysystems = new Paysystems();

    // пробегаем по всем плат. системам
    for( int i = 0; !dataArray.at(i).isUndefined(); i++ )
    {
        // проверяем
        if( dataArray.at( i ).isObject() )
            dataObject = dataArray.at( i ).toObject();
        else
        {
            PROJECT_QDEBUG << "Paysystem is not JsonObject";
            return;
        }

        // выгребаем транзакцию
        Paysystem paysystem;
        QString key           = dataObject["key"].toString();
        paysystem.name        = dataObject["name"].toString();
        paysystem.max         = Money( dataObject["max"].toDouble() );
        paysystem.min         = Money( dataObject["min"].toDouble() );
        paysystem.commission  = dataObject["commission"].toDouble();
        paysystem.staticCommission
                              = Money( dataObject["staticCommission"].toDouble() );
        paysystem.placeholder = dataObject["placeholder"].toString();
        paysystem.iconUrl     = dataObject["iconUrl"].toString();

        // добавляем ее в список
        paysystems->insert( key, paysystem );
    }

    PROJECT_QDEBUG << "Before emit <Paysystems>";
    emit paysystemsUpdated( QSharedPointer<Paysystems>( paysystems ) );
    PROJECT_QDEBUG << "Request <Paysystems> parsed";

    return;
}

/* ---------------------------------------------------------------------------------------------- */
//void GetRequestPaysystems::parseError(const QJsonObject&)
//{
//    return;
//}

/* ---------------------------------------------------------------------------------------------- */
QString GetRequestPaysystems::constructApiString() const
{
    // здесь подставляем ИД пользователя
    QString apiStr = "/methods";

    return  apiStr;
}
