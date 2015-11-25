#include "topic.hpp"
#include "messages.hpp"
#include <stdexcept>


Topic::Topic(std::string const & _topicName, int _nSlides)
        : m_topicName(_topicName), m_numberOfSlides(_nSlides)
{
    if (_topicName == "")
        throw Messages::TopicNameEmpty;
    if (_nSlides < 0)
        Messages::NegativeSlidesCount;
}


Topic::~Topic()
{
    for (Topic * _topics : m_topics)
        delete _topics;
}


void Topic::addTopic(std::string _topicName, int _nSlides)
{
    Topic * _nTopic = new Topic(_topicName, _nSlides);
    m_topics.push_back(_nTopic);
}


int Topic::getSubtopicsSlidesCount()
{
    int nSumm = 0;
    for (Topic * _top : m_topics)
    {
        nSumm += _top->getNumberOfSlides();
        nSumm += _top->getSubtopicsSlidesCount();
    }
    return nSumm;
}

void Topic::setNumberOfSlides(int _nSlides)
{
    if (_nSlides >= 0)
        m_numberOfSlides = _nSlides;
    else
        Messages::NegativeSlidesCount;
}