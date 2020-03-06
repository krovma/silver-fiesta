#include "framework/common.h"
#include "editor/editor.h"
#include <filesystem>
#include <fstream>
#include "glare/core/assert.h"
#include "glare/core/string_utils.h"
#include "glare/dev/dev_ui.h"
#include "glare/core/rng.h"
#include "glare/core/log.h"
#include "glare/render/renderer.h"
#include "gameplay/game_editor.h"

/////////////////////////////////////////////////////////////////////////////////
void bullet_proto_start_frame::create(const xml::node& node, void* proto)
{
	m_proto = reinterpret_cast<bullet_proto*>(proto);
	strcpy_s(m_proto->m_start_next_stage, xml::get_attr(node, "next", "NONE").c_str());
	m_proto->m_start_position = xml::get_attr(node, "position", vec2::ZERO);
	m_proto->m_start_rotation = xml::get_attr(node, "rotation", 0.f);
	m_proto->m_start_rotation = xml::get_attr(node, "local", false);
	editor_frame::create(node, proto);
}

void bullet_proto_start_frame::update()
{
	if (ImGui::TreeNode("Start")) {
		const texture2d*  my_texture = g_renderer->get_texture2d_by_id("test_sprite");
		ImTextureID		  my_tex_id = my_texture->get_view_handle();
		const float32     my_tex_w  = my_texture->m_size.x;
		const float32     my_tex_h  = my_texture->m_size.y;

		ImGui::Text("Sprite:");
		ImGui::SameLine();
		ImGui::Image(
			my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
			, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		ImGui::NewLine();
		ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_proto->m_start_position));
		ImGui::InputFloat("Rotation deg", &m_proto->m_start_rotation);
		ImGui::Checkbox("Local Move", &m_proto->m_start_local_move);
		ImGui::InputText("Next Stage", m_proto->m_start_next_stage, GAME_MAX_ID_LENGTH);

		ImGui::TreePop();
	}
}

void bullet_proto_start_frame::destroy()
{
}

void bullet_proto_start_frame::update_xml_node()
{
	m_xml_node.attribute("next").set_value(m_proto->m_start_next_stage);
	m_xml_node.attribute("position").set_value(m_proto->m_start_position.repr().c_str());
	m_xml_node.attribute("rotation").set_value(m_proto->m_start_rotation);
	m_xml_node.attribute("local").set_value(m_proto->m_start_local_move);
}

void bullet_proto_custom_frame::create(const xml::node& node, void* proto)
{
	m_custom = reinterpret_cast<bullet_proto::custom*>(proto);
	strcpy_s(m_custom->m_custom_id, xml::get_attr(node, "id", "NONE").c_str());
	strcpy_s(m_custom->m_next_stage, xml::get_attr(node, "next", "NONE").c_str());
	auto moves = node.child("movements");
	auto blmv_forward = moves.child("move_forward");
	if (!blmv_forward.empty()) {
		m_custom->m_use[BLMV_FORWARD] = true;
		m_custom->m_blmv_forward_speed = xml::get_attr(blmv_forward, "speed", 1.0f);
		m_custom->m_blmv_forward_time = xml::get_attr(blmv_forward, "time", 1.0f);
	}
	auto blmv_toplayer = moves.child("move_toplayer");
	if (!blmv_toplayer.empty()) {
		m_custom->m_use[BLMV_TO_PLAYER] = true;
		m_custom->m_blmv_to_player_speed = xml::get_attr(blmv_toplayer, "speed", 1.0f);
		m_custom->m_blmv_to_player_time = xml::get_attr(blmv_toplayer, "time", 1.0f);
	
	}
	auto blmv_tospawner = moves.child("move_tospawner");
	if (!blmv_tospawner.empty()) {
		m_custom->m_use[BLMV_TO_SPAWNER] = true;
		m_custom->m_blmv_to_spawner_speed = xml::get_attr(blmv_tospawner, "speed", 1.0f);
		m_custom->m_blmv_to_spawner_time = xml::get_attr(blmv_tospawner, "time", 1.0f);
	
	}
	editor_frame::create(node, proto);
}

void bullet_proto_custom_frame::update()
{
	if (ImGui::TreeNode(m_custom->m_custom_id)) {
		//ImGui::Text("%s", m_stage_id);
		//ImGui::SameLine();
		if (ImGui::Button("Stage ID")) {
			ImGui::OpenPopup("EditCustomStageID");
		}
		if (ImGui::BeginPopup("EditCustomStageID")) {
			ImGui::InputText("New ID", m_rename_buf, GAME_MAX_ID_LENGTH);
			if (ImGui::Button("Confirm")) {
				strcpy_s(m_custom->m_custom_id, m_rename_buf);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Discard")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		//ImGui::NewLine();
		ImGui::Checkbox("1.Forward Move", &m_custom->m_use[BLMV_FORWARD]);
		if (m_custom->m_use[BLMV_FORWARD]) {
			ImGui::InputFloat("Speed##BLMVFW", &m_custom->m_blmv_forward_speed);
			ImGui::InputFloat("Time##BLMVFW", &m_custom->m_blmv_forward_time);
		}
		ImGui::Checkbox("2. To Player", &m_custom->m_use[BLMV_TO_PLAYER]);
		if (m_custom->m_use[BLMV_TO_PLAYER]) {
			ImGui::InputFloat("Speed##BLMVTP", &m_custom->m_blmv_to_player_speed);
			ImGui::InputFloat("Time##BLMVTP", &m_custom->m_blmv_to_player_time);
		}
		ImGui::Checkbox("3. To Spawner", &m_custom->m_use[BLMV_TO_SPAWNER]);
		if (m_custom->m_use[BLMV_TO_SPAWNER]) {
			ImGui::InputFloat("Speed##BLMVTS", &m_custom->m_blmv_to_spawner_speed);
			ImGui::InputFloat("Time##BLMVTS", &m_custom->m_blmv_to_spawner_time);
		}
		//... More Comes here
		ImGui::InputText("Next Stage", m_custom->m_next_stage, GAME_MAX_ID_LENGTH);

		ImGui::TreePop();
	}
}

void bullet_proto_custom_frame::destroy()
{
}

void bullet_proto_custom_frame::update_xml_node()
{
	m_xml_node.attribute("id").set_value(m_custom->m_custom_id);
	m_xml_node.attribute("next").set_value(m_custom->m_next_stage);
	m_xml_node.remove_child("movements");
	auto move = m_xml_node.append_child("movements");
	if (m_custom->m_use[BLMV_FORWARD]) {
		auto cur = move.append_child("move_forward");
		cur.append_attribute("speed").set_value(m_custom->m_blmv_forward_speed);
		cur.append_attribute("time").set_value(m_custom->m_blmv_forward_time);
	}
	if (m_custom->m_use[BLMV_TO_PLAYER]) {
		auto cur = move.append_child("move_toplayer");
		cur.append_attribute("speed").set_value(m_custom->m_blmv_to_player_speed);
		cur.append_attribute("time").set_value(m_custom->m_blmv_to_player_time);
	}
		if (m_custom->m_use[BLMV_TO_SPAWNER]) {
		auto cur = move.append_child("move_tospawner");
		cur.append_attribute("speed").set_value(m_custom->m_blmv_to_spawner_speed);
		cur.append_attribute("time").set_value(m_custom->m_blmv_to_spawner_time);
	}
}

void bullet_proto_end_frame::create(const xml::node& node, void* proto)
{
	m_proto = reinterpret_cast<bullet_proto*>(proto);
	m_proto->m_end_effect = xml::get_attr(node, "effect", false);
	editor_frame::create(node, proto);
}

void bullet_proto_end_frame::update()
{
	if (ImGui::TreeNode("End")) {
		ImGui::Checkbox("Show Effect", &m_proto->m_end_effect);
		ImGui::TreePop();
	}
}

void bullet_proto_end_frame::destroy()
{
}

void bullet_proto_end_frame::update_xml_node()
{
	m_xml_node.attribute("effetc").set_value(m_proto->m_end_effect);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bullet_proto_editor::bullet_proto_editor(game_editor* game)
	:m_game(game)
{
}

bullet_proto_editor::~bullet_proto_editor()
{
	delete m_start_frame;
	delete m_end_frame;
	for(auto& each : m_custom_frames) {
		delete each;
	};
}

void bullet_proto_editor::start(const char* xml_path)
{
	if (!std::filesystem::exists(xml_path)) {
		//std::ofstream fout(xml_path);
		//fout.close();
		m_xml_doc = new xml::document();
		xml::node root = m_xml_doc->append_child("bullet_proto");
		root.append_attribute("id").set_value(format("untitled-%i", g_rng.rand()).c_str());

		xml::node start = root.append_child("start");
		start.append_attribute("local_move")	.set_value("false");
		start.append_attribute("position")		.set_value("0,0");
		start.append_attribute("rotation")		.set_value("0");
		//start.append_attribute("sprite")		.set_value("");
		start.append_attribute("next")			.set_value("NONE");
		
		xml::node custom = root.append_child("custom");

		xml::node end = root.append_child("end");
		end.append_attribute("effect")			.set_value("false");
		
		if (!m_xml_doc->save_file(xml_path)) {
			FATAL(format("Saving data file %s failed"));
		}
		delete m_xml_doc;
	}
	m_xml_doc = xml::load_file(xml_path);
	m_xml_path = xml_path;
	const xml::node root = m_xml_doc->child("bullet_proto");
	strcpy_s(m_tmp_proto.m_proto_id, root.attribute("id").value());
	
	//1. Init start frame node
	{
		m_start_frame = new bullet_proto_start_frame();
		m_start_frame->create(root.child("start"), &m_tmp_proto);
	}
	// 2. Init end frame node
	{
		m_end_frame = new bullet_proto_end_frame();
		m_end_frame->create(root.child("end"), &m_tmp_proto);
	}
	m_custom_frame_node = root.child("custom");
	auto customs = m_custom_frame_node.children("frame");
	for (auto& each_frame : customs) {
		auto& custom = m_tmp_proto.m_custom_stages.emplace_back(bullet_proto::custom());
		bullet_proto_custom_frame* created = new bullet_proto_custom_frame();
		created->create(each_frame, &custom);
		m_custom_frames.emplace_back(created);
	}
}

void bullet_proto_editor::update()
{
	ImGui::Begin("Bullet Proto", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::BeginGroup();
	ImGui::InputText("ProtoID", m_tmp_proto.m_proto_id, GAME_MAX_ID_LENGTH);
	ImGui::NewLine();
	if (ImGui::Button("Play")) {
		m_game->create_test_bullet(m_tmp_proto);
	}
	ImGui::SameLine();
	ImGui::Button("Apply");
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		save_xml();	
	}
	ImGui::SameLine();
	ImGui::ArrowButton("Back", ImGuiDir_Left);
	ImGui::EndGroup();
	ImGui::BeginChild("Stages",ImVec2(0,0), true);
	{
		m_start_frame->update();
		for (auto& each_frame : m_custom_frames) {
			each_frame->update();
		}
		if(ImGui::Button("+")) {
			add_new_stage();
		}
		m_end_frame->update();
	}
	ImGui::EndChild();
	ImGui::End();
}

void bullet_proto_editor::end()
{
	if (!save_xml()) {
		ALERT(format("Saving %s failed.", m_xml_path.c_str()));
	}
}

void bullet_proto_editor::add_new_stage()
{
	auto created = new bullet_proto_custom_frame();
	xml::node new_node = m_custom_frame_node.append_child("frame");
	new_node.append_attribute("id").set_value(format("stage-%i", g_rng.rand()).c_str());
	new_node.append_attribute("next").set_value("NONE");
	auto move = new_node.append_child("movements");
	move.append_child("move_forward");
	move.append_child("move_toplayer");
	move.append_child("move_tospawner");
	move.append_child("move_sinusoidal");
	move.append_child("move_circle");
	move.append_child("move_random");
	auto& new_custom = m_tmp_proto.m_custom_stages.emplace_back(bullet_proto::custom());
	created->create(new_node, &new_custom);
	m_custom_frames.emplace_back(created);
}

bool bullet_proto_editor::save_xml()
{
	m_xml_doc->child("bullet_proto").attribute("id").set_value(
		m_tmp_proto.m_proto_id
	);
	m_start_frame->update_xml_node();
	for(auto& each: m_custom_frames) {
		each->update_xml_node();
	}
	m_end_frame->update_xml_node();

	return m_xml_doc->save_file(m_xml_path.c_str());
}
