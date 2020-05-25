#ifndef PROJECT_POST_REQUEST_CASHOUT_HPP
#define PROJECT_POST_REQUEST_CASHOUT_HPP

#include "Global.hpp"
#include "PostRequest.hpp"
#include "Money.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Запрос "Вывод средств".
 */
class PostRequestCashout : public PostRequest
{
    Q_OBJECT

public:
    PostRequestCashout(QObject* parent = nullptr);
    ~PostRequestCashout() {
        PROJECT_QDEBUG << "Request <Cashout> destroyed";
    }


    /**
     * Инициация отправки запроса POST.
     * Перед отправкой нужно подписаться на сигнал получения ответа (у класса-наследника)
     * @param data - Body запроса (form-data)
     * @return приготовленные для post form-data
     */
    QUrlQuery formData(Money& sum, QString& method) const;

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
    void cashoutPassed();

    /**
     * Сигнал передает сообщение об ошибке.
     */
    void cashoutPassError(QString error);
};

PROJECT_NAMESPACE_END

#endif // PROJECT_POST_REQUEST_CASHOUT_HPP
