#ifndef COLOR_H
#define COLOR_H

#include "nlohmann/json.hpp"
#include <iostream>

struct Color
{
    Color()
        : R(255)
        , G(0)
        , B(0)
        , A(255)
    {
    }
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    Color(const Color& other)
    {
        R = other.R;
        G = other.G;
        B = other.B;
        A = other.A;
    }
    Color& operator=(const Color& other)
    {
        R = other.R;
        G = other.G;
        B = other.B;
        A = other.A;
        return *this;
    }
    std::string GetConfig()
    {
        nlohmann::json config;
        config["R"] = R;
        config["G"] = G;
        config["B"] = B;
        config["A"] = A;
        return config.dump();
    }
    void SetConfig(std::string config)
    {
        nlohmann::json configJSON;
        try
        {
            configJSON = nlohmann::json::parse(config);
        }
        catch (...)
        {
            std::cout << "Failed to parse configuration color configuration" << std::endl;
        }
        if (configJSON.empty())
        {
            return;
        }
        try
        {
            if (configJSON.contains("R"))
            {
                R = configJSON["R"].get<unsigned char>();
            }
            if (configJSON.contains("G"))
            {
                G = configJSON["G"].get<unsigned char>();
            }
            if (configJSON.contains("B"))
            {
                B = configJSON["B"].get<unsigned char>();
            }
            if (configJSON.contains("A"))
            {
                A = configJSON["A"].get<unsigned char>();
            }
        }
        catch (...)
        {
            std::cout << "Failed to color values" << std::endl;
        }
    }
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};

#endif //COLOR_H
