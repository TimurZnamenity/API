#ifndef PROJECT_REQUEST_HPP
#define PROJECT_REQUEST_HPP

#include <QObject>
#include "Global.hpp"
#include "Requester.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Абстрактный класс для строительства запросов.
 */
class Request : public QObject
{
    Q_OBJECT

/* ############################################################################################## */
public: /* Конструкторы / деструктор */
/* ############################################################################################## */

    explicit Request(QObject* parent = nullptr);
    virtual ~Request();

/* ############################################################################################## */
protected: /* Виртуальные методы (и не только) для наследников */
/* ############################################################################################## */

    /// Обработчик запросов.
    Requester*  m_requester;

    /**
     * Возвращает строковый ИД пользователя.
     * @return ИД пользователя
     */
    QString userId() const;

    /**
     * Обрабатывает ответ на запрос.
     */
    virtual void parseResponse(const QJsonObject& jsonObject) = 0;

    /**
     * Обрабатывает ошибки в ответе на запрос.
     */
    virtual void parseError(const QJsonObject& jsonObject);

    /**
     * Формирует строку запроса api.
     * @return API-строка
     */
    virtual QString constructApiString() const = 0;
};

PROJECT_NAMESPACE_END


#endif // PROJECT_REQUEST_HPP
