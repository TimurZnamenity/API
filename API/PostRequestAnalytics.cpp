#include "PostRequestAnalytics.hpp"



using namespace PROJECT;

/* ---------------------------------------------------------------------------------------------- */
PostRequestAnalytics::PostRequestAnalytics(QObject* parent) : PostRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
void PostRequestAnalytics::parseResponse(const QJsonObject& jsonObject)
{
    // структура ответа
    // здесь парсим ответ - успешный
    emit analyticsPassed();

    return;
}

/* ---------------------------------------------------------------------------------------------- */
void PostRequestAnalytics::parseError(const QJsonObject& jsonObject)
{
    QString error = jsonObject["message"].toString();
    emit analyticsPassError(error);
    return;
}

/* ---------------------------------------------------------------------------------------------- */
QString PostRequestAnalytics::constructApiString() const
{
    return QString( "/usage" );
}
