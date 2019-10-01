#ifdef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.h"
#include "GPXParser.h"

GPXdoc * new_gpxdoc(void);
Waypoint * new_waypoint(void);
Track * new_track(void);
TrackSegment * new_tracksegment(void);
Route * new_route(void);
char * new_string(void);
Attribute * new_attribute(void);
void free_string(char * toFree);
void create_doc_from_tree(xmlDoc * doc, GPX ** doc);

#endif
