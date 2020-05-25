#include "PostRequestCashout.hpp"



using namespace PROJECT;

/* ---------------------------------------------------------------------------------------------- */
PostRequestCashout::PostRequestCashout(QObject* parent) : PostRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
QUrlQuery PostRequestCashout::formData(Money& sum, QString& method) const
{
    QUrlQuery postData;
    postData.addQueryItem("sum", sum.toStr());
    postData.addQueryItem("method", method);
    return postData;
}


/* ---------------------------------------------------------------------------------------------- */
void PostRequestCashout::parseResponse(const QJsonObject& jsonObject)
{
    // структура ответа
    // здесь парсим ответ - успешный cashout или нет
    emit cashoutPassed();

    return;
}

/* ---------------------------------------------------------------------------------------------- */
void PostRequestCashout::parseError(const QJsonObject& jsonObject)
{
    QString error = jsonObject["message"].toString();
    emit cashoutPassError(error);
    return;
}

/* ---------------------------------------------------------------------------------------------- */
QString PostRequestCashout::constructApiString() const
{
    // извлекаем ИД пользователя из токена
    QString idString = userId();

    // здесь подставляем ИД пользователя
    QString apiStr = "/users/" + idString + "/withdraw";

    return  apiStr;
}
