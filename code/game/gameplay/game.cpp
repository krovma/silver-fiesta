#include "gameplay/game.h"
#include "framework/app.h"
#include "glare/render/renderer.h"
#include "glare/render/shader.h"
#include "glare/core/color.h"
#include "glare/render/mesh_builder.h"
#include "glare/render/vertex.h"
#include "glare/render/sprite.h"
#include "editor/editor.h"
#include "imgui/imgui.h"
#include "gameplay/bullet.h"

renderer* g_renderer = nullptr;
static texture2d* g_tex = nullptr;
static texture2d* g_bgtex = nullptr;

static void _set_imgui_style()
{
	ImVec4* colors                         = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg]               = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]             = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_SliderGrab]            = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_Button]                = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	colors[ImGuiCol_ButtonHovered]         = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	colors[ImGuiCol_ButtonActive]          = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	colors[ImGuiCol_Header]                = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	colors[ImGuiCol_HeaderHovered]         = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_HeaderActive]          = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	colors[ImGuiCol_Separator]             = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	colors[ImGuiCol_SeparatorActive]       = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]         = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
}

game::game(app* app)
	:m_app(app)
{
	g_renderer = m_app->m_renderer;
}
game::~game()
{
	delete m_unlit_shader;
}

void game::start()
{
	_set_imgui_style();
	m_unlit_shader = shader::load_from_xml("data/shader/unlit.shader", m_app->m_renderer);
	g_renderer->m_buffer_vbo->set_buffer_layout(buffer_layout::acquire_layout_of<vertex_pcu>());
	m_unlit_shader->create_dx_vbo_layout(g_renderer->m_buffer_vbo->get_buffer_layout());
	g_tex = g_renderer->load_texture2d_from_file("test_sprite", "data/texture/d_test4x4.png");
	g_bgtex = g_renderer->load_texture2d_from_file("bg_sprite", "data/texture/d_stage06h.png");
	m_test_sprite = new sprite_sheet(g_tex, {4,4});

	m_proto_editor = new bullet_proto_editor(this);
	constexpr const char* filename = "data/game/bullet_TestBullet0.xml";
	m_proto_editor->start(filename);

	m_test_mesh = new mesh(buffer_layout::acquire_layout_of<vertex_pcu>());
	const aabb2 zone{{-375, -660}, {375, 180}};
	mesh_builder::add_aabb2(*m_test_mesh, zone);
	m_root = new  empty_entity();
}

void game::begin_frame()
{
	//m_proto_editor->begin_frame();
	if (m_test_bullet) {
		m_test_bullet->begin_frame();
	}
}

void game::update(float32 delta_sec)
{
	m_proto_editor->update();
	m_test_mesh->update_gpu_mesh(g_renderer);
	if (m_test_bullet) {
		m_test_bullet->update(delta_sec);
	}
	// ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	// if (ImGui::BeginMenuBar()) {
	// 	if (ImGui::BeginMenu("File")) {
	// 		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	// 		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	// 		if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
	// 		ImGui::EndMenu();
	// 	}
	// 	ImGui::EndMenuBar();
	// }
	//
	// // Edit a color (stored as ~4 floats)
	// ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&my_color));
	//
	// // Plot some values
	// const float my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
	// ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));
	//
	// // Display contents in a scrolling region
	// ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	// ImGui::BeginChild("Scrolling");
	// for (int n = 0; n < 50; n++)
	// 	ImGui::Text("%04d: Some text", n);
	// ImGui::EndChild();
	// ImGui::End();
	//
	
}

void game::render() const
{
	static vec2 fraction = vec2{750.f, 840.f} / vec2{1200.f, 900.f};
	
	g_renderer->set_ortho({-375, -660}, {375, 180}, -1, 1);
	//const sprite s = m_test_sprite->get_sprite({3,2});
	g_renderer->set_viewport(fraction * g_renderer->m_resolution, {30, 30});
	
	g_renderer->bind_shader(m_unlit_shader);
	g_renderer->bind_constant_buffer(CONSTANT_CAMERA_BUFFER, g_renderer->m_buffer_project);
	g_renderer->bind_texture(g_bgtex, TEXTURE_SLOT_DIFFUSE, MIN_LINEAR_MAG_LINEAR);
	g_renderer->draw_mesh(m_test_mesh);
	//g_renderer->m_buffer_vbo->buffer(verts.data(), verts.size());
	//g_renderer->bind_vbo(g_renderer->m_buffer_vbo);
	//g_renderer->draw(verts.size(), 0);

	if (m_test_bullet) {
		g_renderer->bind_texture(g_tex);
		mesh tmp_mesh(buffer_layout::acquire_layout_of<vertex_pcu>());
		m_test_bullet->render(tmp_mesh);
		tmp_mesh.update_gpu_mesh(g_renderer);
		g_renderer->draw_mesh(&tmp_mesh);
	}
}

void game::end_frame()
{
	if (m_test_bullet) {
		m_test_bullet->end_frame();
	}
	if (m_test_bullet && m_test_bullet->is_garbage()) {
		delete m_test_bullet;
		m_test_bullet = nullptr;
	}
}

void game::stop()
{
	delete m_test_bullet;
	delete m_root;
	m_proto_editor->end();
}

void game::create_test_bullet(bullet_proto& proto)
{
	delete m_test_bullet;
	m_test_bullet = new bullet(m_root, proto);
	m_test_bullet->start();
}
