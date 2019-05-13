#ifndef __SCORE_LOGGER_HDR__
#define __SCORE_LOGGER_HDR__

#include <map>

namespace Score
{
	const float ACCEPTANCE_TASK_SCORE_CORRECT	= 10;
	const float ACCEPTANCE_TASK_SCORE_INCORRECT = -10;
	const float CONFLICT_SCORE_FALSE_ALARM		= -40;
	const float CONFLICT_SCORE_RESOLVED_1ST_QTR = 40;
	const float CONFLICT_SCORE_RESOLVED_2ND_QTR = 35;
	const float CONFLICT_SCORE_RESOLVED_3RD_QTR = 30;
	const float CONFLICT_SCORE_RESOLVED_4TH_QTR = 25;
	const float CONFLICT_SCORE_CONFLICTED		= -20;
	const float HANDOFF_TASK_SCORE_CORRECT		= 10;
	const float HANDOFF_TASK_SCORE_INCORRECT	= -10;

	enum CorrectStatus
	{
		CORRECT_STATUS_UNKNOWN,
		CORRECT_STATUS_CORRECT,
		CORRECT_STATUS_INCORRECT
	};

	class ScoreLogger  
	{
	public:
		static ScoreLogger* instance();
		static void destroy();

		static void LogAcceptanceTaskScore(
			const char* trial_name, 
			CorrectStatus status);

		static void	LogHandoffTaskScore(
			const char* a_trialName,
			CorrectStatus a_correct);

		static void GetCumulativeTotalScores(
			float& total_acceptance_task_score, 
			float& total_conflict_score,
			float& a_totalHandoffTaskScore);

		static void GetTotalScoreForTrial(
			const char* a_trialName,
			float& a_totalAcceptanceTaskScore,
			float& a_totalConflictScore,
			float& a_totalHandoffTaskScore);

		static void LogConflictFalseAlarm(const char* trial_name);

		static void LogConflictResolvedScore(const char* trial_name,
												int acceptance_time, 
												int conflict_time, 
												int resolved_time);

		static void LogConflictScore(const char* trial_name);

	private:
		ScoreLogger();
		~ScoreLogger();

		static ScoreLogger* _instance;
		static std::multimap<const char*, float> _acceptance_task_score;
		static std::multimap<const char*, float> _conflict_score;
		static std::multimap<const char*, float> m_handoffTaskScore;
	};
};
#endif