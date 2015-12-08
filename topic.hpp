#ifndef _TOPIC_HPP_
#define _TOPIC_HPP_
#include <string>
#include <vector>

class Topic
{

public:

    Topic(std::string const & _topicName, int _nSlides = 0);

    Topic(const Topic &) = delete;
    Topic & operator = (const Topic &) = delete;

    ~Topic();

    std::string const & getTopicName() const { return m_topicName; }
    int const & getNumberOfSlides() const { return m_numberOfSlides; }
    std::vector <Topic *> const & getTopics() const { return m_topics; }

    void addTopic(std::string  _topicName, int _nSlides = 0);
	bool hasTopic(std::string _topicName);

    int getSubtopicsSlidesCount();
    void setNumberOfSlides(int _nSlides);
	std::vector<std::string> getTopicsNames(Topic &_tpc);

private:

    std::string m_topicName;
    int m_numberOfSlides;
    std::vector <Topic *> m_topics;


};


#endif //  _TOPIC_HPP_#pragma once