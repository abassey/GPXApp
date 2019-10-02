#include "GPXParser.h"
#include "helper.h"
#include "LinkedListAPI.h"

GPXdoc* createGPXdoc(char* fileName)
{
    if(fileName == NULL) return NULL;

    xmlDoc * doc = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(argv[1], NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    if (doc == NULL && root_element == NULL) return NULL;
    
   // char* (*wpToStr)(void*) = &waypointToString;
	//void (*delWp)(void*) = &free_string;
	//int (*compWp)(void*) = &compare_strings;
    
    //List * tree = initializeList();

    GPXdoc * gdoc = new_gpxdoc();
    
    //turn tree doc to list, then iterate through the list
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return gdoc;
}

char* GPXdocToString(GPXdoc* doc)
{
    if(doc == NULL) return NULL;

	char * temp = new_string();
	char buffer[50];
	
	strcpy(temp, "namespace: ");
	strcpy(temp, doc->namespace);
	strcpy(temp, ", version: ");
	gcvt(doc->version, 10, buffer);
	strcpy(temp, buffer);
	strcpy(temp, ", creator: ");
	strcpy(temp, doc->creator);
	strpy(temp, ", waypoints: [");
	strcpy(temp, toString(doc->waypoints));
	strcpy(temp, "], routes: [");
	strcpy(temp, toString(doc->routes));
	strcpy(temp, "], tracks: [");
	strcpy(temp, toString(doc->tracks));
	strcpy(temp, "]");
	
	return temp;
	
}

void deleteGPXdoc(GPXdoc* doc)
{
	if(doc == NULL) return NULL;
	
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
	if(doc == NULL) return NULL;
	
    int num_waypoints = 0;

    num_waypoints = getLength(doc->waypoints);

    return num_waypoints;
}

int getNumRoutes(const GPXdoc* doc)
{
	if(doc == NULL) return NULL;
	
    int num_routes = 0;

    num_routes = getLength(doc->routes);

    return num_routes;
}

int getNumTracks(const GPXdoc* doc)
{
	if(doc == NULL) return NULL;
	
    int num_tracks = 0;

    num_tracks = getLength(doc->tracks);

    return num_tracks;
}

int getNumSegments(const GPXdoc* doc)
{
	if(doc == NULL) return NULL; 
	
    int num_segments = 0;

    num_segments = getLength(doc->waypoints->segments);

    return num_segments;
}

int getNumAttributes(const GPXdoc* doc)
{
	if(doc == NULL) return NULL; 
	
	int num_attributes = 0;
	
	num_attributes = getLength(doc->tracks->attributes);
	num_attributes += getLength(doc->routes->attirbutes);
	num_attributes += getLength(doc->waypoints->attirbutes);
	
	return num_attributes;
}

Waypoint* getWaypoint(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Waypoint * found_waypoint = new_waypoint();
	strcpy(found_waypoint->name, name);
	
    ListIterator itr = createIterator(doc->tracks);

	void* data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareWaypoints(data, found_track))
			{
				found_waypoint->longitude = data->longitude;
				found_waypoint->latitude = data->latitude;
				found_waypoint->attributes = data->attributes;
				return found_waypoint;
			}

		data = nextElement(&itr);
	}

	return NULL;
}

Track* getTrack(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Track * found_track = new_track();
    strcpy(found_track->name, name);

    ListIterator itr = createIterator(doc->tracks);

	void* data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareTracks(data, found_track))
			{
				found_track->segments = data->segments;
				found_track->attributes = data->attributes;
				return found_track;
			}

		data = nextElement(&itr);
	}

	return NULL;
}

Route* getRoute(const GPXdoc* doc, char* name)
{
	 if(doc == NULL) return NULL;
	 
     Route * found_route = new_route();
     strcpy(found_route->name, name);
     
     ListIterator itr = createIterator(doc->routes);

	void* data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareRoutes(data, found_track))
			{
				found_route->waypoints = data->waypoints;
				found_route->attributes = data->attributes;
				return found_route;
			}

		data = nextElement(&itr);
	}

	return NULL;
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
	Attribute * att = (Attribute *) data;
    char * temp = new_string();
    strcpy(temp, "name: ");
    strcpy(temp, att->name);
    strcpy(temp, ", ");
    strcpy(temp, "value: ");
    strcpy(temp, att->value);
    
    return temp;

}

int compareAttributes(const void *first, const void *second)
{
	const Attribute * a_first = (const Attribute *) first;
	const Attribute * b_second = (const Attribute *) second;
	
	return (strcmp(a_first->name, b_second->name) && strcmp(a_first->value, b_second->value));
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

    Waypoint * wp = (Waypoint *) data;
    char * temp = new_string();
    strcpy(temp,"name: ");
    strcpy(temp,wp->name);
    strcpy(temp,", ");

    strcpy(temp,"longitude: ");
    gcvt(wp->longitude, 10, buffer);
    strcpy(temp, buffer);
    strcpy(temp,", ");

    strcpy(temp,"latitude: ");
    gcvt(wp->latitude, 10, buffer);
    strcpy(temp,buffer);
    strcpy(temp,", [");

    strcpy(temp,toString(wp->attributes));
    strcpy(temp, "]");
    return temp;
}

int compareWaypoints(const void *first, const void *second)
{
	const Waypoint * a_first = (const Waypoint *) first;
	const Waypoint * b_second = (const Waypoint *) second;
	
	return strcmp(a_first->name, b_second->name);
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
	Route * rt = (Route *) data;
	
	char * temp = new_string();
	strcpy(temp, "name: ");
	strcpy(temp, rt->name);
	strcpy(temp, ", [");
	strcpy(temp, toString(rt->waypoints));
	strcpy(temp, "], [");
	strcpy(temp, toString(rt->attributes));
	strcpy(temp, " ]");
	
	return temp;
}

int compareRoutes(const void *first, const void *second)
{
	const Route * a_first = (const Route *) first;
	const Route * b_second = (const Route *) second;
	
	return (strcmp(a_first->name, b_second->name));
}

void deleteTrackSegment(void* data)
{
	TrackSegment * temp = (TrackSegment *) data;
	freeList(temp->waypoints);
	free(temp);
}

char* trackSegmentToString(void* data)
{
	TrackSegment * ts = (TrackSegment *) data;
	char * temp = new_string();
	
	strcpy(temp, toString(ts->waypoints));
	
	return temp;
}

int compareTrackSegments(const void *first, const void *second)
{
	if(first == second) return 0;
	else if (first < second) return -1;
	 
	return 1;
}

void deleteTrack(void* data)
{	
	Track * temp = (Track *) data;
	free(temp->name);
	freeList(temp->segments);
	freeList(temp->attributes);
	free(temp);
}

char* trackToString(void* data)
{
	Track * tr = (Track *) data;
	char * temp = new_string();
	
	strcpy(temp, "name: ");
	strcpy(temp, tr->name);
	strcpy(temp, ", [");
	strcpy(temp, toString(tr->segments));
	strcpy(temp, "] , [");
	strcpy(temp, toString(tr->attributes));
	strcpy(temp, "] ");
	
	return temp;
}

int compareTracks(const void *first, const void *second)
{
	const Track * a_first = (const Track *) first;
	const Track * b_second = (const Track *) second;
	
	return (strcmp(a_first->name, b_second->name));
}
