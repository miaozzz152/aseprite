// Aseprite
// Copyright (C) 2022  Igara Studio S.A.
//
// This program is distributed under the terms of
// the End-User License Agreement for Aseprite.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "app/script/graphics_context.h"

#include "app/color.h"
#include "app/color_utils.h"
#include "app/modules/palettes.h"
#include "app/script/engine.h"
#include "app/script/luacpp.h"
#include "app/util/conversion_to_surface.h"

#ifdef ENABLE_UI

namespace app {
namespace script {

void GraphicsContext::drawImage(const doc::Image* img, int x, int y)
{
  convert_image_to_surface(
    img,
    get_current_palette(),
    m_surface.get(),
    0, 0,
    x, y,
    img->width(), img->height());
}

namespace {

int GraphicsContext_gc(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  gc->~GraphicsContext();
  return 0;
}

int GraphicsContext_save(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  gc->save();
  return 0;
}

int GraphicsContext_restore(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  gc->restore();
  return 0;
}

int GraphicsContext_strokeRect(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  const gfx::Rect rc = convert_args_into_rect(L, 2);
  gc->strokeRect(rc);
  return 0;
}

int GraphicsContext_fillRect(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  const gfx::Rect rc = convert_args_into_rect(L, 2);
  gc->fillRect(rc);
  return 0;
}

int GraphicsContext_drawImage(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  if (const doc::Image* img = may_get_image_from_arg(L, 2)) {
    int x = lua_tointeger(L, 3);
    int y = lua_tointeger(L, 4);
    gc->drawImage(img, x, y);
  }
  return 0;
}

int GraphicsContext_get_width(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  lua_pushinteger(L, gc->width());
  return 1;
}

int GraphicsContext_get_height(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  lua_pushinteger(L, gc->height());
  return 1;
}

int GraphicsContext_get_antialias(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  lua_pushboolean(L, gc->antialias());
  return 1;
}

int GraphicsContext_set_antialias(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  const bool antialias = lua_toboolean(L, 2);
  gc->antialias(antialias);
  return 1;
}

int GraphicsContext_get_color(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  push_obj(L, color_utils::color_from_ui(gc->color()));
  return 1;
}

int GraphicsContext_set_color(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  const app::Color color = convert_args_into_color(L, 2);
  gc->color(color_utils::color_for_ui(color));
  return 1;
}

int GraphicsContext_get_strokeWidth(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  lua_pushnumber(L, gc->strokeWidth());
  return 1;
}

int GraphicsContext_set_strokeWidth(lua_State* L)
{
  auto gc = get_obj<GraphicsContext>(L, 1);
  const float strokeWidth = lua_tonumber(L, 2);
  gc->strokeWidth(strokeWidth);
  return 1;
}

const luaL_Reg GraphicsContext_methods[] = {
  { "__gc", GraphicsContext_gc },
  { "save", GraphicsContext_save },
  { "restore", GraphicsContext_restore },
  { "strokeRect", GraphicsContext_strokeRect },
  { "fillRect", GraphicsContext_fillRect },
  { "drawImage", GraphicsContext_drawImage },
  { nullptr, nullptr }
};

const Property GraphicsContext_properties[] = {
  { "width", GraphicsContext_get_width, nullptr },
  { "height", GraphicsContext_get_height, nullptr },
  { "antialias", GraphicsContext_get_antialias, GraphicsContext_set_antialias },
  { "color", GraphicsContext_get_color, GraphicsContext_set_color },
  { "strokeWidth", GraphicsContext_get_strokeWidth, GraphicsContext_set_strokeWidth },
  { nullptr, nullptr, nullptr }
};

} // anonymous namespace

DEF_MTNAME(GraphicsContext);

void register_graphics_context_class(lua_State* L)
{
  REG_CLASS(L, GraphicsContext);
  REG_CLASS_PROPERTIES(L, GraphicsContext);
}

} // namespace script
} // namespace app

#endif // ENABLE_UI
