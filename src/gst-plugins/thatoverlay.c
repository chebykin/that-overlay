#include <config.h>
#include <gst/gst.h>

#include "gstthatoverlay.h"

static gboolean
init (GstPlugin * plugin)
{
  if (!gst_that_overlay_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    thatoverlay,
    "Filter documentation",
    init, VERSION, GST_LICENSE_UNKNOWN, "PACKAGE_NAME", "origin")
