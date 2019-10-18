#include "GPXParser.h"
#include "helper.h"
#include "LinkedListAPI.h"
#include <stdio.h>

GPXdoc* createGPXdoc(char* fileName)
{
    if(fileName == NULL) return NULL;

    xmlDoc * doc = NULL;
    xmlNode * root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(fileName, NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    if (doc == NULL) return NULL;
    
    //printf("in gdoc creator\n");

    GPXdoc * gdoc = new_gpxdoc();
    
    create_doc_from_tree(root_element, &gdoc);
    
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
	strcat(temp, doc->namespace);
	strcat(temp, ", version: ");
	snprintf(buffer, sizeof(buffer), "%.2f", doc->version);
	strcat(temp, buffer);
	strcat(temp, ", creator: ");
	strcat(temp, doc->creator);
	strcat(temp, ", waypoints: [");
	strcat(temp, toString(doc->waypoints));
	strcat(temp, "], routes: [");
	strcat(temp, toString(doc->routes));
	strcat(temp, "], tracks: [");
	strcat(temp, toString(doc->tracks));
	strcat(temp, "]");
	
	return temp;
	
}

void deleteGPXdoc(GPXdoc* doc)
{
	if(doc == NULL) return;
	
    free_string(doc->creator);

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
	if(doc == NULL) return -1;
	
    int num_waypoints = 0;

    num_waypoints = getLength(doc->waypoints);

    return num_waypoints;
}

int getNumRoutes(const GPXdoc* doc)
{
	if(doc == NULL) return -1;
	
    int num_routes = 0;

    num_routes = getLength(doc->routes);

    return num_routes;
}

int getNumTracks(const GPXdoc* doc)
{
	if(doc == NULL) return -1;
	
    int num_tracks = 0;

    num_tracks = getLength(doc->tracks);

    return num_tracks;
}

int getNumSegments(const GPXdoc* doc)
{
	if(doc == NULL) return -1; 
	
    int num_segments = 0;

    //num_segments = getLength(doc->waypoints->segments);
    
    ListIterator itr = createIterator(doc->tracks);

	Track * data = nextElement(&itr);
	while (data != NULL)
	{
		num_segments += getLength(data->segments);
		
		data = nextElement(&itr);
	}

    return num_segments;
}

int getNumAttributes(const GPXdoc* doc)
{
	if(doc == NULL) return -1; 
	
	int num_attributes = 0;
	
	ListIterator itr = createIterator(doc->waypoints);
	
	Waypoint * wp = nextElement(&itr);
	while (wp != NULL)
	{
		if(strlen(wp->name) != 0) num_attributes++;
		
		num_attributes += getLength(wp->attributes);
		
		wp = nextElement(&itr);
	}
	
	itr = createIterator(doc->routes);
	
	//getting num for routes and its waypoints
	Route * rte = nextElement(&itr);
	while (rte != NULL)
	{
		ListIterator inner_itr = createIterator(rte->waypoints);
				
		Waypoint * rte_wp = nextElement(&inner_itr);
		
		while (rte_wp != NULL)
		{
			if(strlen(rte_wp->name) != 0) num_attributes++;
			
			num_attributes += getLength(rte_wp->attributes);
			
			rte_wp = nextElement(&inner_itr);
		}
		
		if(strlen(rte->name) != 0) num_attributes++;
		
		num_attributes += getLength(rte->attributes);
		
		rte = nextElement(&itr);
	}	
	
	itr = createIterator(doc->tracks);

	//getting num for tracks and track segments
	Track * trk = nextElement(&itr);
	while (trk != NULL)
	{
		ListIterator seg_itr = createIterator(trk->segments);
		
		TrackSegment * ts = nextElement(&seg_itr);
			while (ts != NULL)
			{
				ListIterator inner_itr = createIterator(ts->waypoints);
				
				Waypoint * ts_wp = nextElement(&itr);
				
				while (ts_wp != NULL)
				{
					if(strlen(ts_wp->name) != 0) num_attributes++;
					
					num_attributes += getLength(ts_wp->attributes);
					
					ts_wp = nextElement(&inner_itr);
				}
				
				ts = nextElement(&seg_itr);
			}
		
		if(strlen(trk->name) != 0) num_attributes++;
			
		num_attributes += getLength(trk->attributes);
		
		trk = nextElement(&itr);
	}
	
	return num_attributes;
}

Waypoint* getWaypoint(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Waypoint * found_waypoint = new_waypoint();
	strcpy(found_waypoint->name, name);
	
	ListIterator itr = createIterator(doc->waypoints);
	
	Waypoint * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareWaypoints(data, found_waypoint))
			{
				found_waypoint->longitude = data->longitude;
				found_waypoint->latitude = data->latitude;
				found_waypoint->attributes = data->attributes;
				return found_waypoint;
			}

		data = nextElement(&itr);
	}
	
	itr = createIterator(doc->routes);
	
	//getting num for routes and its waypoints
	Route * rte = nextElement(&itr);
	while (rte != NULL)
	{
		ListIterator inner_itr = createIterator(rte->waypoints);
				
		data = nextElement(&inner_itr);
			while (data != NULL)
			{
				if (compareWaypoints(data, found_waypoint))
					{
						found_waypoint->longitude = data->longitude;
						found_waypoint->latitude = data->latitude;
						found_waypoint->attributes = data->attributes;
						return found_waypoint;
					}

				data = nextElement(&inner_itr);
			}
		
		rte = nextElement(&itr);
	}	
	
	itr = createIterator(doc->tracks);

	//getting num for tracks and track segments
	Track * trk = nextElement(&itr);
	while (trk != NULL)
	{
		ListIterator seg_itr = createIterator(trk->segments);
		
		TrackSegment * ts = nextElement(&seg_itr);
			while (ts != NULL)
			{
				ListIterator inner_itr = createIterator(ts->waypoints);
				
				data = nextElement(&inner_itr);
					while (data != NULL)
					{
						if (compareWaypoints(data, found_waypoint) == 0)
							{
								found_waypoint->longitude = data->longitude;
								found_waypoint->latitude = data->latitude;
								found_waypoint->attributes = data->attributes;
								return found_waypoint;
							}

						data = nextElement(&inner_itr);
					}
				
				ts = nextElement(&seg_itr);
			}
		trk = nextElement(&itr);
	}

	return NULL;
}

Track* getTrack(const GPXdoc* doc, char* name)
{
	if(doc == NULL || name == NULL) return NULL;
	
    Track * found_track = new_track();
    strcpy(found_track->name, name);

    ListIterator itr = createIterator(doc->tracks);

	Track * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareTracks(data, found_track) == 0)
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

	Route * data = nextElement(&itr);
	while (data != NULL)
	{
		if (compareRoutes(data, found_route) == 0)
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
    strcat(temp, att->name);
    strcat(temp, ", ");
    strcat(temp, "value: ");
    strcat(temp, att->value);
    
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
    strcat(temp,wp->name);
    strcat(temp,", ");

    strcpy(temp,"longitude: ");
    snprintf(buffer, sizeof(buffer), "%f", wp->longitude);
    strcat(temp, buffer);
    strcat(temp,", ");

    strcat(temp,"latitude: ");
    snprintf(buffer, sizeof(buffer), "%f", wp->latitude);
    strcat(temp,buffer);
    strcat(temp,", [");

    strcat(temp,toString(wp->attributes));
    strcat(temp, "]");
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
	strcat(temp, rt->name);
	strcat(temp, ", [");
	strcat(temp, toString(rt->waypoints));
	strcat(temp, "], [");
	strcat(temp, toString(rt->attributes));
	strcat(temp, " ]");
	
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
	strcat(temp, tr->name);
	strcat(temp, ", [");
	strcat(temp, toString(tr->segments));
	strcat(temp, "] , [");
	strcat(temp, toString(tr->attributes));
	strcat(temp, "] ");
	
	return temp;
}

int compareTracks(const void *first, const void *second)
{
	const Track * a_first = (const Track *) first;
	const Track * b_second = (const Track *) second;
	
	return (strcmp(a_first->name, b_second->name));
}


bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile)
{
	bool valid = false;
	
	if(gpxSchemaFile == NULL) return NULL;
	if(doc == NULL) return NULL;
	
	xmlDocPtr xml_doc = NULL;
	xmlNodePtr root = NULL;
	
	LIBXML_TEST_VERSION;
	
	xml_doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "gpx");
	xmlDocSetRootElement(xml_doc, root); 
	
	
	create_tree_from_doc( &root, doc);
	
	//xmlSaveFormatFileEnc("gpx.xml" , xml_doc, "UTF-8", 1);
	
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	//char *XMLFileName = "gpx.xml";


	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
	//xmlSchemaDump(stdout, schema); //To print schema dump

	//xml_doc = xmlReadFile(XMLFileName, NULL, 0);

	if (xml_doc == NULL)
	{ 
		fprintf(stderr, "Could not parse");
	} else {
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, xml_doc);
		if (ret == 0)
		{
			//printf("%s validates\n", XMLFileName);
			valid = true;
		}else if (ret > 0) {
			//printf("%s fails to validate\n", XMLFileName);
			valid = false;
		} else {
			//printf("%s validation generated an internal error\n", XMLFileName);
			return NULL;
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(xml_doc);
	}

	// free the resource
	if(schema != NULL)
	xmlSchemaFree(schema);

	xmlSchemaCleanupTypes();
	//sxmlFreeDoc(xml_doc);
	xmlCleanupParser();
	xmlMemoryDump();
	
	return valid;
}

bool writeGPXdoc(GPXdoc* doc, char* fileName)
{
	bool write = false;
	
	if(fileName == NULL) return NULL;
	if(doc == NULL) return NULL;
	
	write = validateGPXDoc(doc, "gpx.xsd");
	
	if(write == true)
	{
		xmlDocPtr xml_doc = NULL;
		xmlNodePtr root = NULL;
		
		LIBXML_TEST_VERSION;
		
		xml_doc = xmlNewDoc(BAD_CAST "1.0");
		root = xmlNewNode(NULL, BAD_CAST "gpx");
		xmlDocSetRootElement(xml_doc, root); 
		
		create_tree_from_doc( &root, doc);
		
		xmlSaveFormatFileEnc(fileName , xml_doc, "UTF-8", 1);
	} else {
		fprintf(stderr, "Error writing to file\n\n");
	}
	
	return write;
}
