#include "controller.hpp"
#include "messages.hpp"
#include <algorithm>
#include "lecture.hpp"
#include "topic.hpp"
#include <iostream>
#include <stdexcept>

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
	/*
    if (_instructorName == "")
        throw Messages::InstructorNameEmpty;
    if (_disciplineName == "")
        throw Messages::DisciplineNameEmpty;
    if (_mainTopicName == "")
        throw Messages::TopicNameEmpty; 
		*/
    if (findByName(_mainTopicName) != -1 )
		throw std::logic_error (Messages::MainTopicDuplication);
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
	if (_parentName.size() > 1)
		addToSubtopic(_parentName, _newTopicName);
	else
	{
		bool newTopicCreation = true;
		int max = m_lectures.size();
		for (int i = 0; i < max; i++)
		{
			if (m_lectures[i]->getTopicName() == _parentName[0])
			{

				m_lectures[i]->addNewTopic(_newTopicName);
				newTopicCreation = false;
			}
		}
		if (newTopicCreation)
			throw std::logic_error(Messages::TopicUnknown);
	}
}


std::vector< std::string > Controller::getSubtopicNames(FullTopicName const & _parentTopic) const
{
    bool notFindTopic = true;
    std::vector<std::string> namesOfSubtopics;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
		for (int k = 0; k < _parentTopic.size(); k++)
		{
			if (m_lectures[i]->getTopicName() == _parentTopic[k])
			{
				namesOfSubtopics = m_lectures[i]->getMainTopic().getTopicsNames(m_lectures[i]->getMainTopic(), _parentTopic);
				notFindTopic = false;
				break;
			}
		}
    }

    if (notFindTopic)
        throw std::logic_error(Messages::TopicUnknown);

    std::sort(namesOfSubtopics.begin(), namesOfSubtopics.end());

    return namesOfSubtopics;
}


void Controller::setTopicSlidesCount(FullTopicName const & _topic, int _slidesCount)
{
    bool notFindTopic = true;
    int max = m_lectures.size();
	if (_topic.size() > 1)
	{
	for (int i = 0; i < max; i++)
		if (m_lectures[i]->getTopicName() == _topic[0])
		{
			for (int j = 0; j < m_lectures[i]->getMainTopic().getTopics().size(); j++)
			{
				if (m_lectures[i]->getMainTopic().getTopics()[j]->getTopicName() == _topic[1])
				{
					m_lectures[i]->getMainTopic().getTopics()[j]->setNumberOfSlides(_slidesCount);
					notFindTopic = false;
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < max; i++)
			for (int k = 0; k < _topic.size(); k++)
			{
				if (m_lectures[i]->getTopicName() == _topic[k])
				{
					m_lectures[i]->getMainTopic().setNumberOfSlides(_slidesCount);
					notFindTopic = false;
					break;
				}
			}
	}

    if (notFindTopic)
        throw std::logic_error (Messages::TopicUnknown);
}


int Controller::getTopicOwnSlidesCount(FullTopicName const & _topic) const
{
    bool notFindTopic = true;
    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
		for (int k = 0; k < _topic.size(); k++)
    {
        if (m_lectures[i]->getTopicName() == _topic[k])
        {
            notFindTopic = false;
            return m_lectures[i]->getMainTopic().getOwnNumberOfSlides(_topic);
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
		for (int k = 0; k < _topic.size(); k++)
		{
			if (m_lectures[i]->getTopicName() == _topic[k])
			{
					m_nSlides += m_lectures[i]->getMainTopic().getTotalNumberOfSlides(_topic);
				notFindTopic = false;
				break;
			}
		}
	}

    if (notFindTopic)
        throw  std::logic_error(Messages::TopicUnknown);

    return m_nSlides;
}


std::string Controller::findLargestLecture() const
{
    if (m_lectures.size() == 0)
        return "";

	if (m_lectures.size() == 1)
		return m_lectures[0]->getTopicName();

    int m_nSlides1 = 0;
    int m_nSlides2 = 0;
    int iteration = 0;

    int max = m_lectures.size();
    for (int i = 0; i < max; i++)
    {
        m_nSlides2 = 0;
		m_nSlides2 += m_lectures[i]->getMainTopic().getNumberOfSlides();


        if (m_nSlides2 > m_nSlides1)
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

	if (m_lectures.size() == 1)
	{
		if (m_lectures[0]->getMainTopic().getNumberOfSlides() == 0)
			m_lecturesNames.push_back(m_lectures[0]->getTopicName());
		return m_lecturesNames;
	}

    for (int i = 0; i < max; i++)
    {
        if (m_lectures[i]->getMainTopic().getNumberOfSlides() == 0)
            m_lecturesNames.push_back(m_lectures[i]->getTopicName());
    }

    std::sort(m_lecturesNames.begin(), m_lecturesNames.end());
    return m_lecturesNames;
}


std::vector<std::string> Controller::findLecturesByKeyword(std::string const & _keyword) const
{
    if (_keyword == "")
        throw std::logic_error (Messages::KeywordEmpty);

    std::vector <std::string> m_lecturesName;
    int max = m_lectures.size();

    for (int i = 0; i < max; i++)
    {
		if (m_lectures[i]->getMainTopic().getTopics().size() == 0)
		{
			std::string _str = m_lectures[i]->getTopicName();
			size_t pos = _str.find(_keyword);
			if (pos != std::string::npos)
				m_lecturesName.push_back(m_lectures[i]->getTopicName());
		}
		else if (m_lectures[i]->getMainTopic().getTopics().size() != 0)
		{
			int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
			for (int j = 0; j < maxJ; j++)
			{
				int maxK = m_lectures[i]->getMainTopic().getTopics()[j]->getTopics().size();
				for (int k = 0; k < maxK; k++)
				{
					std::string _str = m_lectures[i]->getMainTopic().getTopics()[j]->getTopics()[k]->getTopicName();
					size_t pos = _str.find(_keyword);
					if (pos != std::string::npos)
						m_lecturesName.push_back(m_lectures[i]->getTopicName());
				}
			}
		}
		else 
			{
				int maxJ = m_lectures[i]->getMainTopic().getTopics().size();
				for (int j = 0; j < maxJ; j++)
				{
					std::string _str = m_lectures[i]->getMainTopic().getTopics()[j]->getTopicName();
					size_t pos = _str.find(_keyword);
					if (pos != std::string::npos)
						m_lecturesName.push_back(m_lectures[i]->getTopicName());
				}
				
			}
    }

    std::sort(m_lecturesName.begin(), m_lecturesName.end());
    return m_lecturesName;
}


int Controller::getLecturesCountForDiscipline(std::string const & _disciplineName) const
{
    if (_disciplineName == "")
        throw std::logic_error (Messages::DisciplineNameEmpty);

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
        throw std::logic_error(Messages::InstructorNameEmpty);
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

void Controller::addToSubtopic(FullTopicName const & _parentName, std::string const & _newTopicName)
{
	int max = m_lectures.size();
	for (int i = 0; i < max; i++)
	{
		if (m_lectures[i]->getTopicName() == _parentName[0])
		{
			for (int j = 0; j < m_lectures[i]->getMainTopic().getTopics().size(); j++)
			{
				if (m_lectures[i]->getMainTopic().getTopics()[j]->getTopicName() == _parentName[1])
					m_lectures[i]->getMainTopic().getTopics()[j]->addTopic(_newTopicName);
			}
		}
	}
}


int Controller::resolveByName(std::string const & _name) const
{
    int index = findByName(_name);
    if (index == -1)
        throw std::logic_error (Messages::MainTopicUnknown);
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