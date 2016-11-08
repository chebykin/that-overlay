#ifndef _GST_THAT_OVERLAY_H_
#define _GST_THAT_OVERLAY_H_

#include <gst/video/gstvideofilter.h>

G_BEGIN_DECLS
#define GST_TYPE_THAT_OVERLAY   (gst_that_overlay_get_type())
#define GST_THAT_OVERLAY(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_THAT_OVERLAY,GstThatOverlay))
#define GST_THAT_OVERLAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_THAT_OVERLAY,GstThatOverlayClass))
#define GST_IS_THAT_OVERLAY(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_THAT_OVERLAY))
#define GST_IS_THAT_OVERLAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_THAT_OVERLAY))
typedef struct _GstThatOverlay GstThatOverlay;
typedef struct _GstThatOverlayClass GstThatOverlayClass;
typedef struct _GstThatOverlayPrivate GstThatOverlayPrivate;

struct _GstThatOverlay
{
  GstVideoFilter base;
  GstThatOverlayPrivate *priv;
};

struct _GstThatOverlayClass
{
  GstVideoFilterClass base_that_overlay_class;
};

GType gst_that_overlay_get_type (void);

gboolean gst_that_overlay_plugin_init (GstPlugin * plugin);

G_END_DECLS
#endif /* _GST_THAT_OVERLAY_H_ */
