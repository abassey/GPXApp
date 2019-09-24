#ifndef GPX_PARSER_H
#define GPX_PARSER_H

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemastypes.h>
#include "LinkedListAPI.h"

//Represents a generic GPX element/XML node Attribute
typedef struct  {
    //Attribute name.  Must not be NULL
	char* 	name;
    //Attribute value.  Must not be NULL
	char*	value;
} Attribute;

typedef struct {
    //Waypoint name.  Must not be NULL.  May be an empty string.
    char* name;
    //Waypoint longitude.  Must be initialized.
    float longitude;
    //Waypoint latitude.  Must be initialized.
    float latitude;
    //Additional waypoint attributes
    //All objects in the list will be of type Attribute.  It must not be NULL.  It may be empty.
    List* attributes;
} Waypoint;

typedef struct {
    //Route name.  Must not be NULL.  May be an empty string.
    char* name;
    //Waypoints that make up the route
    //All objects in the list will be of type Waypoint.  It must not be NULL.  It may be empty.
    List* waypoints;
    //Additional route attributes
    //All objects in the list will be of type Attribute.  It must not be NULL.  It may be empty.
    List* attributes;
} Route;

typedef struct {
    //Waypoints that make up the track segment
    //All objects in the list will be of type Waypoint.  It must not be NULL.  It may be empty.
    List* waypoints;
} TrackSegment;

typedef struct {
    //Track name.  Must not be NULL.  May be an empty string.
    char* name;
    //Segments that make up the track
    //All objects in the list will be of type TrackSegment.  It must not be NULL.  It may be empty.
    List* segments;

    //Additional route attributes
    //All objects in the list will be of type Attribute.  It must not be NULL.  It may be empty.
    List* attributes;
} Track;


typedef struct {

    //Namespace associated with our GPX doc.  Must not be empty.  While a real GPX doc might have
    //multiple namespaces associated with it, will will assume there is only one
    char namespace[256];

    //GPX version.  Must be initialized.  Will usually be 1.1
    float version;

    //GPX creator. Must not be NULL. Must not be an empty string.
    char* creator;

    //Waypoints in the GPX file
    //All objects in the list will be of type Waypoint.  It must not be NULL.  It may be empty.
    List* waypoints;

    //Routes in the GPX file
    //All objects in the list will be of type Route.  It must not be NULL.  It may be empty.
    List* routes;

    //Tracks in the GPX file
    //All objects in the list will be of type Track.  It must not be NULL.  It may be empty.
    List* tracks;
} GPXdoc;



//A1

/* Public API - main */

/** Function to create an GPX object based on the contents of an GPX file.
 *@pre File name cannot be an empty string or NULL.
       File represented by this name must exist and must be readable.
 *@post Either:
        A valid GPXdoc has been created and its address was returned
		or
		An error occurred, and NULL was returned
 *@return the pinter to the new struct or NULL
 *@param fileName - a string containing the name of the GPX file
**/
GPXdoc* createGPXdoc(char* fileName);

/** Function to create a string representation of an GPX object.
 *@pre GPX object exists, is not null, and is valid
 *@post GPX has not been modified in any way, and a string representing the GPX contents has been created
 *@return a string contaning a humanly readable representation of an GPX object
 *@param obj - a pointer to an GPX struct
**/
char* GPXdocToString(GPXdoc* doc);

/** Function to delete doc content and free all the memory.
 *@pre GPX object exists, is not null, and has not been freed
 *@post GPX object had been freed
 *@return none
 *@param obj - a pointer to an GPX struct
**/
void deleteGPXdoc(GPXdoc* doc);

/* For the five "get..." functions below, return the count of specified entities from the file.  They all share the same format,
    and only differ in what they have to count.

 *@pre GPX object exists, is not null, and has not been freed
 *@post GPX object has not been modified in any way
 *@return the number of entities in the object
 *@param obj - a pointer to an GPXdoc struct
 */

//http://www.trackreport.net
//Routes, tracks, and wayppoints must be stored in the same order as they appera in the file

//Number of waypoints in the GPX file - i.e. objects in the waypoint list
int getNumWaypoints(const GPXdoc* doc);

//Number of routes in the GPX file
int getNumRoutes(const GPXdoc* doc);

//Number of tracks in the GPX file
int getNumTracks(const GPXdoc* doc);

//Total number of segments in all tracks in the document
int getNumSegments(const GPXdoc* doc);

//Total number of attributes in the document
int getNumAttributes(const GPXdoc* doc);



// Function that returns a waypoint with the given name.  If more than one exists, return the first one.
Waypoint* getWaypoint(const GPXdoc* doc, char* name);
// Function that returns a track with the given name.  If more than one exists, return the first one.
Track* getTrack(const GPXdoc* doc, char* name);
// Function that returns a route with the given name.  If more than one exists, return the first one.
Route* getRoute(const GPXdoc* doc, char* name);

/* ******************************* List helper functions  - MUST be implemented *************************** */

void deleteAttribute( void* data);
char* attributeToString( void* data);
int compareAttributes(const void *first, const void *second);

void deleteWaypoint(void* data);
char* waypointToString( void* data);
int compareWaypoints(const void *first, const void *second);

void deleteRoute(void* data);
char* routeToString(void* data);
int compareRoutes(const void *first, const void *second);

void deleteTrackSegment(void* data);
char* trackSegmentToString(void* data);
int compareTrackSegments(const void *first, const void *second);

void deleteTrack(void* data);
char* trackToString(void* data);
int compareTracks(const void *first, const void *second);


#endif
