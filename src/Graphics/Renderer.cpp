#include <sstream>
#include "Renderer.h"
#include "../Core/Camera.h"
#include "../Core/Collision.h"

Renderer::Renderer(SDL_Window* window, Camera* cam) : camera(cam) {

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	renderer = SDL_CreateRenderer(window, NULL);

	//Set the renderer to *logically* render things at 640x360, then scale it up to the window at rendering time
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderLogicalPresentation(renderer, min_res.x, min_res.y, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

void Renderer::DrawSheet(const Sprite& sheet, const Vec2i& pos) {
	const SDL_FRect dest = { pos.x, pos.y, sheet.info.sheet_size.x, sheet.info.sheet_size.y };

	SDL_RenderTexture(renderer, sheet.texture, NULL, &dest);
}

void Renderer::DrawSprite(const Sprite& spr) {
	//Cast to SDL
	const Sprite::Info* si = &spr.info;

	//Only draw sprites if they will be seen by the camera
	Vec2i sprite_pos = Round(si->pos.x - (si->spr_size.x * si->scale.x * si->origin.x),
		si->pos.y - (si->spr_size.y * si->scale.y * si->origin.y));
	if (Collision::AABB(camera->viewport, Rect(sprite_pos, { si->spr_size.x * si->scale.x, si->spr_size.y * si->scale.y }))) {
		const SDL_FRect src = { si->curr_frame * si->frame_size.x,
								si->sheet_row * si->frame_size.y,
								si->frame_size.x,	si->frame_size.y };


		const SDL_FRect dest = { sprite_pos.x - camera->viewport.x,
								sprite_pos.y - camera->viewport.y,
								si->spr_size.x * si->scale.x,
								si->spr_size.y * si->scale.y };

		//Set the tint
		SDL_SetTextureColorMod(spr.texture, si->tint.r * 255, si->tint.g * 255, si->tint.b * 255);
		SDL_SetTextureAlphaMod(spr.texture, si->tint.a * 255);

		SDL_RenderTexture(renderer, spr.texture, &src, &dest);
	}
}

void Renderer::DrawTxt(Text& txt) {
	Text::Info* ti = &txt.info;

	SDL_Color c = {
		static_cast<Uint8>(ti->color.r * 255),
		static_cast<Uint8>(ti->color.g * 255),
		static_cast<Uint8>(ti->color.b * 255),
		static_cast<Uint8>(ti->color.a * 255)
	};

	if (!txt.font->GetFont()) {
		std::cout << "Font is null\n";
		return;
	}
	if (ti->str.length() == 0 or !ti->color.a)
		return;

	//Disable logical rendering so we can draw text properly
	SDL_SetRenderLogicalPresentation(renderer, win_size.x, win_size.y, SDL_LOGICAL_PRESENTATION_DISABLED);

	if (surface) SDL_DestroySurface(surface);
	surface = TTF_RenderText_Blended_Wrapped(txt.font->GetFont(), ti->str.c_str(), ti->str.length(), c, ti->max_width);
	if (!surface) {
		std::cout << "Failed to create text surface!\n";
		return;
	}
	if (text_tar) SDL_DestroyTexture(text_tar);
	text_tar = SDL_CreateTextureFromSurface(renderer, surface);
	if (!text_tar) {
		std::cout << "Failed to create text texture!\n";
		return;
	}

	TTF_GetStringSizeWrapped(txt.font->GetFont(), ti->str.c_str(), strlen(ti->str.c_str()), txt.GetPhysicalMaxW(), &ti->str_size.x, nullptr);
	ti->str_size.y = surface->h;


	Rect true_cam_vp = Rect({ camera->viewport.x, camera->viewport.y },
		{ camera->viewport.w * Text::res_scale, camera->viewport.h * Text::res_scale });
	Vec2i txt_pos = { ti->pos.x + camera->viewport.x, ti->pos.y + camera->viewport.y };
	txt_pos = Round(txt_pos.x - (ti->str_size.x * ti->origin.x), txt_pos.y - (ti->str_size.y * ti->origin.y));
	if (Collision::AABB(true_cam_vp, Rect(txt_pos, Vec2i(ti->str_size.x, ti->str_size.y)))) {
		SDL_FRect src_rect = { 0, 0, surface->w, surface->h };
		SDL_FRect dest_rect = {
			txt_pos.x - true_cam_vp.x,
			txt_pos.y - true_cam_vp.y,
			static_cast<float>(surface->w),
			static_cast<float>(surface->h)
		};
		SDL_RenderTexture(renderer, text_tar, &src_rect, &dest_rect);
	}
	//Re-enable logical rendering at the base resolution
	SDL_SetRenderLogicalPresentation(renderer, min_res.x, min_res.y, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
}

void Renderer::DrawGrid(const Vec2i& start, const Vec2i& end, const uchar& tile_size) {
	//Vertical Lines
	for (int i = start.x; i < end.x; i += tile_size)
		DrawLine(Line{ {i, start.y}, {i, end.y} }, Color(1, 0, 0));
	//Horizontal lines
	for (int i = start.y; i < end.y; i += tile_size)
		DrawLine(Line{ {start.x, i}, {end.x, i} }, Color(1, 0, 0));
}

void Renderer::DrawPath(std::vector<Vec2i> path, const Color& path_color) {

	Rect point_box = { {0}, {2} };
	for (const auto& point : path) {
		point_box.x = point.x - 1; point_box.y = point.y - 1;
		DrawRect(point_box, path_color);
	}
}

void Renderer::DrawLine(const Line& line, const Color& color, const uchar edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing Collision::LineRect
	SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

	SDL_RenderLine(renderer, line.x1 - camera->viewport.x, line.y1 - camera->viewport.y, line.x2 - camera->viewport.x, line.y2 - camera->viewport.y);
}

void Renderer::DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uchar edge_w) {
	//Only draw if colliding with the camera
	if (Collision::RectCircle(camera->viewport, circle)) {
		Vec2f circle_pos = { (float)(circle.x - camera->viewport.x), (float)(circle.y - camera->viewport.y) };
		float inner_r = circle.r - edge_w;

		//Outline
		if (stroke_color.a) {
			SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
			for (int w = -circle.r; w <= circle.r; ++w) {
				for (int h = -circle.r; h <= circle.r; ++h) {
					float dist_sq = w * w + h * h;
					if (dist_sq <= circle.r * circle.r and dist_sq >= inner_r * inner_r)
						SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
				}
			}
		}

		//Fill
		if (fill_color.a) {
			SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
			for (int w = -inner_r; w <= inner_r; ++w) {
				for (int h = -inner_r; h <= inner_r; ++h) {
					if (w * w + h * h <= inner_r * inner_r)
						SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
				}
			}
		}
	}
}

void Renderer::DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uchar edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing triangle collisions so fuck that
	Vec2f tri_pos1 = { (float)(tri.pos1.x - camera->viewport.x), (float)(tri.pos1.y - camera->viewport.y) };
	Vec2f tri_pos2 = { (float)(tri.pos2.x - camera->viewport.x), (float)(tri.pos2.y - camera->viewport.y) };
	Vec2f tri_pos3 = { (float)(tri.pos3.x - camera->viewport.x), (float)(tri.pos3.y - camera->viewport.y) };

	if (fill_color.a) {
		SDL_FColor f_color = { fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255 };

		//Draw the triangle
		SDL_Vertex verts[3];

		verts[0].position = { tri_pos1.x, tri_pos1.y };
		verts[0].color = f_color;

		verts[1].position = { tri_pos2.x, tri_pos2.y };
		verts[1].color = f_color;

		verts[2].position = { tri_pos3.x, tri_pos3.y };
		verts[2].color = f_color;

		SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
	}

	//Draw the edges
	if (stroke_color.a) {
		SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
		SDL_RenderLine(renderer, tri_pos1.x, tri_pos1.y, tri_pos2.x, tri_pos2.y);
		SDL_RenderLine(renderer, tri_pos2.x, tri_pos2.y, tri_pos3.x, tri_pos3.y);
		SDL_RenderLine(renderer, tri_pos3.x, tri_pos3.y, tri_pos1.x, tri_pos1.y);
	}
}

void Renderer::DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color, const uchar edge_w) {
	//Only draw if colliding with the camera
	if (Collision::AABB(rect, camera->viewport)) {
		Vec2f rect_pos = { (float)(rect.x - camera->viewport.x), (float)(rect.y - camera->viewport.y) };
		float w = rect.w;
		float h = rect.h;
		//Normalize w/h
		if (w < 0) {
			rect_pos.x += w;
			w = -w;
		}
		if (h < 0) {
			rect_pos.y += h;
			h = -h;
		}
		//Draw the fill
		if (fill_color.a) {
			SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
			SDL_FRect sdl_rect = { rect_pos.x, rect_pos.y, w, h };
			SDL_RenderFillRect(renderer, &sdl_rect);
		}

		//Draw the edges
		if (stroke_color.a) {
			SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
			//Top
			SDL_FRect top = { rect_pos.x, rect_pos.y, w, edge_w };
			SDL_RenderFillRect(renderer, &top);
			//Bottom
			SDL_FRect bot = { rect_pos.x, (rect_pos.y + h) - edge_w, w, edge_w };
			SDL_RenderFillRect(renderer, &bot);
			//Left
			SDL_FRect left = { rect_pos.x, rect_pos.y + edge_w, edge_w, h - (edge_w * 2) };
			SDL_RenderFillRect(renderer, &left);
			//Right
			SDL_FRect right = { rect_pos.x + w - edge_w, rect_pos.y + edge_w, edge_w, h - edge_w * 2 };
			SDL_RenderFillRect(renderer, &right);
		}
	}
}