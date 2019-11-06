#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemastypes.h>
#include "LinkedListAPI.h"
#include "GPXParser.h"

#define R 6371
#define TO_RAD (3.1415926536 / 180)

GPXdoc * new_gpxdoc(void);
Waypoint * new_waypoint(void);
Track * new_track(void);
TrackSegment * new_tracksegment(void);
Route * new_route(void);
char * new_string(void);
Attribute * new_attribute(void);
void free_string(void * toFree);
void create_doc_from_tree(xmlNode * root, GPXdoc ** gdoc);
void create_tree_from_doc(xmlNodePtr * root, GPXdoc * gdoc);
float dist_calc(float lat1, float lon1, float lat2, float lon2);
void custom_strcpy(char * dest, char * scr, int start_index, int end_index);
char ** new_array(void);
void free_array(void ** arr);


#endif
