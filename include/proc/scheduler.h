#pragma once

#include <proc/thread.h>
#include <int/handler.h>
#include <util/list.h>

void scheduler_queue_add(list_node_t *node);
void scheduler_queue_remove(list_node_t *node);

void scheduler_init(void);
void scheduler_set_idle(thread_t *idle);

void scheduler_tick(frame_t *frame);
