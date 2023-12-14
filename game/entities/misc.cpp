#include "misc.h"
#include "event.h"

void LevelTransition::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		SpylikeEvents::LevelChangeEvent le("LEVEL_Change", levelPath);
		eventManager->emit(le);
	}
}
