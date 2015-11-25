#include "lecture.hpp"
#include "messages.hpp"

Lecture::Lecture(std::string const & _lectureTitle, std::string const & _teachersName,
                 std::string const & _topicName, int _numberOfSlides)
        : m_mainTopic(_topicName, _numberOfSlides)
{
    if (_lectureTitle == "")
        throw Messages::DisciplineNameEmpty;
    else
        m_lectureTitle = _lectureTitle;

    if (_teachersName == "")
        throw Messages::InstructorNameEmpty;
    else
        m_teachersName = _teachersName;
}


Lecture::~Lecture()
{
    m_mainTopic.~Topic();
}

void Lecture::addNewTopic(std::string _topicName)
{
    for (int i = 0; i < m_mainTopic.getTopics().size(); i++)
    {
        if (&m_mainTopic.getTopics()[i]->getTopicName() == &_topicName)
            throw Messages::MainTopicDuplication;
    }
    m_mainTopic.addTopic(_topicName);
}