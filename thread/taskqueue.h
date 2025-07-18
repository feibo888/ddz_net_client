#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "cards.h"
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

struct Task
{
    int bet = 0;
    Cards cards;
};



class TaskQueue : public QObject
{
    Q_OBJECT
public:
    TaskQueue(const TaskQueue&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;

    static TaskQueue* getInstance();

    //1. 添加任务
    void add(Task& t);
    //2. 取出一个任务
    Task take();
    //3. 得到任务队列中任务的数量
    int size();
    //4. 情况任务队列
    void clear();

private:
    explicit TaskQueue(QObject *parent = nullptr);

private:
    static TaskQueue m_taskQueue;
    QQueue<Task> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;

signals:
};

#endif // TASKQUEUE_H
