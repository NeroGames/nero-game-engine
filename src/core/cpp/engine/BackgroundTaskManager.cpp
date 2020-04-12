////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/engine/BackgroundTaskManager.h>
#include <exception>
/////////////////////////////////////////////////////////////
namespace nero
{
	std::vector<BackgroundTask::Ptr>				BackgroundTaskManager::m_BackgroundTaskTable = std::vector<BackgroundTask::Ptr>();
	std::map<std::string, std::shared_future<int>>	BackgroundTaskManager::m_BackgroundFutureMap = std::map<std::string, std::shared_future<int>>();
	int												BackgroundTaskManager::m_CountTask = 0;

	BackgroundTaskManager::BackgroundTaskManager()
	{

	}

	BackgroundTask::Ptr BackgroundTaskManager::createTask(const std::string& taskName, const std::string& taskCategory)
	{
		//TODO, two task cannot have the same name
		m_CountTask++;

		std::string name = taskName == string::StringPool.BLANK ? "Task_" + string::toString(m_CountTask) : taskName;

		m_BackgroundTaskTable.push_back(std::make_shared<BackgroundTask>(name, taskCategory));

		return m_BackgroundTaskTable.back();
	}

	void BackgroundTaskManager::startTask(void (*callback)(BackgroundTask::Ptr backgroundTask), const std::string& taskName, const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, callback]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			callback(backgroundTask);

			return 0;
		}).share();
	}

	void BackgroundTaskManager::startTask(void (*callback)(const Parameter& parameter, BackgroundTask::Ptr backgroundTask), const Parameter& parameter, const std::string& taskName, const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, callback, parameter]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			callback(parameter, backgroundTask);

			return 0;
		}).share();
	}

	void BackgroundTaskManager::startTask(std::function<void (BackgroundTask::Ptr backgroundTask)> callback, const std::string& taskName, const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, callback]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			callback(backgroundTask);

			return 0;
		}).share();
	}

	void BackgroundTaskManager::startTask(std::function<void (const Parameter& parameter, BackgroundTask::Ptr backgroundTask)> callback, const Parameter& parameter, const std::string& taskName, const std::string& taskCategory)
	{
		BackgroundTaskManager::m_BackgroundFutureMap[taskName] = std::async(std::launch::async, [taskName, taskCategory, callback, parameter]()
		{
			BackgroundTask::Ptr backgroundTask = createTask(taskName, taskCategory);
			callback(parameter, backgroundTask);

			return 0;
		}).share();
	}

	const std::vector<BackgroundTask::Ptr>& BackgroundTaskManager::getTaskTable()
	{
		return m_BackgroundTaskTable;
	}

	const BackgroundTask::Ptr BackgroundTaskManager::findTaskByName(const std::string& name)
	{
		for(BackgroundTask::Ptr task : m_BackgroundTaskTable)
		{
			if(task->getName() == name)
			{
				return task;
			}
		}

		return nullptr;
	}

	const std::vector<BackgroundTask::Ptr> BackgroundTaskManager::findTaskByCategory(const std::string& category)
	{
		std::vector<BackgroundTask::Ptr> result;

		for(BackgroundTask::Ptr task : m_BackgroundTaskTable)
		{
			if(task->getCategory() == category)
			{
				result.push_back(task);
			}
		}

		return result;
	}

	const BackgroundTask::Ptr BackgroundTaskManager::getLastTask()
	{
		if(!m_BackgroundTaskTable.empty())
		{
			return m_BackgroundTaskTable.back();
		}

		return nullptr;
	}

	std::shared_future<int>& BackgroundTaskManager::getLastFuture()
	{
		return m_BackgroundFutureMap.rbegin()->second;
	}


}

