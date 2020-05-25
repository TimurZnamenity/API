#include "GetRequestUser.hpp"



using namespace PROJECT;

/* ---------------------------------------------------------------------------------------------- */
GetRequestUser::GetRequestUser(QObject* parent) : GetRequest(parent)
{

}


/* ---------------------------------------------------------------------------------------------- */
void GetRequestUser::parseResponse(const QJsonObject& jsonObject)
{
    QJsonObject   dataObject = jsonObject;

    // вытаскиваем data
    uint64_t id         = static_cast<uint64_t>( dataObject["id"].toInt() );
    QString name        = dataObject["name"].toString();
    QString avatar      = dataObject["avatar"].toString();
    double  balance     = dataObject["balance"].toDouble();
    QString output      = QString("%1\n%2\n%3\n%4")
                                .arg(id)
                                .arg(name)
                                .arg(avatar)
                                .arg(balance);

    // теперь разбираем по-настоящему
    auto user = new User();

    user->id     = id;
    user->name   = name;
    user->avatar = avatar;
    user->cash   = Money(balance);

    PROJECT_QDEBUG << "Before emit <User>";

    emit userUpdated( QSharedPointer<User>( user ) );

    PROJECT_QDEBUG << "Request <User> parsed";

    return;
}

/* ---------------------------------------------------------------------------------------------- */
//void GetRequestUser::parseError(const QJsonObject&)
//{
//    return;
//}

/* ---------------------------------------------------------------------------------------------- */
QString GetRequestUser::constructApiString() const
{
    // извлекаем ИД пользователя из токена
    QString idString = userId();

    // здесь подставляем ИД пользователя
    QString apiStr = "/users/" + idString;

    return  apiStr;
}
