
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
fprintf(stderr, "prepare\n");
  ThatOverlayImpl::CairoOverlayState *state = (ThatOverlayImpl::CairoOverlayState *)user_data;
GstVideoInfo info;

gst_video_info_from_caps (&info, caps);
state->width = info.width;
state->height = info.height;

  state->valid = TRUE;
}

static void
draw_overlay (GstElement * overlay, cairo_t * cr, guint64 timestamp, 
  guint64 duration, gpointer user_data)
{
fprintf(stderr, "draw_overlay\n");
  ThatOverlayImpl::CairoOverlayState *s = (ThatOverlayImpl::CairoOverlayState *)user_data;
  double scale;

  if (!s->valid)
    return;

  scale = 2*(((timestamp/(int)1e7) % 70)+30)/100.0;
  cairo_translate(cr, s->width/2, (s->height/2)-30);
  cairo_scale (cr, scale, scale);

  cairo_move_to (cr, 0, 0);
  cairo_curve_to (cr, 0,-30, -50,-30, -50,0);
  cairo_curve_to (cr, -50,30, 0,35, 0,60 );
  cairo_curve_to (cr, 0,35, 50,30, 50,0 ); 
  cairo_curve_to (cr, 50,-30, 0,-30, 0,0 );
  cairo_set_source_rgba (cr, 0.9, 0.0, 0.1, 0.7);
  cairo_fill (cr);
}

ThatOverlayImpl::ThatOverlayImpl (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline)  : FilterImpl (config, std::dynamic_pointer_cast<MediaObjectImpl>(mediaPipeline)) 
{
fprintf(stderr, "ThatOverlayImpl::ThatOverlayImpl\n");
  GstElement* sampleplugin;

  g_object_set (element, "filter-factory", "cairooverlay", NULL);

  g_object_get (G_OBJECT (element), "filter", &sampleplugin, NULL);

  if (sampleplugin == NULL) {
    throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
                            "Media Object not available");
  }

g_signal_connect (sampleplugin, "draw", G_CALLBACK (draw_overlay), &this->st);
g_signal_connect (sampleplugin, "caps-changed",   G_CALLBACK (prepare_overlay), &this->st);

  g_object_unref (sampleplugin);
}

MediaObjectImpl *
ThatOverlayImplFactory::createObject (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline) const
{
fprintf(stderr, "ThatOverlayImpl::createObject\n");
  return new ThatOverlayImpl (config, mediaPipeline);
}

ThatOverlayImpl::StaticConstructor ThatOverlayImpl::staticConstructor;

ThatOverlayImpl::StaticConstructor::StaticConstructor()
{
fprintf(stderr, "ThatOverlayImpl::StaticConstructor::StaticConstructor\n");
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* thatoverlay */
} /* module */
} /* kurento */
