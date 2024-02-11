#ifndef STATEVALUE_H
#define STATEVALUE_H

#include "nlohmann/json.hpp"
#include <atomic>

//class that keeps the track if value is changed [thread-safe]
//Indicates if values was changed during the last assignment
template <class T>
class StateValue
{
 public:
    StateValue(T value)
        : m_changed(false)
    {
        m_value = value;
    };
    void SetValue(T value)
    {
        if (value != m_value)
        {
            m_value = value;
            m_changed = true;
        }
        else
        {
            m_value = value;
            m_changed = false;
        }
    }
    T GetValue()
    {
        return m_value;
    }
    // returns true if value was change in the last assignment
    bool IsChanged()
    {
        return m_changed;
    }
    std::string GetConfig()
    {
        nlohmann::json config;
        config["value"] = m_value.load();
        return config.dump();
    }

    void SetConfig(std::string config)
    {
        nlohmann::json configJSON = nlohmann::json ::parse(config);
        if (configJSON.contains("value"))
        {
            m_value = configJSON["value"].get<T>();
        }
    }
    StateValue& operator=(const T other)
    {
        if (other != m_value)
        {
            m_value = other;
            m_changed = true;
        }
        else
        {
            //m_value = other;
            if (m_changed)
            {
                m_changed = false;
            }
        }
        return *this;
    }
    ~StateValue() = default;
 private:
    // Value itself
    std::atomic<T> m_value;
    // Value changed state
    std::atomic<bool> m_changed;
};

#endif //LIMITEDVALUE_H
