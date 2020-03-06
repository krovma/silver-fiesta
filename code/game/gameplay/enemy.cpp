#include "gameplay/enemy.h"
#include "glare/core/event.h"
#include "gameplay/bullet_proto.h"
#include "glare/math/aabb2.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/renderer.h"

static vec2	PLAYER_EXTEND { 16.f, 24.f };
//static sprite_sheet* g_enemy_sprite_sheet = nullptr;

enemy::enemy(enemy_proto* proto)
	:m_proto(proto)
{
	sprite_sheet* enemy_sheet = sprite_sheet::get_sprite_sheet("marisa");
	m_anim = enemy_sheet->create_sprite_copy(15);
	m_collider = new disk_collider(this, 18.f);
}

enemy::~enemy()
{
	delete m_anim;
	delete m_collider;
}

void enemy::start()
{
	m_position = {-300, 0};
	m_velocity = {100, 0};
}

void enemy::begin_frame()
{
}

void enemy::update(float32 delta_sec)
{
	m_next_bullet -= delta_sec;
	if (m_next_bullet <= 0) {
		dict args;
		args.set("spawner", static_cast<entity*>(this));
		args.set("proto", bullet_proto::get_bullet_proto("enemy-test"));
		event::fire("game-spawn-bullet", args);
		m_next_bullet = 0.3f;
	}
	m_position += m_velocity * delta_sec;
}

void enemy::render(mesh& collection) const
{
	aabb2 box;
	box.min = m_position - PLAYER_EXTEND;
	box.max = m_position + PLAYER_EXTEND;
	vec2 uvs[4];
	m_anim->get_current_frame_uvs(uvs);
	mesh_builder::add_aabb2(collection, box, uvs);
}

void enemy::end_frame()
{
	if (m_position.x > 375) {
		m_valid = false;
	}
}

void enemy::stop()
{
}
