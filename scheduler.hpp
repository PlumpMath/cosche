#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <unordered_map>
#include <memory>

#include "task_node.hpp"
#include "toposort.hpp"

class AbstractTask;
class AbstractFuture;

template <class Rt>
class Task;

class Scheduler : private Toposort<TaskNode, TaskNodeHasher>
{

    friend AbstractTask;

public:

    Scheduler() : _running(false) {}

    template <class Rt>
    Task<Rt>& getNewTask()
    {
        _tasks.emplace_back
        (
            std::make_unique<Task<Rt>>(*this)
        );

        push(_tasks.back().get());

        return static_cast<Task<Rt>&>(*(_tasks.back()));
    }

    void run();

private:

    void checkFutures();

    void haltWaitingFuture(std::shared_ptr<AbstractFuture>&& future,
                           AbstractTask* task);

    bool _running;
    std::vector<std::unique_ptr<AbstractTask>> _tasks;
    std::unordered_map<std::shared_ptr<AbstractFuture>, AbstractTask*> _futures;
};

#endif // __SCHEDULER_H__