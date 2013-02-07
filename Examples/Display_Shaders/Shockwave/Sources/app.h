/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#pragma once

class App
{
public:
	int start(const std::vector<std::string> &args);
	void window_close();

private:
	void render_shockwave(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object);
	void on_input_up(const clan::InputEvent &key);
	void draw_texture(clan::Canvas &canvas, const clan::Rectf &rect, const clan::Colorf &color, const clan::Rectf &texture_unit1_coords);

	bool quit;
	clan::Vec2f center;
	float timer;
	float shockwave_time_elapsed;
	float shockwave_start_time;
	clan::Vec3f shockParams;
	float shockwave_rate;
	float glow;
};

