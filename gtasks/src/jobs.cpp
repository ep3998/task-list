/*
 *  GTasks - A C++/Qt API client for Google Tasks
 *
 *  Copyright (C) 2011 Gregory Schlomoff <gregory.schlomoff@gmail.com>
 *                     http://gregschlom.com
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include "jobs.h"

#include <QStringList>
#include <QDateTime>

namespace GTasks {

/*!
  ListTasklistsJob
*/
ListTasklistsJob::ListTasklistsJob(Service* service)
	: Job(service,
		  Job::Get, Tasklist::generateSelfLink(""),
		  SIGNAL(result(GTasks::TasklistCollection, GTasks::Error)))
{
}

void ListTasklistsJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(TasklistCollection(response), error);
}

ListTasklistsJob& ListTasklistsJob::maxResults(int max)                    { addRequestParam("maxResults", max); return *this; }
ListTasklistsJob& ListTasklistsJob::pageToken(const QString& pageToken)    { addRequestParam("pageToken",  pageToken); return *this; }

/*!
  GetTasklistJob
*/
GetTasklistJob::GetTasklistJob(Service* service, const QUrl& selfLink)
	: Job(service,
		  Job::Get, selfLink,
		  SIGNAL(result(GTasks::Tasklist, GTasks::Error)))
{
}

void GetTasklistJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Tasklist(response), error);
}

/*!
  UpdateTasklistJob
*/
UpdateTasklistJob::UpdateTasklistJob(Service* service, const Tasklist& tasklist)
	: Job(service,
		  Job::Put, tasklist.selfLink(),
		  SIGNAL(result(GTasks::Tasklist, GTasks::Error)))
{
	setRequestData(tasklist.serialize());
}

void UpdateTasklistJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Tasklist(response), error);
}

/*!
  InsertTasklistJob
*/
InsertTasklistJob::InsertTasklistJob(Service* service, const Tasklist& tasklist)
	: Job(service,
		  Job::Post, Tasklist::generateSelfLink(""),
		  SIGNAL(result(GTasks::Tasklist, GTasks::Error)))
{
	QVariantMap data;
	data.insert("title", tasklist.title());
	setRequestData(data);
}

void InsertTasklistJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Tasklist(response), error);
}

/*!
  DeleteTasklistJob
*/
DeleteTasklistJob::DeleteTasklistJob(Service* service, const QUrl& selfLink)
	: Job(service,
		  Job::Delete, selfLink,
		  SIGNAL(result(GTasks::Error)))
{
}

void DeleteTasklistJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	Q_UNUSED(response);
	emit result(error);
}

/*!
  ListTasksJob
*/
ListTasksJob::ListTasksJob(Service* service, const QString& tasklistId)
	: Job(service,
		  Job::Get, Task::generateSelfLink(tasklistId, ""),
		  SIGNAL(result(GTasks::TaskCollection, GTasks::Error)))
{
}

void ListTasksJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(TaskCollection(response), error);
}

// Optional parameters setters
ListTasksJob& ListTasksJob::maxResults(int max)                    { addRequestParam("maxResults",   max); return *this; }
ListTasksJob& ListTasksJob::pageToken(const QString& pageToken)    { addRequestParam("pageToken",    pageToken); return *this; }
ListTasksJob& ListTasksJob::completedBefore(const QDateTime& date) { addRequestParam("completedMax", date); return *this; }
ListTasksJob& ListTasksJob::completedAfter(const QDateTime& date)  { addRequestParam("completedMin", date); return *this; }
ListTasksJob& ListTasksJob::dueBefore(const QDateTime& date)       { addRequestParam("dueMax",       date); return *this; }
ListTasksJob& ListTasksJob::dueAfter(const QDateTime& date)        { addRequestParam("dueMin",       date); return *this; }
ListTasksJob& ListTasksJob::updatedAfter(const QDateTime& date)    { addRequestParam("updatedMin",   date); return *this; }
ListTasksJob& ListTasksJob::showCompleted(bool flag)               { addRequestParam("showCompleted",flag); return *this; }
ListTasksJob& ListTasksJob::showDeleted(bool flag)                 { addRequestParam("showDeleted",  flag); return *this; }
ListTasksJob& ListTasksJob::showHidden(bool flag)                  { addRequestParam("showHidden",   flag); return *this; }

/*!
  GetTaskJob
*/
GetTaskJob::GetTaskJob(Service* service, const QUrl& selfLink)
	: Job(service,
		  Job::Get, selfLink,
		  SIGNAL(result(GTasks::Task, GTasks::Error)))
{
}

void GetTaskJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Task(response), error);
}

/*!
  InsertTaskJob
*/
InsertTaskJob::InsertTaskJob(Service* service, const QString& tasklistId, const Task& task)
	: Job(service,
		  Job::Post, Task::generateSelfLink(tasklistId, ""),
		  SIGNAL(result(GTasks::Task, GTasks::Error)))
{
	QVariantMap data;

	data.insert("title", task.title());
	data.insert("notes", task.notes());
	if (!task.due().isNull()) {
		data.insert("due", task.due());
	}

	setRequestData(data);
}

void InsertTaskJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Task(response), error);
}

InsertTaskJob& InsertTaskJob::asChildOf(const QString& taskId)
{
	if (!taskId.isEmpty()) {
		addRequestParam("parent", taskId);
	}
	return *this;
}
InsertTaskJob& InsertTaskJob::after(const QString& taskId)
{
	if (!taskId.isEmpty()) {
		addRequestParam("previous", taskId);
	}
	return *this;
}

/*!
  UpdateTaskJob
*/
UpdateTaskJob::UpdateTaskJob(Service* service, const Task& task)
	: Job(service,
		  Job::Put, task.selfLink(),
		  SIGNAL(result(GTasks::Task, GTasks::Error)))
{

	QVariantMap fullData = task.serialize();
	QVariantMap savedData;

	QStringList savedFields;
	savedFields << "id" << "title" <<  "notes" <<  "status" <<  "due";

	// Don't update the due date if it's empty. TODO: How do we clear the due date?
	if (fullData.value("due").toString().isEmpty()) {
		savedFields.removeOne("due");
	}

	foreach (const QString& field, savedFields) {
		if (fullData.contains(field)) {
			savedData.insert(field, fullData.value(field));
		}
	}

	setRequestData(savedData);
}

void UpdateTaskJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Task(response), error);
}

/*!
  DeleteTaskJob
*/
DeleteTaskJob::DeleteTaskJob(Service* service, const QUrl& selfLink)
	: Job(service,
		  Job::Delete, selfLink,
		  SIGNAL(result(GTasks::Error)))
{
}

void DeleteTaskJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	Q_UNUSED(response);
	emit result(error);
}

/*!
  MoveTaskJob
*/
MoveTaskJob::MoveTaskJob(Service* service, const QUrl& selfLink)
	: Job(service,
		  Job::Post, QUrl(selfLink.toString() + "/move"),
		  SIGNAL(result(GTasks::Task, GTasks::Error)))
{
}

void MoveTaskJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	emit result(Task(response), error);
}

MoveTaskJob& MoveTaskJob::asChildOf(const QString& taskId) { addRequestParam("parent",   taskId); return *this; }
MoveTaskJob& MoveTaskJob::after(const QString& taskId)     { addRequestParam("previous", taskId); return *this; }

/*!
  ClearTasksJob
*/
ClearTasksJob::ClearTasksJob(Service* service, const QString& tasklistId)
	: Job(service,
		  Job::Post, QUrl("https://www.googleapis.com/tasks/v1/lists/" + tasklistId + "/clear"),
		  SIGNAL(result(GTasks::Error)))
{
}

void ClearTasksJob::parseReply(const QVariantMap& response, const GTasks::Error& error)
{
	Q_UNUSED(response);
	emit result(error);
}

} // namespace GTasks

