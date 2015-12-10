#include "topic.hpp"
#include "messages.hpp"
#include <stdexcept>
#include <algorithm>

Topic::Topic(std::string const & _topicName, int _nSlides)
        : m_topicName(_topicName), m_numberOfSlides(_nSlides)
{
    if (_topicName == "")
		throw std::logic_error (Messages::TopicNameEmpty);
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
	if (!hasTopic(_topicName))
	{
		Topic * _nTopic = new Topic(_topicName, _nSlides);
		m_topics.push_back(_nTopic);
	}
}

bool Topic::hasTopic(std::string _topicName)
{
	for (Topic * _top : m_topics)
	{
		if (_top->getTopicName() == _topicName)
			throw std::logic_error(Messages::SubtopicExists);
	}

	return false;

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
        throw std::logic_error(Messages::NegativeSlidesCount);
}

std::vector<std::string> Topic::getTopicsNames(Topic &_tpc, FullTopicName const & _parentTopic)
{
	std::vector <std::string> names;
	if (_parentTopic.size() > 1)
	{
		if (_tpc.getTopicName() == _parentTopic[0])
		{
			for (int j = 0; j < _tpc.getTopics().size(); j++)
			{
				if (_tpc.getTopics()[j]->getTopicName() == _parentTopic[1])
					for (int i = 0; i < _tpc.getTopics()[j]->getTopics().size(); i++)
						names.push_back(_tpc.getTopics()[j]->getTopics()[i]->getTopicName());
			}
		}
	}
	else
	{
		int max = _tpc.getTopics().size();

		for (int i = 0; i < max; i++)
		{
			names.push_back(_tpc.getTopics()[i]->getTopicName());
			//if (_tpc.getTopics()[i]->getTopics().size() != -1)
			//{
			//	for (int j = 0; j < _tpc.getTopics()[i]->getTopics().size(); j++)
			//		names.push_back(_tpc.getTopics()[i]->getTopics()[j]->getTopicName());
			//	}
		}
	}
	return names;
}

int Topic::getTotalNumberOfSlides(Topic & _tpc, FullTopicName const & _parentTopic)
{
	int numbers = 0;
	if (_parentTopic.size() > 1)
	{
		if (_tpc.getTopicName() == _parentTopic[0])
		{
			for (int j = 0; j < _tpc.getTopics().size(); j++)
			{
				if (_tpc.getTopics()[j]->getTopicName() == _parentTopic[1])
					for (int i = 0; i < _tpc.getTopics()[j]->getTopics().size(); i++)
						numbers += _tpc.getTopics()[j]->getTopics()[i]->getNumberOfSlides();
			}
		}
	}
	else
	{
		int max = _tpc.getTopics().size();
		numbers = _tpc.getNumberOfSlides();
		for (int i = 0; i < max; i++)
		{
			numbers += _tpc.getTopics()[i]->getNumberOfSlides();
		}
	}
	
	return numbers;
}
