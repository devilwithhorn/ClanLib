/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "precomp.h"
#include "app.h"

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib App Example");
	desc.set_size(clan::Size(1024, 700), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// Connect the Window close event
	clan::Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up);

	font = clan::Font(canvas, "tahoma", 20);

	const int unset_value = -1232311;
	num_iterations = 0;
	base_line = 0;
	result_i_plus_plus = unset_value;
	result_empty = unset_value;
	result_create_string = unset_value;

	cb_main.set(this, &App::initialise_1);
	game_time.reset();

	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();

		clan::Size canvas_size = canvas.get_size();

		// Clear the display in a dark blue nuance
		canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

		std::string text("ClanLib Benchmark Utility");
		clan::Size text_size = font.get_text_size(canvas, text);
		font.draw_text(canvas, ( ( canvas_size.width - text_size.width) / 2), 32, text, clan::Colorf::white);

		int ypos = 96;
		const int ygap = 20;
		if (num_iterations)
		{
			font.draw_text(canvas, 10, ypos, clan::string_format("%1 = Iterations", num_iterations));
			ypos += ygap;
		}

		if (result_empty != unset_value)
		{
			font.draw_text(canvas, 10, ypos, clan::string_format("%1 = Base Line Test (ideally, should equal 0)", result_empty));
			ypos += ygap;
		}
		if (result_i_plus_plus != unset_value)
		{
			font.draw_text(canvas, 10, ypos, clan::string_format("%1 = {i++;}", result_i_plus_plus));
			ypos += ygap;
		}
		if (result_create_string != unset_value)
		{
			font.draw_text(canvas, 10, ypos, clan::string_format("%1 = {string=std::string();}", result_create_string));
			ypos += ygap;
		}
		cb_main.invoke();

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::draw_info(const std::string &text)
{
	clan::Size text_size = font.get_text_size(canvas, text);
	font.draw_text(canvas,  ( ( canvas.get_width() - text_size.width) / 2), 64, text, clan::Colorf::white);
	window.flip(1);
}

void App::initialise_1()
{
	draw_info("* Calculating Required Iterations *");

	cb_test.set(&tests, &Tests::test_empty);

	clan::ubyte64 start_time = clan::System::get_microseconds();
	for (num_iterations = 0; ; num_iterations++)
	{
		clan::ubyte64 current_time = clan::System::get_microseconds();

		if ((current_time - start_time) >= (1000000) * 1)		// Run basic test for max 1 second
			break;
	
		for (int cnt=0; cnt < 0xFFFFFF; cnt++)
		{
			num_iterations++;
			cb_test.invoke();
		}
	}
	cb_main.set(this, &App::initialise_2);
}

int App::run_test()
{
	clan::ubyte64 start_time = clan::System::get_microseconds();
	for (clan::ubyte64 cnt=0; cnt < num_iterations; cnt++)
	{
		cb_test.invoke();
	}
	clan::ubyte64 current_time = clan::System::get_microseconds();
	return (current_time - start_time) / base_line;

}

void App::initialise_2()
{
	draw_info("* Calculating Base Line *");

	cb_test.set(&tests, &Tests::test_empty);
	base_line = 1;
	base_line = run_test();

	cb_main.set(this, &App::test_1);
}

void App::test_1()
{
	draw_info("* Testing - Empty Function *");

	cb_test.set(&tests, &Tests::test_empty);
	result_empty = run_test();
	cb_main.set(this, &App::test_2);
}

void App::test_2()
{
	draw_info("* Testing - {i++} *");

	cb_test.set(&tests, &Tests::test_i_plus_plus);
	result_i_plus_plus = run_test();
	cb_main.set(this, &App::test_3);
}

void App::test_3()
{
	draw_info("* Testing - {string = std::string()} *");

	cb_test.set(&tests, &Tests::test_create_string);
	result_create_string = run_test();
	cb_main.set(this, &App::initialise_1);
}
