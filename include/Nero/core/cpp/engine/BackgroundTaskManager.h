////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef BACKGROUNDTASKMANAGER_H
#define BACKGROUNDTASKMANAGER_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/Logging.h>
//Cpp
#include <functional>
#include <future>
/////////////////////////////////////////////////////////////
namespace nero
{
	//alias
	class BackgroundTaskManager;
	using BTManager = BackgroundTaskManager;

	class BackgroundTaskManager
	{
		public:
			//create and schedule a task
			static void startTask(void (*callback)(BackgroundTask::Ptr backgroundTask),
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);

			static void startTask(void (*callback)(const Parameter& parameter, BackgroundTask::Ptr backgroundTask),
								  const Parameter& parameter,
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);

			static void startTask(std::function<void (BackgroundTask::Ptr backgroundTask)> callback,
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);

			static void startTask(std::function<void (const Parameter& parameter, BackgroundTask::Ptr backgroundTask)> callback,
								  const Parameter& parameter,
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);
			//test template
			template<typename T>
			static void startTask(void(T::*callBack)(BackgroundTask::Ptr backgroundTask), T* parent,
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);

			template<typename T>
			static void startTask(void(T::*callBack)(const Parameter& parameter, BackgroundTask::Ptr backgroundTask), T* parent,
								  const Parameter& parameter,
								  const std::string& taskName = StringPool.BLANK,
								  const std::string& taskCategory = EngineConstant.DEFAULT_TASK_CATEGORY);

			//get task
			static const std::vector<BackgroundTask::Ptr>&	getTaskTable();
			static const BackgroundTask::Ptr				findTaskByName(const std::string& name);
			static const std::vector<BackgroundTask::Ptr>	findTaskByCategory(const std::string& category);
			static const BackgroundTask::Ptr				getLastTask();
			static std::shared_future<int>&					getLastFuture();


		private:
						BackgroundTaskManager();
			static		BackgroundTask::Ptr createTask(const std::string& taskName, const std::string& taskCategory);

		private:
			static std::vector<BackgroundTask::Ptr> m_BackgroundTaskTable;
			static int m_CountTask;
			static std::map<std::string, std::shared_future<int>>	m_BackgroundFutureMap;
	};

	template<typename T>
	void BackgroundTaskManager::startTask(void(T::*callBack)(BackgroundTask::Ptr backgroundTask),
										  T* parent,
										  const std::string& taskName,
										  const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, parent, callBack]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			(parent->*callBack)(backgroundTask);

			return 0;
		}).share();
	}

	template<typename T>
	void BackgroundTaskManager::startTask(void(T::*callBack)(const Parameter& parameter, BackgroundTask::Ptr backgroundTask),
										  T* parent,
										  const Parameter& parameter,
										  const std::string& taskName,
										  const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, parent, callBack, parameter]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			(parent->*callBack)(parameter, backgroundTask);

			return 0;
		}).share();
	}
}




#endif // BACKGROUNDTASKMANAGER_H