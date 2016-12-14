
#include <pango/pangocairo.h>
#include <gst/gst.h>
#include "MediaPipeline.hpp"
#include <ThatOverlayImplFactory.hpp>
#include "ThatOverlayImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include <gst/video/video.h>

#define GST_CAT_DEFAULT kurento_that_overlay_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoThatOverlayImpl"

namespace kurento
{
namespace module
{
namespace thatoverlay
{

static void
prepare_overlay (GstElement * overlay, GstCaps * caps, gpointer user_data)
{
  ThatOverlayImpl::CairoOverlayState *state = (ThatOverlayImpl::CairoOverlayState *)user_data;
  GstVideoInfo info;

  gst_video_info_from_caps (&info, caps);
  state->width = info.width;
  state->height = info.height;
  state->frame = 0;
  state->valid = TRUE;
}

static void
draw_overlay (GstElement * overlay, cairo_t * cr, guint64 timestamp, 
  guint64 duration, gpointer user_data)
{
  ThatOverlayImpl::CairoOverlayState *s = (ThatOverlayImpl::CairoOverlayState *)user_data;

  if (!s->valid)
    return;

  PangoLayout *layout;
  PangoFontDescription *desc;

  layout = pango_cairo_create_layout (cr);
  cairo_identity_matrix (cr);

  pango_layout_set_text (layout, s->watermark.c_str(), -1);
  desc = pango_font_description_from_string (s->watermarkFont.c_str());
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  cairo_set_source_rgba (cr, s->watermarkColor.r, s->watermarkColor.g, s->watermarkColor.b, s->watermarkColor.a);
  pango_cairo_update_layout (cr, layout);
  if ((s->frame % s->watermarkInterval) == 0)
  {
    int x = 0, y = 0;
    while (x < (s->width / 2) && y < 20)
    {
      x = rand() % s->width;
      y = rand() % s->height;
    }
    s->watermarkPosition.x = x;
    s->watermarkPosition.y = y;
  }
  cairo_move_to(cr, s->watermarkPosition.x, s->watermarkPosition.y);
  pango_cairo_show_layout (cr, layout);
  g_object_unref (layout);

  layout = pango_cairo_create_layout (cr);
  cairo_scale (cr, 1, 1);
  pango_layout_set_text (layout, s->title.c_str(), -1);
  desc = pango_font_description_from_string (s->titleFont.c_str());
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
  cairo_set_source_rgba (cr, s->titleColor.r, s->titleColor.g, s->titleColor.b, s->titleColor.a);
  cairo_move_to(cr, s->titlePosition.x, s->titlePosition.y);
  pango_cairo_update_layout (cr, layout);
  pango_cairo_show_layout (cr, layout);
  g_object_unref (layout);

  s->frame++;
}

ThatOverlayImpl::ThatOverlayImpl (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline)  : FilterImpl (config, std::dynamic_pointer_cast<MediaObjectImpl>(mediaPipeline)) 
{
  GstElement* sampleplugin;

  g_object_set (element, "filter-factory", "cairooverlay", NULL);

  g_object_get (G_OBJECT (element), "filter", &sampleplugin, NULL);

  if (sampleplugin == NULL) {
    throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
                            "Media Object not available");
  }

  st.title = "developer_79756";
  st.titleFont = "Serif 14";
  st.watermark = "255.255.255.255\n12JAN2016 23:24Z";
  st.watermarkFont = "Sans Mono 14";

  st.titleColor.r = 1;
  st.titleColor.g = 1;
  st.titleColor.b = 0;
  st.titleColor.a = 1;
  st.watermarkColor.r = 1;
  st.watermarkColor.g = 1;
  st.watermarkColor.b = 1;
  st.watermarkColor.a = 0.5;

  st.watermarkInterval = 4;

  g_signal_connect (sampleplugin, "draw", G_CALLBACK (draw_overlay), &this->st);
  g_signal_connect (sampleplugin, "caps-changed",   G_CALLBACK (prepare_overlay), &this->st);

  g_object_unref (sampleplugin);
}

void ThatOverlayImpl::setup (int x, int y, const std::string &title, const std::string &titleFont, float tR, float tG, float tB, float tA, const std::string &watermark, const std::string &watermarkFont, float wR, float wG, float wB, float wA, int interval)
{
  st.title = title;
  st.titleFont = titleFont;
  st.titlePosition.x = x;
  st.titlePosition.y = y;
  st.titleColor.r = tR;
  st.titleColor.g = tG;
  st.titleColor.b = tB;
  st.titleColor.a = tA;

  st.watermark = watermark;
  st.watermarkFont = watermarkFont;
  st.watermarkColor.r = wR;
  st.watermarkColor.g = wG;
  st.watermarkColor.b = wB;
  st.watermarkColor.a = wA;

  st.watermarkInterval = interval;
}

MediaObjectImpl *
ThatOverlayImplFactory::createObject (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline) const
{
  return new ThatOverlayImpl (config, mediaPipeline);
}

ThatOverlayImpl::StaticConstructor ThatOverlayImpl::staticConstructor;

ThatOverlayImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* thatoverlay */
} /* module */
} /* kurento */
