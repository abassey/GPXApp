#include "GPXParser.h"
#include "helper.h"

GPXdoc* createGPXdoc(char* fileName)
{
    if fileName == NULL return NULL;

    xmlDoc * doc = NULL;
    xmlNode * root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(argv[1], NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    if doc == NULL return NULL;

    GPXdoc * gdoc = new_gpxdoc();
    
    
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return gdoc;
}

char* GPXdocToString(GPXdoc* doc)
{
    if doc == NULL return NULL;


}

void deleteGPXdoc(GPXdoc* doc)
{
    free(doc->creator);

    //free each waypoint with list iterator then free whole list
    freeList(doc->waypoints);

    //free each route with list iterator then free whole list
    freeList(doc->routes);

    //free each track with list iterator then free whole list
    freeList(doc->tracks);
    free(doc);
}

int getNumWaypoints(const GPXdoc* doc)
{
    int num_waypoints = 0;

    num_waypoints = getLength(doc->waypoints);

    return num_waypoints;
}

int getNumRoutes(const GPXdoc* doc)
{
    int num_routes = 0;

    num_routes = getLength(doc->routes);

    return num_routes;
}

int getNumTracks(const GPXdoc* doc)
{
    int num_tracks = 0;

    num_tracks = getLength(doc->tracks);

    return num_tracks;
}

int getNumSegments(const GPXdoc* doc)
{
    int num_segments = 0;

    num_segments = getLength(doc->waypoints->segments);

    return num_segments;
}

int getNumAttributes(const GPXdoc* doc)
{

}

Waypoint* getWaypoint(const GPXdoc* doc, char* name)
{
    Waypoint * found_waypoint = new_waypoint();

    findElement(doc->waypoints, compareWaypoints(), name);
    //find out what this returns/how this works
}

Track* getTrack(const GPXdoc* doc, char* name)
{
     Track * found_track = new_track();

    findElement(doc->tracks, compareTracks(), name);
    //find out what this returns/how this works
}

Route* getRoute(const GPXdoc* doc, char* name)
{
     Route * found_route = new_route();

    findElement(doc->routes, compareRoutes(), name);
    //find out what this returns/how this works
}

/* ******************************* List helper functions  - MUST be implemented *************************** */

void deleteAttribute( void* data)
{
    Attribute * temp = (Attribute *) data;
    free(temp->name);
    free(temp->value);
    free(temp);
}

char* attributeToString( void* data)
{
    char * temp = (Attribute *) data;

}

int compareAttributes(const void *first, const void *second)
{

}

void deleteWaypoint(void* data)
{
    Waypoint * temp = (Waypoint *) data;
    free(temp->name);
    freeList(temp->attributes);
    free(temp);
}

char* waypointToString( void* data)
{
    char buffer[50];

    char * temp = (Waypoint *) data;
    strcpy(temp,"name: ");
    strcpy(temp,data->name);
    strcpy(temp,", ");

    strcpy(temp,"longitude: ");
    gcvt(data->longitude, 10, buffer);
    strcpy(temp, buffer);
    strcpy(temp,", ");

    strcpy(temp,"latitude: ");
    gcvt(data->latitude, 10, buffer);
    strcpy(temp,buffer);
    strcpy(temp,", ");

    strcpy(temp,toString(data->attributes));
    return temp;
}

int compareWaypoints(const void *first, const void *second)
{

}

void deleteRoute(void* data)
{
    Route * temp = (Route *) data;
    free(temp->name);
    freeList(temp->waypoints);
    freeList(temp->attributes);
    free(temp);
}

char* routeToString(void* data)
{

}

int compareRoutes(const void *first, const void *second)
{

}

void deleteTrackSegment(void* data)
{

}

char* trackSegmentToString(void* data)
{

}

int compareTrackSegments(const void *first, const void *second)
{

}

void deleteTrack(void* data)
{

}

char* trackToString(void* data)
{

}

int compareTracks(const void *first, const void *second)
{

}
