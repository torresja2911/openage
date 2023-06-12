// Copyright 2018-2023 the openage authors. See copying.md for legal info.

#include "world.h"

#include "log/log.h"

#include <nyan/nyan.h>

#include "gamestate/game_entity.h"
#include "gamestate/game_state.h"
#include "renderer/render_factory.h"


namespace openage::gamestate {

World::World(const std::shared_ptr<GameState> &state) :
	state{state} {
	// TODO
}

void World::load_data() {
	auto nyan_db = this->state->get_nyan_db();

	auto game_entities = nyan_db->get_obj_children_all("engine.util.game_entity.GameEntity");
	for (auto entity : game_entities) {
		log::log(INFO << "found game entity: " << entity);
	}
}

void World::attach_renderer(const std::shared_ptr<renderer::RenderFactory> &render_factory) {
	this->render_factory = render_factory;

	for (auto entity_ref : state->get_game_entities()) {
		auto world_render_entity = this->render_factory->add_world_render_entity();
		entity_ref.second->set_render_entity(world_render_entity);
	}
}


} // namespace openage::gamestate
