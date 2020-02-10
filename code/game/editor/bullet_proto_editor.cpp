#include "framework/common.h"
#include "editor/editor.h"
#include <filesystem>
#include <fstream>
#include "glare/core/assert.h"
#include "glare/core/string_utilities.h"
#include "glare/dev/dev_ui.h"
#include "glare/core/rng.h"
#include "glare/core/log.h"

/////////////////////////////////////////////////////////////////////////////////
void bullet_proto_start_frame::create(const xml::node& node)
{
	strcpy_s(m_next_stage, xml::get_attr(node, "next", "NONE").c_str());
	m_position = xml::get_attr(node, "position", vec2::ZERO);
	m_rotation = xml::get_attr(node, "rotation", 0.f);
	m_rotation = xml::get_attr(node, "local", false);
	editor_frame::create(node);
}

void bullet_proto_start_frame::update()
{
	if (ImGui::TreeNode("Start")) {
		ImGuiIO&    io        = ImGui::GetIO();
		ImTextureID my_tex_id = io.Fonts->TexID;
		float32     my_tex_w  = (float32)io.Fonts->TexWidth;
		float32     my_tex_h  = (float32)io.Fonts->TexHeight;

		ImGui::Text("Sprite:");
		ImGui::SameLine();
		ImGui::Image(
			my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
			, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
		ImGui::NewLine();
		ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
		ImGui::InputFloat("Rotation deg", &m_rotation);
		ImGui::Checkbox("Local Move", &m_local_move);
		ImGui::InputText("Next Stage", m_next_stage, NAME_BUFFER_SIZE);

		ImGui::TreePop();
	}
}

void bullet_proto_start_frame::destroy()
{
}

void bullet_proto_custom_frame::create(const xml::node& node)
{
	strcpy_s(m_stage_id, xml::get_attr(node, "id", "NONE").c_str());
	strcpy_s(m_next_stage, xml::get_attr(node, "next", "NONE").c_str());
	auto m1_forward = node.child("move_forward");
	if(m1_forward.first_attribute().empty()) {
		debug_log("Huraaaaayyy\n");
	}
	// parse all movements and set the value;
	editor_frame::create(node);
}

void bullet_proto_custom_frame::update()
{
	if (ImGui::TreeNode(m_stage_id)) {
		//ImGui::Text("%s", m_stage_id);
		//ImGui::SameLine();
		if (ImGui::Button("Stage ID")) {
			ImGui::OpenPopup("EditCustomStageID");
		}
		if (ImGui::BeginPopup("EditCustomStageID")) {
			ImGui::InputText("New ID", m_tmp_buf, NAME_BUFFER_SIZE);
			if (ImGui::Button("Confirm")) {
				strcpy_s(m_stage_id, m_tmp_buf);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Discard")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		//ImGui::NewLine();
		ImGui::Checkbox("1.Forward Move", &m_use[0]);
		if (m_use[0]) {
			ImGui::InputFloat("Speed", &m_1_forward_speed);
			ImGui::InputFloat("Time", &m_1_forward_time);
		}
		//... More Comes here
		ImGui::InputText("Next Stage", m_next_stage, NAME_BUFFER_SIZE);

		ImGui::TreePop();
	}
}

void bullet_proto_custom_frame::destroy()
{
}

void bullet_proto_end_frame::create(const xml::node& node)
{
	m_effect = xml::get_attr(node, "effect", false);
	editor_frame::create(node);
}

void bullet_proto_end_frame::update()
{
	if (ImGui::TreeNode("End")) {
		ImGui::Checkbox("Show Effect", &m_effect);
		ImGui::TreePop();
	}
}

void bullet_proto_end_frame::destroy()
{
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bullet_proto_editor::bullet_proto_editor()
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

	//1. Init start frame node
	{
		m_start_frame = new bullet_proto_start_frame();
		strcpy_s(m_bullet_id, root.attribute("id").value());
		m_start_frame->create(root.child("start"));
	}
	// 2. Init end frame node
	{
		m_end_frame = new bullet_proto_end_frame();
		m_end_frame->create(root.child("end"));
	}
	m_custom_frame_node = root.child("custom");
	auto customs = m_custom_frame_node.children("frame");
	for (auto& each_frame : customs) {
		bullet_proto_custom_frame* created = new bullet_proto_custom_frame();
		created->create(each_frame);
		m_custom_frames.emplace_back(created);
	}
}

void bullet_proto_editor::update()
{
	ImGui::Begin("Bullet Proto", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::BeginGroup();
	ImGui::InputText("ProtoID", m_bullet_id, NAME_BUFFER_SIZE);
	ImGui::NewLine();
	ImGui::Button("Play");
	ImGui::SameLine();
	ImGui::Button("Apply");
	ImGui::SameLine();
	ImGui::Button("Save");
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
	if (!m_xml_doc->save_file(m_xml_path.c_str())) {
		ALERT(format("Saving %s failed.", m_xml_path.c_str()));
	}
}

void bullet_proto_editor::add_new_stage()
{
	auto created = new bullet_proto_custom_frame();
	xml::node new_node = m_custom_frame_node.append_child("frame");
	new_node.append_attribute("id").set_value(format("stage-%i", g_rng.rand()).c_str());
	new_node.append_attribute("next").set_value("NONE");
	new_node.append_child("move_forward");
	new_node.append_child("move_toplayer");
	new_node.append_child("move_tospawner");
	new_node.append_child("move_sinusoidal");
	new_node.append_child("move_circle");
	new_node.append_child("move_random");
	created->create(new_node);
	m_custom_frames.emplace_back(created);
}
