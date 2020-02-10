#include "gameplay/game.h"
#include "framework/app.h"
#include "glare/render/renderer.h"
#include "glare/render/shader.h"
#include "glare/core/color.h"
#include "glare/render/vertex.h"
#include "editor/editor.h"
#include "imgui/imgui.h"

static renderer* g_renderer = nullptr;

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

	m_proto_editor = new bullet_proto_editor();
	constexpr const char* filename = "data/game/bullet_TestBullet0.xml";
	m_proto_editor->start(filename);
}

void game::begin_frame()
{
	//m_proto_editor->begin_frame();
}

static bool my_tool_active;
static rgba my_color;

void game::update()
{
	m_proto_editor->update();
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
	static std::vector<vertex_pcu> verts =
	{
		{{0,0,0}, color::RED, {0,0}},
		{{1,0,0}, color::GREEN, {0,0}},
		{{0,1,0}, color::BLUE, {0,0}},
	};
	g_renderer->bind_shader(m_unlit_shader);
	g_renderer->m_buffer_vbo->buffer(verts.data(), verts.size());
	g_renderer->bind_vbo(g_renderer->m_buffer_vbo);
	g_renderer->draw(3, 0);
}

void game::end_frame()
{
}

void game::stop()
{
	m_proto_editor->end();
}
