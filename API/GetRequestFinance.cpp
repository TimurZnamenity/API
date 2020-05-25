#include "GetRequestFinance.hpp"



using namespace PROJECT;

GetRequestFinance::GetRequestFinance(QObject *parent) : GetRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
void GetRequestFinance::parseResponse(const QJsonObject& jsonObject)
{
    auto dataObject = jsonObject;
    QJsonObject donationsObject;
    if( dataObject["donations"].isObject() )
        donationsObject = dataObject["donations"].toObject();
    else
    {
        PROJECT_QDEBUG << "Donations is not a QJsonObject";
        return;
    }

    // вытаскиваем data
    Money cash          = Money( dataObject["balance"].toDouble() );
    Money cashIn        = Money( donationsObject["sum"].toDouble() );
    Money cashOut       = Money( dataObject["cashout_sum"].toDouble() );
    auto donationAmount = static_cast<uint64_t>( donationsObject["count"].toInt() );

    // теперь разбираем по-настоящему
    auto finance = new Finance();

    finance->cash           = cash;
    finance->cashIn         = cashIn;
    finance->cashOut        = cashOut;
    finance->donationAmount = donationAmount;

    emit financeUpdated( QSharedPointer<Finance>( finance ) );

    return;
}

/* ---------------------------------------------------------------------------------------------- */
//void GetRequestFinance::parseError(const QJsonObject&)
//{
//    return;
//}

/* ---------------------------------------------------------------------------------------------- */
QString GetRequestFinance::constructApiString() const
{
    // извлекаем ИД пользователя из токена
    QString idString = userId();

    // здесь подставляем ИД пользователя
    QString apiStr = "/users/" + idString + "/stats";

    return  apiStr;
}
