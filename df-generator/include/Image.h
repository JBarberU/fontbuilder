#pragma once

#include <vector>
#include <string>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"


namespace dfgenerator
{

enum ColorMode {
    CM_RGBA,
    CM_RGB,
    CM_ARGB
};

template <class T, size_t N>
struct Pixel
{
    Pixel()
    {
        for (size_t i = 0; i < N; ++i)
            m_channels[i] = {};
    }

    Pixel(const T (&channels)[N])
    {
        setChannels(channels);
    }

    const Pixel &operator=(const Pixel<T, N> &pixel)
    {
        setChannels(pixel.m_channels);
        return *this;
    }

    void setChannels(const T (&channels)[N])
    {
        for (size_t i = 0; i < N; ++i)
            m_channels[i] = channels[i];
    }

    T m_channels[N];
};

template <class T>
class Resolution
{
public:
    Resolution(const T &width, const T &height) :
        m_width(width), m_height(height), m_product(width * height)
    {}

    const T &getProduct() const
    { return m_product; }

    const T &w() const
    { return m_width; }

    const T &h() const
    { return m_height; }

    float getAspectRatio() const
    { return m_height == 0 ? 0.f : float(m_width) / float(m_height); }

    bool operator<(const Resolution<T> &resolution) const
    { return m_product < resolution.m_product; }

    bool operator>(const Resolution<T> &resolution) const
    { return m_product > resolution.m_product; }

    bool operator==(const Resolution<T> &resolution) const
    { return m_width == resolution.m_width && m_height == resolution.m_height; }

private:
    T m_width, m_height, m_product;
};

template <class T, int N>
class Image
{
public:

    Image(const std::string &fileName) : m_resolution(0,0)
    {
        std::cout << "Loading image... ";
        std::cout.flush();
        int w,h,c;
        unsigned char *img_data = stbi_load(fileName.c_str(), &w, &h, &c, STBI_rgb_alpha);
        if (img_data)
        {
            m_resolution = {size_t(w),size_t(h)};
            m_pixels = std::vector<Pixel<T,N>>(m_resolution.getProduct());
            memcpy((void*)m_pixels.data(), img_data, w * h * c);
        }
        else
        {
            std::cerr << "Unable to open image: " << fileName << std::endl;
        }
        std::cout << "done!" << std::endl;
    }

    Image(const Resolution<size_t> &resolution) : m_resolution(resolution)
    {
        m_pixels = std::vector<Pixel<T,N>>(m_resolution.getProduct(), Pixel<T,N>());
    }

    Image(const std::vector<Pixel<T,N>> &pixels, const Resolution<size_t> &resolution) :
        m_pixels(pixels), m_resolution(resolution)
    {}

    Image() : m_resolution{0,0}
    {}

    ~Image()
    {}

    const Image<T,N> &operator=(const Image<T,N> &img)
    {
        m_resolution = img.getResolution();
        m_pixels = img.m_pixels;
        return *this;
    }

    const std::vector<Pixel<T,N>> &getPixels() const
    { return m_pixels; }

    std::vector<Pixel<T,N>> &getNonConstPixels()
    { return m_pixels; }

    void setPixels(const std::vector<Pixel<T,N>> &pixels, const Resolution<size_t> &resolution)
    {
        m_pixels = pixels;
        m_resolution = resolution;
    }

    const Resolution<size_t> &getResolution() const
    { return m_resolution; }

    bool saveImage(const std::string &fileName) const
    {
        int channels{4};
        int status = stbi_write_png(fileName.c_str(), m_resolution.w(), m_resolution.h(), channels, (unsigned char *)m_pixels.data(), 0);
        return status;
    }

    void filterAnd(const Pixel<uint32_t,1> &filter)
    {
        for (Pixel<uint32_t,1> &px : m_pixels)
            px.m_channels[0] &= filter.m_channels[0];
    }

    Image<T,N> scaleLinear(const Resolution<size_t> &targetResolution) const
    {
        Image<T,N> newImage{targetResolution};
        assert(targetResolution.getAspectRatio() == m_resolution.getAspectRatio());
        bool scaleUp{m_resolution < targetResolution};
        size_t oppPpD{m_resolution.w() / targetResolution.w()}; // Original pixels per pixel per dimension
        uint32_t a,r,g,b;

        std::cout << "Scaling image... "; std::cout.flush();
        for (size_t y{0}; y < targetResolution.w(); ++y)
        {
            for (size_t x{0}; x < targetResolution.h(); ++x)
            {
                if (scaleUp)
                {
                    assert(0); // This might need to be implemented :)
                }
                else
                {
                    ;//Scale down
                    a = 0;
                    b = 0;
                    g = 0;
                    r = 0;
                    for (size_t ix{0}; ix < oppPpD; ++ix)
                    {
                        for (size_t iy{0}; iy < oppPpD; ++iy)
                        {
                            a += (m_pixels[y * oppPpD + x * oppPpD * m_resolution.w() + ix + iy * m_resolution.w()].m_channels[0] >> 24)  & 0xff;
                            b += (m_pixels[y * oppPpD + x * oppPpD * m_resolution.w() + ix + iy * m_resolution.w()].m_channels[0] >> 16)  & 0xff;
                            g += (m_pixels[y * oppPpD + x * oppPpD * m_resolution.w() + ix + iy * m_resolution.w()].m_channels[0] >> 8 )  & 0xff;
                            r += (m_pixels[y * oppPpD + x * oppPpD * m_resolution.w() + ix + iy * m_resolution.w()].m_channels[0])        & 0xff;
                        }
                    }
                    a = ((a / (oppPpD*oppPpD)) << 24) & 0xff000000;
                    b = ((b / (oppPpD*oppPpD)) << 16) & 0x00ff0000;
                    g = ((g / (oppPpD*oppPpD)) << 8)  & 0x0000ff00;
                    r = ((r / (oppPpD*oppPpD)))       & 0x000000ff;

                    newImage.getNonConstPixels()[y + x * targetResolution.w()].m_channels[0] = a | b | g | r;
                }
            }
        }
        std::cout << "done!" << std::endl;
        return newImage;
    }

private:
    std::vector<Pixel<T,N>> m_pixels;
    Resolution<size_t> m_resolution;
    ColorMode m_colorMode;
};

}
