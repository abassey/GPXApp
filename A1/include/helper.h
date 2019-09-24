#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.h"
#include "GPXParser.h"

GPXdoc * new_gpxdoc(void);
Waypoint * new_waypoint(void);
Track * new_track(void);
Route * new_route (void);