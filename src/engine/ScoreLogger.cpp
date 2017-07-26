#include "ScoreLogger.h"

#include <string>

using namespace Score;

ScoreLogger* ScoreLogger::_instance = 0;
std::multimap<const char*, float> ScoreLogger::_acceptance_task_score;
std::multimap<const char*, float> ScoreLogger::_conflict_score;
std::multimap<const char*, float> ScoreLogger::m_handoffTaskScore;

ScoreLogger::ScoreLogger()
{
}

ScoreLogger::~ScoreLogger()
{
}

ScoreLogger* ScoreLogger::instance()
{
	if (!_instance)
	{
		_instance = new ScoreLogger();
	}

	return _instance;
}

void ScoreLogger::destroy()
{
	if (_instance)
	{
		delete _instance;
		_instance = 0;
	}
}

void ScoreLogger::LogAcceptanceTaskScore(
	const char* trial_name, 
	CorrectStatus status)
{
	float score = 0.0;

	switch (status)
	{
	case CORRECT_STATUS_CORRECT:
		score = ACCEPTANCE_TASK_SCORE_CORRECT;
		break;
	case CORRECT_STATUS_INCORRECT:
		score = ACCEPTANCE_TASK_SCORE_INCORRECT;
		break;
	case CORRECT_STATUS_UNKNOWN:
	default:
		score = 0.0;
		break;
	}

	_acceptance_task_score.insert(std::pair<const char*, float>(trial_name, score));
}

void
ScoreLogger::LogHandoffTaskScore(const char* a_trialName,
									CorrectStatus a_correct)
{
	float score = 0.0;

	switch (a_correct)
	{
	case CORRECT_STATUS_CORRECT:
		score = HANDOFF_TASK_SCORE_CORRECT;
		break;
	case CORRECT_STATUS_INCORRECT:
		score = HANDOFF_TASK_SCORE_INCORRECT;
		break;
	case CORRECT_STATUS_UNKNOWN:
	default:
		score = 0.0;
		break;
	}

	m_handoffTaskScore.insert(std::pair<const char*, float>(a_trialName, score));
}

void ScoreLogger::LogConflictResolvedScore(const char* trial_name, 
											int acceptance_time, 
											int conflict_time, 
											int resolved_time)
{
	float score = 0.0;
	int conflict_duration = conflict_time - acceptance_time;

	float percent = (float) (resolved_time - acceptance_time) / (float) conflict_duration;

	if ((percent >= 0) && (percent <= 0.25))
	{
		score = CONFLICT_SCORE_RESOLVED_1ST_QTR;
	}
	else if ((percent > 0.25) && (percent <= 0.5))
	{
		score = CONFLICT_SCORE_RESOLVED_2ND_QTR;
	}
	else if ((percent > 0.5) && (percent <= 0.75))
	{
		score = CONFLICT_SCORE_RESOLVED_3RD_QTR;
	}
	else if ((percent > 0.75) && (percent <= 100))
	{
		score = CONFLICT_SCORE_RESOLVED_4TH_QTR;
	}

	_conflict_score.insert(std::pair<const char*, float>(trial_name, score));
}

void ScoreLogger::LogConflictFalseAlarm(const char* trial_name)
{
	_conflict_score.insert(std::pair<const char*, float>(trial_name, CONFLICT_SCORE_FALSE_ALARM));
}

void ScoreLogger::LogConflictScore(const char* trial_name)
{
	_conflict_score.insert(std::pair<const char*, float>(trial_name, CONFLICT_SCORE_CONFLICTED));
}

void ScoreLogger::GetCumulativeTotalScores(
			float& total_acceptance_task_score, 
			float& total_conflict_score,
			float& a_totalHandoffTaskScore)
{
	for (std::multimap<const char*, float>::const_iterator it = 
			_acceptance_task_score.begin(); it != _acceptance_task_score.end(); it++)
	{
		total_acceptance_task_score += it->second;
	}

	for (std::multimap<const char*, float>::const_iterator it2 = 
			_conflict_score.begin(); it2 != _conflict_score.end(); it2++)
	{
		total_conflict_score += it2->second;
	}

	for (std::multimap<const char*, float>::const_iterator it3 = 
			m_handoffTaskScore.begin(); it3 != m_handoffTaskScore.end(); it3++)
	{
		a_totalHandoffTaskScore += it3->second;
	}
}

void ScoreLogger::GetTotalScoreForTrial(const char* a_trialName,
										float& a_totalAcceptanceTaskScore,
										float& a_totalConflictScore,
										float& a_totalHandoffTaskScore)
{
	for (std::multimap<const char*, float>::const_iterator it = 
			_acceptance_task_score.begin(); it != _acceptance_task_score.end(); it++)
	{
		if (std::string(it->first) == std::string(a_trialName))
		{
			a_totalAcceptanceTaskScore += it->second;
		}
	}
		
	for (std::multimap<const char*, float>::const_iterator it2 = 
			_conflict_score.begin(); it2 != _conflict_score.end(); it2++)
	{
		if (std::string(it2->first) == std::string(a_trialName))
		{
			a_totalConflictScore += it2->second;
		}
	}

	for (std::multimap<const char*, float>::const_iterator it3 = 
			m_handoffTaskScore.begin(); it3 != m_handoffTaskScore.end(); it3++)
	{
		if (std::string(it3->first) == std::string(a_trialName))
		{
			a_totalHandoffTaskScore += it3->second;
		}
	}
}