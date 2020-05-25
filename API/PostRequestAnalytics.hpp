#ifndef PROJECT_POST_REQUEST_ANALYTHICS_HPP
#define PROJECT_POST_REQUEST_ANALYTHICS_HPP

#include "Global.hpp"
#include "PostRequest.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Запрос "Аналитика".
 */
class PostRequestAnalytics : public PostRequest
{
    Q_OBJECT

public:
    PostRequestAnalytics(QObject* parent = nullptr);
    ~PostRequestAnalytics() {
        PROJECT_QDEBUG << "Request <Analytics> destroyed";
    }

private:

    /**
     * Обрабатывает ответ на запрос.
     */
    void parseResponse(const QJsonObject& jsonObject) override;

    /**
     * Обрабатывает ошибки в ответе на запрос.
     */
    void parseError(const QJsonObject& jsonObject) override;

    /**
     * Формирует строку запроса api.
     * @return API-строка
     */
    QString constructApiString() const override;

signals:
    /**
     * Сигнал сообщает что вывод средств успешен.
     */
    void analyticsPassed();

    /**
     * Сигнал передает сообщение об ошибке.
     */
    void analyticsPassError(QString error);
};

PROJECT_NAMESPACE_END

#endif // PROJECT_POST_REQUEST_ANALYTHICS_HPP
