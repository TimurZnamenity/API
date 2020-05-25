#include "GetRequestSettings.hpp"



using namespace PROJECT;


GetRequestSettings::GetRequestSettings(QObject *parent) : GetRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
void GetRequestSettings::parseResponse(const QJsonObject& jsonObject)
{
    QJsonObject   dataObject = jsonObject;
    QJsonArray    dataArray;

    // теперь разбираем по-настоящему
    auto settings = new Settings();

    settings->currency = dataObject["accountCurrency"].toString();

    if( dataObject["withdrawWallets"].isArray() )
        dataArray = dataObject["withdrawWallets"].toArray();
    else
    {
        PROJECT_QDEBUG << "Settings request does not contain JsonArray";
        return; // обработка ошибки
    }

    // пробегаем по всем кошелькам
    for( int i = 0; !dataArray.at(i).isUndefined(); i++ )
    {
        QJsonObject object;

        // проверяем
        if( dataArray.at( i ).isObject() )
            object = dataArray.at( i ).toObject();
        else
        {
            PROJECT_QDEBUG << "Setting is not JsonObject";
            return;
        }

        // выгребаем кошелек
        Wallet wallet;
        wallet.key   = object["_key"].toString();
        wallet.value = object["value"].toString();

        // добавляем ее в список
        settings->wallets.push_back( wallet );
    }

    PROJECT_QDEBUG << "Before emit <Settings>";
    emit settingsUpdated( QSharedPointer<Settings>( settings ) );
    PROJECT_QDEBUG << "Request <Settings> parsed";

    return;
}

/* ---------------------------------------------------------------------------------------------- */
//void GetRequestSettings::parseError(const QJsonObject&)
//{
//    return;
//}

/* ---------------------------------------------------------------------------------------------- */
QString GetRequestSettings::constructApiString() const
{
    // извлекаем ИД пользователя из токена
    QString idString = userId();

    // здесь подставляем ИД пользователя
    QString apiStr = "/users/" + idString + "/settings";

    return  apiStr;
}
