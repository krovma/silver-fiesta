#include "glare/core/common.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "glare/core/window.h"
#include "glare/render/vertex.h"
#include "glare/render/shader.h"
#include "glare/dev/dev_ui.h"
#include "glare/core/rng.h"
#include "glare/core/clock.h"
#include "framework/app.h"
#include "imgui/imgui.h"
using namespace glare;

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT(*_imgui_proc) (HWND, UINT, WPARAM, LPARAM) = ImGui_ImplWin32_WndProcHandler;

app* the_app = nullptr;
rng g_rng;

bool game_windows_message_handling_procedure(void* hwnd, UINT message_code, WPARAM wparam, LPARAM lparam)
{
	const ImGuiIO *imgui_io = nullptr;
	if(dev_ui::m_run) {
		_imgui_proc(static_cast<HWND>(hwnd), message_code, wparam, lparam);
		imgui_io = &ImGui::GetIO();
	}
	if (message_code == WM_CLOSE) {
		the_app->event_close();
		return true;
	}
	if (message_code == WM_KEYDOWN) {
		the_app->event_keyboard(true, wparam);
		return true;
	}
	if (message_code == WM_KEYUP) {
		the_app->event_keyboard(false, wparam);
		return true;
	}
	if (message_code == WM_CHAR) {
		if (imgui_io && imgui_io->WantCaptureKeyboard) {
			return false;
		}
	}
	return false;
}

int WINAPI WinMain(_In_ HINSTANCE application_instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR command_line, _In_ int n_show_cmd)
{
	UNUSED(application_instance);
	UNUSED(prev_instance);
	UNUSED(command_line);
	UNUSED(n_show_cmd);
	DEFAULT_WINDOW_EVENT_PROC = game_windows_message_handling_procedure;
	// Start up
	// #Todo: Load game config XML
	g_master_clock = new clock(nullptr);
	g_master_clock->set_fps(60);
	the_app = new app();
	the_app->start();
	while (the_app->m_run)
	{
		const double last_frame_time = get_current_time_seconds();
		the_app->run_frame();
		const double current_time = get_current_time_seconds();
		const double dt = current_time - last_frame_time;
		g_master_clock->step(dt);
	}
	the_app->stop();
	delete the_app;
	return 0;
}