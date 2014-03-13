#pragma once

#include "Image.h"
#include <thread>
#include <deque>

namespace dfgenerator
{

typedef Image<uint32_t,1> dfImage;

class DistanceField
{
public:
    DistanceField(const std::string &fileName, const size_t &maximumMeasurement);

    const dfImage getDfImage(const int &distance, const int &cores) const;
    const dfImage &getOriginalImage() const;

private:
    dfImage m_originalImage;
    Resolution<size_t> m_targetResolution;
};

DistanceField::DistanceField(const std::string &fileName, const size_t &maximumMeasurement) :
    m_originalImage(fileName), m_targetResolution{0,0}
{
    size_t w{size_t(maximumMeasurement * m_originalImage.getResolution().getAspectRatio())}, h{size_t(maximumMeasurement * (1.f / m_originalImage.getResolution().getAspectRatio()))};

    m_targetResolution = {w,h};
}

bool isIn(const Pixel<uint32_t,1> &pixel)
{ return pixel.m_channels[0] & 0xff000000; }

void getDistanceToNearestOpposite(const dfImage *source, Image<int, 1> *destination, const size_t &index, const int &spread)
{
    int distance{0};
    bool thisPixelOn{isIn(source->getPixels()[index])};
    while (distance++ < spread)
    {
        // TL -> TR
        for (size_t i = index - (distance * source->getResolution().w()) - distance; i < index - (distance * source->getResolution().w()) + distance; ++i)
            if (i > 0 && i < source->getResolution().getProduct() && (thisPixelOn != isIn(source->getPixels()[i])))
            {
                destination->getNonConstPixels()[index].m_channels[0] = thisPixelOn ? distance : -distance;
                return;
            }

        // BL -> BR
        for (size_t i = index + (distance * source->getResolution().w()) - distance; i < index + (distance * source->getResolution().w()) + distance; i += 4)
            if (i > 0 && i < source->getResolution().getProduct() && (thisPixelOn != isIn(source->getPixels()[i])))
            {
                destination->getNonConstPixels()[index].m_channels[0] = thisPixelOn ? distance : -distance;
                return;
            }

        // TL -> BL
        for (size_t i = index - (distance * source->getResolution().w()) - distance; i < index + (distance * source->getResolution().w()) - distance;
             i += source->getResolution().w())
            if (i > 0 && i < source->getResolution().getProduct() && (thisPixelOn != isIn(source->getPixels()[i])))
            {
                destination->getNonConstPixels()[index].m_channels[0] = thisPixelOn ? distance : -distance;
                return;
            }

        // TR -> BR
        for (size_t i = index - (distance * source->getResolution().w()) + distance; i < index + (distance * source->getResolution().w()) + distance;
             i += source->getResolution().w())
            if (i > 0 && i < source->getResolution().getProduct() && (thisPixelOn != isIn(source->getPixels()[i])))
            {
                destination->getNonConstPixels()[index].m_channels[0] = thisPixelOn ? distance : -distance;
                return;
            }
    }
    destination->getNonConstPixels()[index].m_channels[0] = spread;
}

void threadJob(const dfImage *source, Image<int, 1> *destination, const size_t &start, const size_t &stop, const int &spread)
{
    for (size_t i{start}; i < stop; ++i)
        getDistanceToNearestOpposite(source, destination, i, spread);
}

const dfImage DistanceField::getDfImage(const int &distance, const int &cores) const
{
    Image<int, 1> newDistanceImage{m_originalImage.getResolution()};
    int maxPos{0}, maxNeg{0};

    size_t numThreads{size_t(cores)};
    size_t jobsPerThread {m_originalImage.getResolution().getProduct() / numThreads};
    std::deque<std::thread> threadPool;
    std::deque<std::thread> running;

    std::cout << "Creating distance field... "; std::cout.flush();
    for (size_t i{0}; i < numThreads; ++i)
    {
        running.push_back(std::thread(threadJob, &m_originalImage, &newDistanceImage, i * size_t(jobsPerThread), (i + 1) * size_t(jobsPerThread), distance));
    }

    for (std::thread &t : running)
    {
        t.join();
    }
    std::cout << "done!" << std::endl;

    std::cout << "Remapping distance to alpha value... "; std::cout.flush();
    for (size_t i{0}; i< newDistanceImage.getPixels().size(); ++i)
    {
        if (newDistanceImage.getPixels()[i].m_channels[0] > maxPos)
            maxPos = newDistanceImage.getPixels()[i].m_channels[0];
        else if (newDistanceImage.getPixels()[i].m_channels[0] < maxNeg)
            maxNeg = newDistanceImage.getPixels()[i].m_channels[0];
    }

    dfImage newImg(m_originalImage.getResolution());
    float ratio;
    char alpha; uint32_t col;
    for (size_t i{0}; i < newImg.getResolution().getProduct(); ++i)
    {
        if (newDistanceImage.getPixels()[i].m_channels[0] < 0)
        {
            ratio = (float(newDistanceImage.getPixels()[i].m_channels[0]) / float(maxNeg));
            alpha = 0x80 - 0x80 * ratio;
        }
        else
        {
            ratio = (float(newDistanceImage.getPixels()[i].m_channels[0]) / float(maxPos));
            alpha = 0x80 + 0x80 * ratio;
        }
        if (alpha)
            ;
        col = ((alpha << 24) & 0xff000000) | 0xff0000 | 0xff00 | 0xff;
        newImg.getNonConstPixels()[i] = {{col}};
    }
    std::cout << "done!" << std::endl;

    return newImg.scaleLinear(m_targetResolution);
}

const dfImage &DistanceField::getOriginalImage() const
{ return m_originalImage; }

}
