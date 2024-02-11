#ifndef LIMITEDVALUE_H
#define LIMITEDVALUE_H

#include "nlohmann/json.hpp"
#include <atomic>

//https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

//class that takes into consideration value and associated limits [thread-safe]
template <class T>
class LimitedValue
{
public:
    LimitedValue(T value, T minLimit, T maxLimit)
    {
        m_value = value;
        m_minLimit = minLimit;
        m_maxLimit = maxLimit;
    };

    void SetValue(T value)
    {
        if (value <= m_minLimit)
        {
            m_value = m_minLimit.load();
        }
        else if (value >= m_maxLimit)
        {
            m_value = m_maxLimit.load();
        }
        else
        {
            m_value = value;
        }
    }

    T GetValue()
    {
        return m_value;
    }

    void SetMinLimit(T minLimit)
    {
        if (minLimit > m_maxLimit)
        {
            m_minLimit = m_maxLimit.load();
        }
        else
        {
            m_minLimit = minLimit;
        }
        //adjust value
        if (m_value < m_minLimit)
        {
            m_value = m_minLimit.load();
        }
    }

    T GetMinLimit()
    {
        return m_minLimit;
    }

    void SetMaxLimit(T maxLimit)
    {
        if (maxLimit < m_minLimit)
        {
            m_maxLimit = m_minLimit.load();
        }
        else
        {
            m_maxLimit = maxLimit;
        }
        //adjust value
        if (m_value > m_maxLimit)
        {
            m_value = m_maxLimit.load();
        }
    }

    T GetMaxLimit()
    {
        return m_maxLimit;
    }

    std::string GetConfig()
    {
        nlohmann::json config;
        config["value"] = m_value.load();
        config["minLimit"] = m_minLimit.load();
        config["maxLimit"] = m_maxLimit.load();
        return config.dump();
    }

    void SetConfig(std::string config)
    {
        nlohmann::json configJSON = nlohmann::json ::parse(config);
        if (configJSON.contains("value"))
        {
            m_value = configJSON["value"].get<T>();
        }
        if (configJSON.contains("minLimit"))
        {
            m_minLimit = configJSON["minLimit"].get<T>();
        }
        if (configJSON.contains("maxLimit"))
        {
            m_maxLimit = configJSON["maxLimit"].get<T>();
        }
    }

	~LimitedValue() = default;
 private:
    // Value itself
    std::atomic<T> m_value;
    // min limit
    std::atomic<T> m_minLimit;
    // max limit
    std::atomic<T> m_maxLimit;
};

#endif //LIMITEDVALUE_H
