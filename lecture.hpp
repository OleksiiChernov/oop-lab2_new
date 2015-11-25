#ifndef _LECTURE_HPP_
#define _LECTURE_HPP_
#include <string>
#include "topic.hpp"

/*****************************************************************************/

class Topic;

/*****************************************************************************/


class Lecture
{

public:

    Lecture(std::string const & _teachersName, std::string const & _lectureTitle,
            std::string const & _topicName, int _numberOfSlides = 0);

    Lecture(const Lecture &) = delete;
    Lecture & operator = (const Lecture &) = delete;

    ~Lecture();

    std::string const & getLectureTitle() const { return m_lectureTitle; }
    std::string const & getTeachersName() const { return m_teachersName; }
    std::string const & getTopicName()    const { return m_mainTopic.getTopicName(); }
    Topic & getMainTopic() { return m_mainTopic; }

    void addNewTopic(std::string _topicName);


private:

    std::string m_lectureTitle;
    std::string m_teachersName;
    Topic m_mainTopic;
};


#endif //  _LECTURE_HPP_#pragma once