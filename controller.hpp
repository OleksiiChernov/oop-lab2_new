#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

/*****************************************************************************/

#include <string>
#include <vector>
#include "lecture.hpp"

/*****************************************************************************/


class Controller
{

    /*-----------------------------------------------------------------*/

public:

    /*-----------------------------------------------------------------*/

    Controller();

    ~Controller();

    Controller(const Controller &) = delete;
    Controller & operator = (const Controller &) = delete;

    /*-----------------------------------------------------------------*/

    void createLecture(std::string const & _instructorName,
                       std::string const & _disciplineName,
                       std::string const & _mainTopicName);

    std::string const & getLectureInstructorName(std::string const & _mainTopicName) const;

    std::string const & getLectureDisciplineName(std::string const & _mainTopicName) const;

    /*-----------------------------------------------------------------*/

    typedef std::vector< std::string > FullTopicName;

    void addTopic(FullTopicName const & _parentTopic,
                  std::string const & _newTopicName);

    std::vector< std::string > getSubtopicNames(FullTopicName const & _parentTopic) const;

    /*-----------------------------------------------------------------*/

    void setTopicSlidesCount(FullTopicName const & _topic, int _slidesCount);

    int getTopicOwnSlidesCount(FullTopicName const & _topic) const;

    int getTopicTotalSlidesCount(FullTopicName const & _topic) const;

    /*-----------------------------------------------------------------*/

    std::string findLargestLecture() const;

    std::vector< std::string > detectEmptyLectures() const;

    std::vector< std::string > findLecturesByKeyword(std::string const & _keyword) const;

    int getLecturesCountForDiscipline(std::string const & _disciplineName) const;

    std::vector< std::string > getInstructorDisciplines(std::string const & _instructorName) const;

    /*-----------------------------------------------------------------*/

	void addToSubtopic(FullTopicName const & _parentName, std::string const & _newTopicName);

private:

    int resolveByName(std::string const & _name) const;

    int findByName(std::string const & _name) const;

    std::vector <Lecture *> m_lectures;

};


/*****************************************************************************/

#endif //  _CONTROLLER_HPP_#pragma once