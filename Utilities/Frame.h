#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <string>

class Frame
{
public:
	Frame();
    Frame(std::uint32_t width,
          std::uint32_t height,
          std::uint8_t channels,
          std::uint8_t* data,
          std::int64_t captureTime = -1,
          std::int64_t index = -1,
          std::string providerName = "");
	Frame(const Frame& other);
	Frame(Frame&& other) noexcept;
	// timestamp of captured frame
	std::int64_t captureTime{ -1 };
	//image data
	std::shared_ptr<std::uint8_t> data;
    // width
    std::uint32_t width;
    // height
    std::uint32_t height;
    // number of channels
    std::uint8_t channels;
	// frame index in video / timestamp in camera / index in directory
    std::int64_t index{ -1 };
    // provider name
    std::string providerName;
    // check if the object is pointing to something / is it empty?
    [[nodiscard]] bool Empty() const;
	//copy to destination
	void CopyTo(Frame& destination) const;
	//clear resources
	void Clear();
	~Frame();
	Frame& operator=(const Frame& other);
	Frame& operator=(Frame&& other) noexcept;
};

#endif //FRAME_H