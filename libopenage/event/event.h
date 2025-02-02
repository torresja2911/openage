// Copyright 2017-2023 the openage authors. See copying.md for legal info.

#pragma once

#include <cstddef>
#include <memory>

#include "event/eventhandler.h"
#include "time/time.h"


namespace openage::event {
class EventEntity;

using event_hash_t = size_t;

/**
 * The actual one event that may be called - it is used to manage the event itself.
 * It does not need to be stored.
 */
class Event : public std::enable_shared_from_this<Event> {
public:
	Event(const std::shared_ptr<EventEntity> &trgt,
	      const std::shared_ptr<EventHandler> &eventhandler,
	      const EventHandler::param_map &params);

	const std::weak_ptr<EventEntity> &get_entity() const {
		return this->entity;
	}

	const std::shared_ptr<EventHandler> &get_eventhandler() const {
		return this->eventhandler;
	}

	/**
	 * Reschedule will call the predict_invoke_time method to initiate a reschedule
	 * for the event it uses the reference_time as base for its calculation
	 */
	void reschedule(const time::time_t reference_time);

	event_hash_t hash() const {
		return this->myhash;
	}

	const time::time_t &get_time() const {
		return this->time;
	}

	void set_time(const time::time_t &t) {
		this->time = t;
	}

	const EventHandler::param_map &get_params() const {
		return this->params;
	}

	/**
	 * Let this event depend on another an event entity.
	 * When this entity is changes, the event is reevaluated.
	 *
	 * To be called in the EventHandler::setup function.
	 */
	void depend_on(const std::shared_ptr<EventEntity> &dependency);

	/**
     * Cancel the event.
     */
	void cancel(const time::time_t reference_time);

	/**
	 * For sorting events by their trigger time.
	 */
	bool operator<(const Event &other) const;

	/**
	 * When a change happens on an EventEntity (this->entity),
	 * it needs to be processed and all depending events need reevaluation as well.
	 * This registers the time so we know the point in time that we
	 * need to go back to and handle the change.
	 * When changes happen after `last_change_time` in the same time-reaching-round,
	 * they can be ignored since the earlies point in time determines all implications.
	 */
	void set_last_changed(const time::time_t &t) {
		this->last_change_time = t;
	}

	/**
	 * Get the time the  event was changed the last time.
	 */
	const time::time_t &get_last_changed() const {
		return this->last_change_time;
	}

private:
	/**
	 * Parameters for the event (determined by its EventHandler)
	 */
	EventHandler::param_map params;

	/** The actor that this event refers to. */
	std::weak_ptr<EventEntity> entity;

	/** Type of this event. */
	std::shared_ptr<EventHandler> eventhandler;

	/**
	 * Time this event occurs/occured.
	 * It establishes the order of events in the EventQueue.
	 */
	time::time_t time;

	/** Time this event was registered to be changed last. */
	time::time_t last_change_time = time::time_t::min_value();

	/** Precalculated std::hash for the event */
	event_hash_t myhash;
};


} // namespace openage::event
