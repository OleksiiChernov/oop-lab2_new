#include "controller.hpp"
#include "messages.hpp"
#include <algorithm>
#include "lecture.hpp"
#include "topic.hpp"
#include <iostream>

Controller::Controller() {}


Controller::~Controller()
{
    for (Lecture * _lec : m_lectures)
        delete _lec;
}

void Controller::createLecture(
        std::string const & _instructorName,
        std::string const & _disciplineName,
        std::string const & _mainTopicName
)
{
    /* проверял инвариант тут, не работает.
    if (_instructorName == "")
        throw Messages::InstructorNameEmpty;
    if (_disciplineName == "")
        throw Messages::DisciplineNameEmpty;
    if (_mainTopicName == "")
        throw Messages::TopicNameEmpty; 
        */
    if (findByName(_mainTopicName) != -1 )
        throw Messages::MainTopicDuplication;
    Lecture *_newLecture = new Lecture(_instructorName, _disciplineName, _mainTopicName);
    //m_lectures.push_back(new Lecture (_disciplineName, _instructorName, _mainTopicName));
    m_lectures.push_back(_newLecture);
}


std::string const & Controller::getLectureInstructorName(std::string const & _mainTopicName) const
{
    int i = resolveByName(_mainTopicName);
    return m_lectures[i]->getTeachersName();

}


std::string const & Controller::getLectureDisciplineName(std::string const & _mainTopicName) const
{
    int i = resolveByName(_mainTopicName);
    return m_lectures[i]->getLectureTitle();
}


void Controller::addTopic(FullTopicName const & _parentName, std::string const & _newTopicName)
{
    bool newTopicCreation = true;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getTopicName() == _parentName[i])
        {
            m_lectures[i]->addNewTopic(_newTopicName);
            newTopicCreation = false;
        }
    }
    if (newTopicCreation)
        throw Messages::TopicUnknown;
}


std::vector< std::string > Controller::getSubtopicNames(FullTopicName const & _parentTopic) const
{
    bool notFindTopic = true;
    std::vector <std::string> namesOfSubtopics;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getTopicName() == _parentTopic[i])
        {
            int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
            for (int j = 0; j < maxJ; j++)
            {
                namesOfSubtopics.push_back(m_lectures[i]->getMainTopic().getTopics()[j]->getTopicName());
            }
            notFindTopic = false;
            break;
        }
    }

    if (notFindTopic)
        throw Messages::TopicUnknown;

    std::sort(namesOfSubtopics.begin(), namesOfSubtopics.end());

    return namesOfSubtopics;
}


void Controller::setTopicSlidesCount(FullTopicName const & _topic, int _slidesCount)
{
    bool notFindTopic = true;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getTopicName() == _topic[i])
        {
            m_lectures[i]->getMainTopic().setNumberOfSlides(_slidesCount);
            notFindTopic = false;
            break;
        }
    }

    if (notFindTopic)
        throw Messages::TopicUnknown;
}


int Controller::getTopicOwnSlidesCount(FullTopicName const & _topic) const
{
    bool notFindTopic = true;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getTopicName() == _topic[i])
        {
            notFindTopic = false;
            return m_lectures[i]->getMainTopic().getNumberOfSlides();
        }
    }
    return -1;
    //if (notFindTopic)
    //    throw Messages::TopicUnknown;
   // return -1;
}


int Controller::getTopicTotalSlidesCount(FullTopicName const & _topic) const
{
    bool notFindTopic = true;
    int m_nSlides = 0;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getTopicName() == _topic[i])
        {
            int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
            for (int j = 0; j < maxJ; j++)
            {
                m_nSlides += m_lectures[i]->getMainTopic().getTopics()[j]->getNumberOfSlides();
            }
            notFindTopic = false;
            break;
        }
    }

    if (notFindTopic)
        throw Messages::TopicUnknown;

    return m_nSlides;
}


std::string Controller::findLargestLecture() const
{
    if (m_lectures.size() == 0)
        return "";

    int m_nSlides1 = 0;
    int m_nSlides2 = 0;
    int iteration;

    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        m_nSlides2 = 0;
        int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
        for (int j = 0; j < maxJ; j++)
        {
            m_nSlides2 += m_lectures[i]->getMainTopic().getTopics()[i]->getNumberOfSlides();
        }

        if (m_nSlides1 < m_nSlides2)
        {
            m_nSlides1 = m_nSlides2;
            iteration = i;
        }
    }

    return m_lectures[iteration]->getTopicName();
}



std::vector<std::string> Controller::detectEmptyLectures() const
{
    std::vector<std::string> m_lecturesNames;
    int max = m_lectures.size();

    for (int i = 0; i < max; i++)
    {
        int m_nSlides = 0;
        int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
        for (int j = 0; j < maxJ; j++)
        {
            m_nSlides += m_lectures[i]->getMainTopic().getTopics()[j]->getNumberOfSlides();
        }
        if (m_nSlides == 0)
            m_lecturesNames.push_back(m_lectures[i]->getLectureTitle());
    }

    std::sort(m_lecturesNames.begin(), m_lecturesNames.end());
    return m_lecturesNames;
}


std::vector<std::string> Controller::findLecturesByKeyword(std::string const & _keyword) const
{
    if (_keyword == "")
        throw Messages::KeywordEmpty;

    std::vector <std::string> m_lecturesName;
    int max = m_lectures.size();

    for (int i = 0; i < max; i++)
    {
        int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
        for (int j = 0; j < maxJ; j++)
        {
            std::string _str = m_lectures[i]->getMainTopic().getTopics()[j]->getTopicName();
            size_t pos = _str.find(_keyword);
            if (pos != std::string::npos)
                m_lecturesName.push_back(m_lectures[i]->getLectureTitle());
        }
    }

    std::sort(m_lecturesName.begin(), m_lecturesName.end());
    return m_lecturesName;
}


int Controller::getLecturesCountForDiscipline(std::string const & _disciplineName) const
{
    if (_disciplineName == "")
        throw Messages::DisciplineNameEmpty;

    int nLections = 0;

    int max = m_lectures.size();

    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getLectureTitle() == _disciplineName)
            nLections++;
    }

    return nLections;
}


std::vector<std::string> Controller::getInstructorDisciplines(std::string const & _instructorName) const
{
    if (_instructorName == "")
        throw Messages::InstructorNameEmpty;
    std::vector <std::string> mDisciplines;

    int max = m_lectures.size();

    for (int i = 0; i < max; i++)
    {
        std::string disc;
        int n = 0;
        if (m_lectures[i]->getTeachersName() == _instructorName)
        {
            disc = m_lectures[i]->getLectureTitle();
            for (int j = 0; j < mDisciplines.size(); j++)
            {
                if (mDisciplines[j] == disc)
                    n++;
            }
            if (n == 0)
                mDisciplines.push_back(disc);
        }

    }

    std::sort(mDisciplines.begin(), mDisciplines.end());
    return mDisciplines;
}


int Controller::resolveByName(std::string const & _name) const
{
    int index = findByName(_name);
    if (index == -1)
        throw Messages::MainTopicUnknown;
    return index;
}


int Controller::findByName(std::string const & _name) const
{
    int Max = m_lectures.size();
    for (int i = 0; i < Max; i++)
    {
        if (m_lectures[i]->getTopicName() == _name)
            return i;
    }
    return -1;
}