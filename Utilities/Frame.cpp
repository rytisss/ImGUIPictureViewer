#include "Frame.h"
#include <iostream>

Frame::Frame()
	: index(-1)
	, captureTime(-1)
    , width(0)
    , height(0)
    , channels(0)
    , data(nullptr)
{
}

Frame::Frame(std::uint32_t width,
             std::uint32_t height,
             std::uint8_t channels,
             std::uint8_t* data,
             std::int64_t captureTime,
             std::int64_t index,
             std::string providerName)
{
    this->width = width;
    this->height = height;
    this->channels = channels;
    std::shared_ptr<std::uint8_t> sharedPtr(data, [](std::uint8_t* p){ delete[] p; });
    this->data = sharedPtr;
    this->captureTime = captureTime;
    this->index = index;
    this->providerName = providerName;
}

Frame::Frame(const Frame& other)
{
	//std::cout << "Copy constructor call in Frame" << std::endl;
	this->data = other.data;
    this->providerName = other.providerName;
	this->index = other.index;
	this->captureTime = other.captureTime;
    this->width = other.width;
    this->height = other.height;
    this->channels = other.channels;
}

Frame::Frame(Frame&& other) noexcept
{
    //std::cout << "Copy constructor call in Frame" << std::endl;
    this->data = other.data;
    this->providerName = other.providerName;
    this->index = other.index;
    this->captureTime = other.captureTime;
    this->width = other.width;
    this->height = other.height;
    this->channels = other.channels;
	other.Clear();
}

void Frame::CopyTo(Frame& destination) const
{
    destination.data = this->data;
    destination.providerName = this->providerName;
    destination.index = this->index;
    destination.captureTime = this->captureTime;
    destination.width = this->width;
    destination.height = this->height;
    destination.channels = this->channels;
}

void Frame::Clear()
{
    this->data = nullptr;
    this->providerName = "";
    this->index = -1;
    this->captureTime = -1;
    this->width = 0;
    this->height = 0;
    this->channels = 0;
}

Frame::~Frame()
{
	Clear();
}

Frame& Frame::operator=(const Frame& other)
{
	if (&other != this)
	{
        this->data = other.data;
        this->providerName = other.providerName;
        this->index = other.index;
        this->captureTime = other.captureTime;
        this->width = other.width;
        this->height = other.height;
        this->channels = other.channels;
	}
	return *this;
}

Frame& Frame::operator=(Frame&& other) noexcept
{
    this->data = other.data;
    this->providerName = other.providerName;
    this->index = other.index;
    this->captureTime = other.captureTime;
    this->width = other.width;
    this->height = other.height;
    this->channels = other.channels;
    other.Clear();
	return *this;
}

bool Frame::Empty() const
{
    return data == nullptr;
}
